#include "http_quad_session.h"

#include <boost/beast/ssl.hpp>

#include "misc/logger.h"
#include "misc/trim.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/http/mql_request_parser.h"
#include "network/server/session/http/request_parser.h"
#include "network/server/session/http/response/http_response_buffer.h"
#include "network/server/session/http/response/return_codes_helper.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "query/parser/mql_query_parser.h"
#include "query/query_context.h"

using namespace boost;
using namespace MDBServer;
using namespace MQL;
namespace beast = boost::beast;
namespace http = beast::http;
namespace chrono = std::chrono;

template<typename stream_t>
HttpQuadSession<stream_t>::HttpQuadSession(
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
HttpQuadSession<stream_t>::~HttpQuadSession()
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
void HttpQuadSession<stream_t>::run(std::unique_ptr<HttpQuadSession<stream_t>> obj)
{
    HttpResponseBuffer response_buffer(obj->stream);

    std::ostream response_ostream(&response_buffer);

    // Without this line ConnectionException won't be caught properly
    response_ostream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // impossible for now to get Protocol::RequestType::INVALID
    auto request_type = RequestParser::get_request_type(obj->request.target());

    if (obj->request.method() == boost::beast::http::verb::options) {
        return send_options(response_ostream);
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

    auto&& [query, response_type] = RequestParser::parse_query(obj->request);

    if (!obj->server.authorize(request_type, auth_token)) {
        return send_unauthorized(response_ostream);
    }

    try {
        obj->execute_query(query, response_ostream, response_type);
    } catch (const ConnectionException& e) {
        logger.error() << "Connection(" << obj->worker << ") exception: " << e.what();
    } catch (const std::exception& e) {
        logger.error() << "Unexpected(" << obj->worker << ") exception: " << e.what();
    } catch (...) {
        logger.error() << "Unknown(" << obj->worker << ") exception";
    }
}

template<typename stream_t>
std::unique_ptr<QueryExecutor>
    HttpQuadSession<stream_t>::create_query_executor(Op& logical_plan, ReturnType return_type)
{
    const auto start_optimizer = chrono::system_clock::now();

    ExecutorConstructor executor_constructor(return_type);
    logical_plan.accept_visitor(executor_constructor);

    optimizer_duration = chrono::system_clock::now() - start_optimizer;
    return std::move(executor_constructor.executor);
}

template<typename stream_t>
void HttpQuadSession<stream_t>::execute_query(
    const std::string& query,
    std::ostream& os,
    ReturnType response_type
)
{
    try {
        const auto start_parser = chrono::system_clock::now();
        QueryParser parser(query);
        parser_duration = chrono::system_clock::now() - start_parser;

        if (parser.is_update()) {
            run_write_query(query, parser, os);
        } else {
            run_read_query(query, parser, os, response_type);
        }
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
}

template<typename stream_t>
void HttpQuadSession<stream_t>::run_write_query(
    const std::string& query,
    MQL::QueryParser& parser,
    std::ostream& os
)
{
    std::lock_guard<std::mutex> lock(server.update_execution_mutex);

    auto version_scope = buffer_manager.init_version_editable();
    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().prepare(*version_scope, query_timeout);
    }

    logger.info() << "Query(worker " << worker << ", cancel " << get_query_ctx().cancellation_token << ")\n"
                  << trim_string(query);

    const auto start_parser = chrono::system_clock::now();
    auto logical_plan = parser.get_query_plan({});
    parser_duration = chrono::system_clock::now() - start_parser;

    const auto optimizer_start = std::chrono::system_clock::now();
    auto executor = create_query_executor(*logical_plan, MQL::ReturnType::TSV);
    optimizer_duration = chrono::system_clock::now() - optimizer_start;

    const auto execution_start = chrono::system_clock::now();
    try {
        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, false);
        });

        executor->execute(os);
        version_scope->commited = true;
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, true);
        });

        os << "HTTP/1.1 204 No Content\r\n\r\n";
        logger.info() << "Worker " << worker << " update finished\n"
                      << "Parser    : " << parser_duration.count() << " ms\n"
                      << "Optimizer : " << optimizer_duration.count() << " ms\n"
                      << "Execution : " << execution_duration.count() << " ms";
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
}

template<typename stream_t>
void HttpQuadSession<stream_t>::run_read_query(
    const std::string& query,
    MQL::QueryParser& parser,
    std::ostream& os,
    ReturnType return_type
)
{
    auto version_scope = buffer_manager.init_version_readonly();

    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().prepare(*version_scope, query_timeout);
    }

    logger.info() << "Query(worker " << worker << ", cancel " << get_query_ctx().cancellation_token << ")\n"
                  << trim_string(query);

    const auto start_parser = chrono::system_clock::now();
    auto logical_plan = parser.get_query_plan({});
    parser_duration = chrono::system_clock::now() - start_parser;

    auto executor = create_query_executor(*logical_plan, return_type);

    const auto execution_start = chrono::system_clock::now();
    try {
        send_query_header(os, get_content_type(return_type));

        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, false);
        });

        const auto result_count = executor->execute(os);
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, true);
        });

        logger.info() << "Worker " << worker << " results: " << result_count << "\n"
                      << "Parser    : " << parser_duration.count() << " ms\n"
                      << "Optimizer : " << optimizer_duration.count() << " ms\n"
                      << "Execution : " << execution_duration.count() << " ms";
    } catch (const InterruptedException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, true);
        });

        logger.error() << "Worker " << worker << " timed out after " << execution_duration.count() << " ms";
    } catch (const QueryExecutionException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;
        logger.error() << "Worker " << worker << ": " << e.what();
    }
}

template class MDBServer::HttpQuadSession<asio::ip::tcp::socket>;
template class MDBServer::HttpQuadSession<boost::beast::ssl_stream<asio::ip::tcp::socket>>;
