#include "http_quad_session.h"

#include <iomanip>

#include <boost/beast/ssl.hpp>

#include "misc/logger.h"
#include "misc/trim.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/http/mql_request_parser.h"
#include "network/server/session/http/request_parser.h"
#include "network/server/session/http/response/http_response_buffer.h"
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
{ }

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

    auto request_type = RequestParser::get_request_type(obj->request.target());
    if (request_type == Protocol::RequestType::INVALID) {
        response_ostream << "HTTP/1.1 404 Not Found\r\n\r\n";
        return;
    }

    if (request_type == Protocol::RequestType::AUTH) {
        auto&& [user, pass] = Common::RequestParser::parse_auth(obj->request);
        auto&& [auth_token, valid_until] = obj->server.create_auth_token(user, pass);
        if (auth_token.empty()) {
            response_ostream << "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Bearer\r\n\r\n";
        } else {
            auto valid_until_t = chrono::system_clock::to_time_t(valid_until);
            response_ostream << "HTTP/1.1 200 OK\r\n"
                                "Content-Type: application/json; charset=utf-8\r\n"
                                "{\"token\":\""
                             << auth_token << "\",\"valid_until\":\""
                             << std::put_time(std::localtime(&valid_until_t), "%Y-%m-%d %T") << "\"}";
        }
        return;
    }

    auto auth_token = Common::RequestParser::get_auth(obj->request);

    if (request_type == Protocol::RequestType::CANCEL) {
        auto&& [worker_id, cancel_token] = Common::RequestParser::parse_cancel(obj->request);
        auto cancel_res = obj->server.try_cancel(worker_id, cancel_token);
        if (cancel_res) {
            response_ostream << "HTTP/1.1 200 OK\r\n\r\n";
        } else {
            response_ostream << "HTTP/1.1 400 Bad Request\r\n\r\n";
        }
        return;
    }

    auto&& [query, response_type] = RequestParser::parse_query(obj->request);

    if (!obj->server.authorize(request_type, auth_token)) {
        response_ostream << "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Bearer\r\n\r\n";
        return;
    }

    // TODO: print worker thread in all errors?
    try {
        obj->execute_query(query, response_ostream, response_type);
    } catch (const ConnectionException& e) {
        logger.error() << "Connection Exception: " << e.what();
    } catch (const std::exception& e) {
        logger.error() << "Unexpected Exception: " << e.what();
    } catch (...) {
        logger.error() << "Unknown exception";
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
    logger.info() << "Query received (worker " << get_query_ctx().thread_info.worker_index << ")\n"
                  << trim_string(query);

    try {
        const auto start_parser = chrono::system_clock::now();
        QueryParser parser(query);
        parser_duration = chrono::system_clock::now() - start_parser;

        if (parser.is_update()) {
            run_write_query(parser, os);
        } else {
            run_read_query(parser, os, response_type);
        }
    } catch (const QueryParsingException& e) {
        logger.error() << "Query Parsing Exception. Line " << e.line << ", col: " << e.column << ": "
                       << e.what();

        os << "HTTP/1.1 400 Bad Request\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << e.what();
    } catch (const QueryException& e) {
        logger.error() << "Query Exception: " << e.what();

        os << "HTTP/1.1 400 Bad Request\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << e.what();
    } catch (const LogicException& e) {
        logger.error() << "Logic Exception: " << e.what();

        os << "HTTP/1.1 500 Internal Server Error\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << e.what();
    }
}

template<typename stream_t>
void HttpQuadSession<stream_t>::run_write_query(MQL::QueryParser& parser, std::ostream& os)
{
    std::lock_guard<std::mutex> lock(server.update_execution_mutex);

    auto version_scope = buffer_manager.init_version_editable();
    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().prepare(*version_scope, query_timeout);
    }

    logger.debug() << "Cancel: `" << get_query_ctx().cancellation_token << "` (worker "
                   << get_query_ctx().thread_info.worker_index << ')';

    const auto start_parser = chrono::system_clock::now();
    auto logical_plan = parser.get_query_plan({});
    parser_duration += chrono::system_clock::now() - start_parser;

    auto executor = create_query_executor(*logical_plan, MQL::ReturnType::TSV);

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
        logger.info() << "Parser duration:    " << parser_duration.count() << "ms\n"
                      << "Execution duration: " << execution_duration.count() << "ms";
    } catch (const InterruptedException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.error() << "Worker " << get_query_ctx().thread_info.worker_index << " timed out after "
                       << execution_duration.count() << " ms";

        os << "HTTP/1.1 408 Request Timeout\r\n";
    } catch (const QueryExecutionException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;
        logger.error() << e.what();

        os << "HTTP/1.1 500 Internal Server Error\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << e.what();
    }
}

template<typename stream_t>
void HttpQuadSession<stream_t>::run_read_query(
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

    logger.debug() << "Cancel: `" << get_query_ctx().cancellation_token << "` (worker "
                   << get_query_ctx().thread_info.worker_index << ')';

    const auto start_parser = chrono::system_clock::now();
    auto logical_plan = parser.get_query_plan({});
    parser_duration += chrono::system_clock::now() - start_parser;

    auto executor = create_query_executor(*logical_plan, return_type);

    const auto execution_start = chrono::system_clock::now();
    try {
        os << "HTTP/1.1 200 OK\r\n"
              "Server: MillenniumDB\r\n";

        switch (return_type) {
        case ReturnType::CSV:
            os << "Content-Type: text/csv; charset=utf-8\r\n";
            break;
        case ReturnType::TSV:
            os << "Content-Type: text/tab-separated-values; charset=utf-8\r\n";
            break;
        default:
            throw LogicException(
                "Response type not implemented: " + std::to_string(static_cast<int>(return_type))
            );
        }
        os << "Access-Control-Allow-Origin: *\r\n"
              "Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept, "
              "Authorization\r\n"
              "Access-Control-Allow-Methods: GET, POST\r\n"
              "\r\n";

        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, false);
        });

        const auto result_count = executor->execute(os);
        execution_duration = chrono::system_clock::now() - execution_start;

        logger.debug([&executor](std::ostream& os) {
            executor->analyze(os, true);
        });

        logger.info() << "Worker             : " << get_query_ctx().thread_info.worker_index << "\n"
                      << "Results            : " << result_count << "\n"
                      << "Parser duration    : " << parser_duration.count() << " ms\n"
                      << "Optimizer duration : " << optimizer_duration.count() << " ms\n"
                      << "Execution duration : " << execution_duration.count() << " ms";
    } catch (const InterruptedException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;
        logger.error() << "Worker " << get_query_ctx().thread_info.worker_index << " timed out after "
                       << execution_duration.count() << " ms";
    } catch (const QueryExecutionException& e) {
        execution_duration = chrono::system_clock::now() - execution_start;
        logger.error() << e.what();
    }
}

template class MDBServer::HttpQuadSession<asio::ip::tcp::socket>;
template class MDBServer::HttpQuadSession<boost::beast::ssl_stream<asio::ip::tcp::socket>>;
