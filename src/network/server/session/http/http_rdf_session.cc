#include "http_rdf_session.h"

#include <boost/beast/ssl.hpp>

#include "misc/logger.h"
#include "misc/trim.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/http/request_parser.h"
#include "network/server/session/http/response/http_response_buffer.h"
#include "network/server/session/http/response/return_codes_helper.h"
#include "network/server/session/http/sparql_request_parser.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/optimizer/rdf_model/executor_constructor.h"
#include "query/parser/sparql_query_parser.h"
#include "query/parser/sparql_update_parser.h"
#include "query/query_context.h"
#include "query/update/sparql/update_executor.h"
#include "system/buffer_manager.h"

using namespace SPARQL;
using namespace boost;
using namespace MDBServer;
namespace http = beast::http;
namespace chrono = std::chrono;

template<typename stream_t>
HttpRdfSession<stream_t>::HttpRdfSession(
    Server& server,
    stream_t&& stream,
    http::request<http::string_body>&& request,
    chrono::seconds query_timeout
) :
    server(server),
    stream(std::move(stream)),
    request(std::move(request)),
    query_timeout(query_timeout)
{
    worker = get_query_ctx().thread_info.worker_index;
}

template<typename stream_t>
HttpRdfSession<stream_t>::~HttpRdfSession()
{
    if constexpr (std::is_same_v<stream_t, asio::ip::tcp::socket>) {
        if (stream.is_open()) {
            stream.close();
        }
    } else if constexpr (std::is_same_v<stream_t, beast::ssl_stream<asio::ip::tcp::socket>>) {
        if (stream.next_layer().is_open()) {
            stream.next_layer().close();
        }
    }
}

template<typename stream_t>
void HttpRdfSession<stream_t>::run(std::unique_ptr<HttpRdfSession> obj)
{
    HttpResponseBuffer response_buffer(obj->stream);

    std::ostream response_ostream(&response_buffer);

    // Without this line ConnectionException won't be caught properly
    response_ostream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    auto request_type = SPARQL::RequestParser::get_request_type(obj->request.target());
    if (request_type == Protocol::RequestType::INVALID) {
        return send_not_found(response_ostream);
    }
    if (request_type == Protocol::RequestType::HEALTH_CHECK) {
        return send_ok(response_ostream);
    }

    if (request_type == Protocol::RequestType::AUTH) {
        auto&& [user, pass] = Common::RequestParser::parse_auth(obj->request);
        auto&& [auth_token, valid_until] = obj->server.create_auth_token(user, pass);
        if (auth_token.empty()) {
            return send_unauthorized(response_ostream);
        } else {
            return send_token_authorized(response_ostream, auth_token, valid_until);
        }
    }

    auto auth_token = Common::RequestParser::get_auth(obj->request);

    if (request_type == Protocol::RequestType::CANCEL) {
        auto&& [worker_id, cancel_token] = Common::RequestParser::parse_cancel(obj->request);
        auto cancel_res = obj->server.try_cancel(worker_id, cancel_token);
        if (cancel_res) {
            return send_ok(response_ostream);
        } else {
            return send_bad_request(response_ostream, "Not Canceled");
        }
    }

    auto&& [query, response_type] = SPARQL::RequestParser::parse_query(obj->request);

    if (!obj->server.authorize(request_type, auth_token)) {
        return send_unauthorized(response_ostream);
    }

    if (request_type == Protocol::RequestType::UPDATE) {
        obj->execute_update_query(query, response_ostream);
    } else /* (request_type == Protocol::RequestType::QUERY) */ {
        obj->execute_readonly_query(query, response_ostream, response_type);
    }
}

template<typename stream_t>
void HttpRdfSession<stream_t>::execute_readonly_query(
    const std::string& query,
    std::ostream& os,
    SPARQL::ResponseType response_type
)
{
    // Declared here because the destruction need to be after calling execute_query_plan
    auto version_scope = buffer_manager.init_version_readonly();

    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().prepare(*version_scope, query_timeout);
    }
    logger.info() << "Query(worker " << worker << ", cancel " << get_query_ctx().cancellation_token << ")\n"
                  << trim_string(query);

    std::unique_ptr<QueryExecutor> current_physical_plan;
    try {
        auto current_logical_plan = create_readonly_logical_plan(query);
        current_physical_plan = create_readonly_physical_plan(*current_logical_plan, response_type);
    } catch (const QueryParsingException& e) {
        std::string msg = "Parsing Exception. Line " + std::to_string(e.line)
                        + ", col: " + std::to_string(e.column) + ": " + e.what();

        logger.error() << msg << "\nQuery:\n" << query;
        return send_bad_request(os, msg);
    } catch (const QueryException& e) {
        std::string msg = e.what();
        logger.error() << "Worker " << worker << ": " << msg;
        return send_bad_request(os, msg);
    } catch (const LogicException& e) {
        std::string msg = e.what();
        logger.error() << "Worker " << worker << ": " << msg;
        return send_internal_error(os, msg);
    }

    if (current_physical_plan == nullptr) {
        return;
    }

    try {
        execute_readonly_query_plan(*current_physical_plan, os, response_type);
    } catch (const ConnectionException& e) {
        logger.error() << "Connection(" << worker << ") exception: " << e.what();
    } catch (const InterruptedException& e) {
        // Handled in execute_readonly_query_plan
    } catch (const QueryExecutionException& e) {
        // Handled in execute_readonly_query_plan
    } catch (const std::exception& e) {
        logger.error() << "Unexpected(" << worker << ") exception: " << e.what();
    } catch (...) {
        logger.error() << "Unknown(" << worker << ") exception";
    }
}

template<typename stream_t>
std::unique_ptr<Op> HttpRdfSession<stream_t>::create_readonly_logical_plan(const std::string& query)
{
    const auto start_parser = chrono::system_clock::now();
    SPARQL::QueryParser parser(query);
    auto logical_plan = parser.get_query_plan({});
    parser_duration = chrono::system_clock::now() - start_parser;
    return logical_plan;
}

template<typename stream_t>
std::unique_ptr<QueryExecutor> HttpRdfSession<stream_t>::create_readonly_physical_plan(
    Op& logical_plan,
    SPARQL::ResponseType response_type
)
{
    const auto start_optimizer = chrono::system_clock::now();

    ExecutorConstructor executor_constructor(response_type);
    logical_plan.accept_visitor(executor_constructor);

    optimizer_duration = chrono::system_clock::now() - start_optimizer;
    return std::move(executor_constructor.executor);
}

template<typename stream_t>
void HttpRdfSession<stream_t>::execute_readonly_query_plan(
    QueryExecutor& physical_plan,
    std::ostream& os,
    SPARQL::ResponseType response_type
)
{
    const auto execution_start = chrono::system_clock::now();
    try {
        send_query_header(os, get_content_type(response_type));

        logger.debug([&physical_plan](std::ostream& os) {
            physical_plan.analyze(os, false);
        });

        const auto result_count = physical_plan.execute(os);
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&physical_plan](std::ostream& os) {
            physical_plan.analyze(os, true);
        });

        logger.info() << "Worker " << worker << " results: " << result_count << "\n"
                      << "Parser    : " << parser_duration.count() << " ms\n"
                      << "Optimizer : " << optimizer_duration.count() << " ms\n"
                      << "Execution : " << execution_duration.count() << " ms";
    } catch (const InterruptedException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&physical_plan](std::ostream& os) {
            physical_plan.analyze(os, true);
        });

        logger.error() << "Worker " << worker << " timed out after " << execution_duration.count() << " ms";
        throw e;
    } catch (const QueryExecutionException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;
        logger.error() << "Worker " << worker << ": " << e.what();
        throw e;
    }
}

template<typename stream_t>
void HttpRdfSession<stream_t>::execute_update_query(const std::string& query, std::ostream& os)
{
    // Mutex to allow only one write query at a time
    std::lock_guard<std::mutex> lock(server.update_execution_mutex);

    auto version_scope = buffer_manager.init_version_editable();

    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().prepare(*version_scope, query_timeout);
    }

    logger.info() << "Query(worker " << worker << ", cancel " << get_query_ctx().cancellation_token << ")\n"
                  << trim_string(query);

    std::unique_ptr<SPARQL::OpUpdate> current_logical_plan;
    try {
        current_logical_plan = create_update_logical_plan(query);
    } catch (const QueryParsingException& e) {
        std::string msg = "Parsing Exception. Line " + std::to_string(e.line)
                        + ", col: " + std::to_string(e.column) + ": " + e.what();

        logger.error() << msg << "\nQuery:\n" << query;
        return send_bad_request(os, msg);
    } catch (const QueryException& e) {
        std::string msg = e.what();
        logger.error() << "Worker " << worker << ": " << msg;
        return send_bad_request(os, msg);
    } catch (const LogicException& e) {
        std::string msg = e.what();
        logger.error() << "Worker " << worker << ": " << msg;
        return send_internal_error(os, msg);
    }

    const auto execution_start = chrono::system_clock::now();

    try {
        UpdateExecutor update_executor;
        for (auto& update : current_logical_plan->updates) {
            update->accept_visitor(update_executor);
        }
        version_scope->commited = true;
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&update_executor](std::ostream& os) {
            os << "Update Stats\n";
            update_executor.print_stats(os);
        });
    } catch (const ConnectionException& e) {
        logger.error() << "Connection(" << worker << ") exception: " << e.what();
        return;
    } catch (const InterruptedException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.error() << "Worker " << worker << " timed out after " << execution_duration.count() << " ms";
        return send_timeout(os);
    } catch (const QueryExecutionException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;
        std::string msg = e.what();
        logger.error() << msg;
        return send_internal_error(os, msg);
    }

    os << "HTTP/1.1 204 No Content\r\n\r\n";
    logger.info() << "Worker " << worker << " update finished\n"
                  << "Parser    : " << parser_duration.count() << " ms\n"
                  << "Execution : " << execution_duration.count() << " ms";
}

template<typename stream_t>
std::unique_ptr<SPARQL::OpUpdate>
    HttpRdfSession<stream_t>::create_update_logical_plan(const std::string& query)
{
    const auto start_parser = chrono::system_clock::now();
    auto logical_plan = SPARQL::UpdateParser::get_query_plan(query);
    parser_duration = chrono::system_clock::now() - start_parser;
    return logical_plan;
}

template class MDBServer::HttpRdfSession<asio::ip::tcp::socket>;
template class MDBServer::HttpRdfSession<boost::beast::ssl_stream<asio::ip::tcp::socket>>;
