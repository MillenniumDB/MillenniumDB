#include "http_quad_session.h"

#include <iomanip>

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
#include "query/update/mql/update_executor.h"

using namespace boost;
using namespace MDBServer;
using namespace MQL;
namespace beast = boost::beast;
namespace http = beast::http;

HttpQuadSession::HttpQuadSession(
    Server& server,
    stream_type&& stream,
    http::request<http::string_body>&& request,
    std::chrono::seconds query_timeout
) :
    server { server },
    stream { std::move(stream) },
    request { std::move(request) },
    query_timeout { query_timeout }
{ }

HttpQuadSession::~HttpQuadSession()
{
    if (stream.socket().is_open()) {
        stream.close();
    }
}

void HttpQuadSession::run(std::unique_ptr<HttpQuadSession> obj)
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
            auto valid_until_t = std::chrono::system_clock::to_time_t(valid_until);
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

    // After parsing the query we don't want to have a connection timeout
    obj->stream.expires_never();

    logger(Category::Info) << "\nQuery received:\n" << trim_string(query) << "\n";

    obj->execute_query(query, response_ostream, response_type);
}

std::unique_ptr<Op> HttpQuadSession::create_logical_plan(const std::string& query)
{
    const auto start_parser = std::chrono::system_clock::now();

    auto logical_plan = QueryParser::get_query_plan(query);
    parser_duration = std::chrono::system_clock::now() - start_parser;
    return logical_plan;
}

std::unique_ptr<QueryExecutor>
    HttpQuadSession::create_query_executor(Op& logical_plan, ReturnType return_type)
{
    const auto start_optimizer = std::chrono::system_clock::now();

    ExecutorConstructor executor_constructor(return_type);
    logical_plan.accept_visitor(executor_constructor);

    optimizer_duration = std::chrono::system_clock::now() - start_optimizer;
    return std::move(executor_constructor.executor);
}

void HttpQuadSession::execute_query(const std::string& query, std::ostream& os, ReturnType response_type)
{
    // Declared here because the destruction need to be after calling execute_query_plan
    auto version_scope = buffer_manager.init_version_readonly();

    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().prepare(*version_scope, query_timeout);
    }
    logger(Category::Info) << "Cancellation: " << get_query_ctx().thread_info.worker_index << ' '
                           << get_query_ctx().cancellation_token;

    std::unique_ptr<QueryExecutor> executor;
    try {
        auto logical_plan = create_logical_plan(query);
        executor = create_query_executor(*logical_plan, response_type);
    } catch (const QueryParsingException& e) {
        logger(Category::Error) << "Query Parsing Exception. Line " << e.line << ", col: " << e.column << ": "
                                << e.what();

        os << "HTTP/1.1 400 Bad Request\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << std::string(e.what());
        return;
    } catch (const QueryException& e) {
        logger(Category::Error) << "Query Exception: " << e.what();

        os << "HTTP/1.1 400 Bad Request\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << std::string(e.what());
    } catch (const LogicException& e) {
        logger(Category::Error) << "Logic Exception: " << e.what();

        os << "HTTP/1.1 500 Internal Server Error\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << std::string(e.what());
    }

    if (executor == nullptr) {
        return;
    }

    try {
        if (auto update_executor = dynamic_cast<UpdateExecutor*>(executor.get())) {
            run_write_query(*update_executor, *version_scope, os);
        } else {
            run_read_query(*executor, os, response_type);
        }
    } catch (const ConnectionException& e) {
        logger(Category::Error) << "Connection Exception: " << e.what();
    }
}

void HttpQuadSession::run_write_query(
    UpdateExecutor& executor,
    BufferManager::VersionScope& version_scope,
    std::ostream& os
)
{
    std::lock_guard<std::mutex> lock(server.update_execution_mutex);

    buffer_manager.upgrade_to_editable(version_scope);

    const auto execution_start = std::chrono::system_clock::now();
    try {
        logger.log(Category::ExecutionStats, [&executor](std::ostream& os) {
            executor.analyze(os, false);
            os << '\n';
        });

        executor.execute(os);
        execution_duration = std::chrono::system_clock::now() - execution_start;

        logger.log(Category::ExecutionStats, [&executor](std::ostream& os) {
            executor.analyze(os, true);
            os << '\n';
        });
    } catch (const ConnectionException& e) {
        logger(Category::Error) << "Connection Exception: " << e.what();
        return;
    } catch (const InterruptedException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;

        logger(Category::Info) << "Timeout thrown after "
                               << std::chrono::duration_cast<std::chrono::milliseconds>(
                                      parser_duration + execution_duration
                                  )
                                      .count()
                               << " ms";

        os << "HTTP/1.1 408 Request Timeout\r\n";
        return;
    } catch (const QueryExecutionException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        logger(Category::Error) << "Query Execution Exception: " << e.what();

        os << "HTTP/1.1 500 Internal Server Error\r\n"
              "Content-Type: text/plain\r\n"
              "\r\n"
           << std::string(e.what());
        return;
    }

    os << "HTTP/1.1 204 No Content\r\n\r\n";
    logger(Category::Info) << "Parser duration: " << parser_duration.count() << "ms\n"
                           << "Execution duration:" << execution_duration.count() << "ms";
}

void HttpQuadSession::run_read_query(QueryExecutor& executor, std::ostream& os, ReturnType return_type)
{
    const auto execution_start = std::chrono::system_clock::now();
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

        logger.log(Category::PhysicalPlan, [&executor](std::ostream& os) {
            executor.analyze(os, false);
            os << '\n';
        });

        const auto result_count = executor.execute(os);
        execution_duration = std::chrono::system_clock::now() - execution_start;

        logger.log(Category::ExecutionStats, [&executor](std::ostream& os) {
            executor.analyze(os, true);
            os << '\n';
        });

        logger(Category::Info) << "Results: " << result_count << '\n'
                               << "Parser duration:    " << parser_duration.count() << " ms\n"
                               << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                               << "Execution duration: " << execution_duration.count() << " ms";
    } catch (const InterruptedException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        logger(
            Category::Info
        ) << "Timeout thrown after "
          << std::chrono::duration_cast<std::chrono::milliseconds>(execution_duration).count() << " ms";
    } catch (const QueryExecutionException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        logger(Category::Error) << "\nQuery Execution Exception: " << e.what();
    } catch (const std::exception& e) {
        logger(Category::Error) << "Unexpected Exception: " << e.what();
    } catch (...) {
        logger(Category::Error) << "Unknown exception";
    }
}
