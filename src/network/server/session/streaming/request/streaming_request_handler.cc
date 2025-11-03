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
    request_reader->set_request(request_bytes, request_size);

    const auto request_type = request_reader->read_request_type();
    switch (request_type) {
    case Protocol::RequestType::QUERY: {
        logger(Category::Debug) << "Request received: QUERY";
        request_reader->check_datatype(Protocol::DataType::STRING);
        const auto query = request_reader->read_string();
        logger(Category::Info) << "\nQuery:\n" << trim_string(query) << "\n";
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

void StreamingRequestHandler::handle_readonly_run()
{
    auto version_scope = buffer_manager.init_version_readonly();
    {
        std::lock_guard<std::mutex> lock(session.get_thread_info_vec_mutex());
        get_query_ctx().prepare(*version_scope, session.get_timeout());
    }

    logger(Category::Info) << "Cancellation: " << get_query_ctx().thread_info.worker_index << ' '
                           << get_query_ctx().cancellation_token;

    // Request must be read here because query_ctx.prepare() clears all posible tmp that could come as parameters
    const auto input_parameters = request_reader->read_parameters();
    std::stringstream parameters_ss;
    if (!input_parameters.empty()) {
        parameters_ss << "Parameters:\n";
        for (const auto& [var_name, object_id] : input_parameters) {
            parameters_ss << var_name << " -> " << object_id << "\n";
        }
    }
    logger(Category::Info) << parameters_ss.str();

    auto parser_start = std::chrono::system_clock::now();
    create_logical_plan(input_parameters);
    auto parser_duration = get_duration(parser_start);

    auto optimizer_start = std::chrono::system_clock::now();
    auto executor = create_streaming_executor();
    auto optimizer_duration = get_duration(optimizer_start);

    logger.log(Category::PhysicalPlan, [&](std::ostream& os) {
        executor->analyze(os, false);
        os << '\n';
    });

    // Send the variables
    response_writer->write_variables(
        executor->projection_vars,
        get_query_ctx().thread_info.worker_index,
        get_query_ctx().cancellation_token
    );
    response_writer->flush();

    auto execution_start = std::chrono::system_clock::now();
    auto result_count = executor->execute(*response_writer);
    auto execution_duration = get_duration(execution_start);

    logger.log(Category::ExecutionStats, [&](std::ostream& os) {
        executor->analyze(os, true);
        os << '\n';
    });

    logger(Category::Info) << "Results:            " << result_count << "\n"
                           << "Parser duration:    " << parser_duration.count() << " ms\n"
                           << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                           << "Execution duration: " << execution_duration.count() << " ms";

    response_writer->write_records_success(
        result_count,
        parser_duration.count(),
        optimizer_duration.count(),
        execution_duration.count()
    );
    response_writer->flush();
}

void StreamingRequestHandler::handle_update_run()
{
    // Mutex to allow only one write query at a time
    std::lock_guard<std::mutex> lock(session.server.update_execution_mutex);

    auto version_scope = buffer_manager.init_version_editable();
    {
        std::lock_guard<std::mutex> lock(session.get_thread_info_vec_mutex());
        get_query_ctx().prepare(*version_scope, session.get_timeout());
    }

    // Request must be read here because query_ctx.prepare() clears all posible tmp that could come as parameters
    const auto input_parameters = request_reader->read_parameters();
    std::stringstream parameters_ss;
    if (!input_parameters.empty()) {
        parameters_ss << "Parameters:\n";
        for (const auto& [var_name, object_id] : input_parameters) {
            parameters_ss << var_name << " -> " << object_id << "\n";
        }
    }
    logger(Category::Info) << parameters_ss.str();

    auto parser_start = std::chrono::system_clock::now();
    create_logical_plan(input_parameters);
    auto parser_duration = get_duration(parser_start);

    auto optimizer_start = std::chrono::system_clock::now();
    auto executor = create_streaming_executor();
    auto optimizer_duration = get_duration(optimizer_start);

    logger.log(Category::PhysicalPlan, [&](std::ostream& os) {
        executor->analyze(os, false);
        os << '\n';
    });

    // Send the variables
    response_writer->write_variables(
        executor->projection_vars,
        get_query_ctx().thread_info.worker_index,
        get_query_ctx().cancellation_token
    );
    response_writer->flush();

    auto execution_start = std::chrono::system_clock::now();
    executor->execute(*response_writer);
    version_scope->commited = true;
    auto execution_duration = get_duration(execution_start);

    logger.log(Category::ExecutionStats, [&](std::ostream& os) {
        executor->analyze(os, true);
        os << '\n';
    });

    logger(Category::Info) << "Parser duration:    " << parser_duration.count() << " ms\n"
                           << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                           << "Execution duration: " << execution_duration.count() << " ms";

    response_writer->write_update_success(
        parser_duration.count(),
        optimizer_duration.count(),
        execution_duration.count()
    );
    response_writer->flush();
}

void StreamingRequestHandler::handle_run(const std::string& query)
{
    try {
        initial_parse(query);

        if (is_update()) {
            handle_update_run();
        } else {
            handle_readonly_run();
        }
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
        const auto msg = e.what();
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
    request_reader->check_datatype(Protocol::DataType::UINT32);
    const uint_fast32_t worker_idx = request_reader->read_uint32();
    request_reader->check_datatype(Protocol::DataType::STRING);
    const std::string cancel_token = request_reader->read_string();

    bool res = session.try_cancel(worker_idx, cancel_token);

    if (res) {
        response_writer->write_cancel_success();
    } else {
        response_writer->write_error("Query cancellation failed");
    }

    response_writer->flush();
}
