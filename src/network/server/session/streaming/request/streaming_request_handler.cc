#include "streaming_request_handler.h"

#include "misc/logger.h"
#include "misc/trim.h"
#include "network/exceptions.h"
#include "network/server/protocol.h"
#include "query/exceptions.h"
#include "query/query_context.h"

using namespace MDBServer;

void StreamingRequestHandler::handle(const uint8_t* request_bytes, std::size_t request_size)
{
    request_reader.set_request(request_bytes, request_size);

    const auto request_type = request_reader.read_request_type();
    switch (request_type) {
    case Protocol::RequestType::QUERY: {
        const auto query = request_reader.read_string();
        logger(Category::Info) << "\nQuery received:\n" << trim_string(query) << "\n";
        handle_run(query);
        break;
    }
    case Protocol::RequestType::CATALOG: {
        logger(Category::Debug) << "Request received: CATALOG";
        handle_catalog();
        break;
    }
    case Protocol::RequestType::CANCEL: {
        logger(Category::Debug) << "Request received: CANCEL";
        handle_cancel();
        break;
    }
    default: {
        throw ProtocolException("Unhandled request type: " + Protocol::request_type_to_string(request_type));
    }
    }
}

void StreamingRequestHandler::handle_run(const std::string& query)
{
    auto readonly_version_scope = buffer_manager.init_version_readonly();

    {
        std::lock_guard<std::mutex> lock(session.get_thread_info_vec_mutex());
        get_query_ctx().prepare(*readonly_version_scope, session.get_timeout());
    }

    logger(Category::Info) << "Cancellation: " << get_query_ctx().thread_info.worker_index << ' '
                           << get_query_ctx().cancellation_token;

    try {
        auto parser_start = std::chrono::system_clock::now();
        auto current_logical_plan = create_logical_plan(query);
        parser_duration_ms = get_duration(parser_start);

        if (is_update(current_logical_plan)) {
            execute_update(current_logical_plan, *readonly_version_scope);
            return;
        }

        auto optimizer_start = std::chrono::system_clock::now();
        auto current_physical_plan = create_readonly_physical_plan(current_logical_plan);
        optimizer_duration_ms = get_duration(optimizer_start);

        logger.log(Category::PhysicalPlan, [&](std::ostream& os) {
            current_physical_plan->analyze(os, false);
            os << '\n';
        });

        // Send the variables
        const auto projection_vars = current_physical_plan->get_projection_vars();
        response_writer->write_variables(
            projection_vars,
            get_query_ctx().thread_info.worker_index,
            get_query_ctx().cancellation_token
        );
        response_writer->flush();

        // Stream all the records
        auto execution_start = std::chrono::system_clock::now();
        const uint64_t result_count = current_physical_plan->execute(*response_writer);
        execution_duration_ms = get_duration(execution_start);

        logger.log(Category::ExecutionStats, [&](std::ostream& os) {
            current_physical_plan->analyze(os, true);
            os << '\n';
        });

        logger(Category::Info) << "Results:            " << result_count << "\n"
                                  "Parser duration:    " << parser_duration_ms.count() << " ms\n"
                                  "Optimizer duration: " << optimizer_duration_ms.count() << " ms\n"
                                  "Execution duration: " << execution_duration_ms.count() << " ms";

        response_writer->write_records_success(
            result_count,
            parser_duration_ms.count(),
            optimizer_duration_ms.count(),
            execution_duration_ms.count()
        );
        response_writer->flush();
    } catch (const QueryException& e) {
        const auto msg = std::string("Query Exception: ") + e.what();
        logger(Category::Error) << msg;
        response_writer->write_error(msg);
        response_writer->flush();
    } catch (const LogicException& e) {
        const auto msg = std::string("Logic Exception: ") + e.what();
        logger(Category::Error) << msg;
        response_writer->write_error(msg);
        response_writer->flush();
    } catch (const InterruptedException& e) {
        const auto msg = std::string("Interrupt Exception: ") + e.what();
        logger(Category::Error) << msg;
        response_writer->write_error(msg);
        response_writer->flush();
    } catch (const QueryExecutionException& e) {
        const auto msg = std::string("Query Execution Exception: ") + e.what();
        logger(Category::Error) << msg;
        response_writer->write_error(msg);
        response_writer->flush();
    } catch (const std::exception& e) {
        const auto msg = std::string("Exception: ") + e.what();
        logger(Category::Error) << msg;
        response_writer->write_error(msg);
        response_writer->flush();
    } catch (...) {
        const auto msg = std::string("Unknown exception");
        logger(Category::Error) << msg;
        response_writer->write_error(msg);
        response_writer->flush();
    }
}

void StreamingRequestHandler::handle_catalog()
{
    response_writer->write_catalog_success();
    response_writer->flush();
}

void StreamingRequestHandler::handle_cancel()
{
    uint_fast32_t worker_idx = request_reader.read_uint32();
    auto cancel_token = request_reader.read_string();

    bool res = session.try_cancel(worker_idx, cancel_token);

    if (res) {
        response_writer->write_cancel_success();
    } else {
        response_writer->write_error("Query cancellation failed");
    }

    response_writer->flush();
}

bool StreamingRequestHandler::is_update(const OpUptr& uptr) {
    if (std::holds_alternative<std::unique_ptr<GQL::Op>>(uptr)) {
        return false;
    } else if (std::holds_alternative<std::unique_ptr<MQL::Op>>(uptr)) {
        return !std::get<std::unique_ptr<MQL::Op>>(uptr)->read_only();
    } else if (std::holds_alternative<std::unique_ptr<SPARQL::Op>>(uptr)) {
        return false;
    }
    return false;
}
