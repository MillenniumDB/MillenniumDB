#include "session.h"

#include <memory>
#include <mutex>

#include "graph_models/rdf_model/rdf_model.h"
#include "network/sparql/http_buffer.h"
#include "network/sparql/request_handler.h"
#include "network/sparql/server.h"
#include "query/optimizer/rdf_model/executor_constructor.h"
#include "query/parser/grammar/error_listener.h"
#include "query/parser/sparql_query_parser.h"
#include "storage/tmp_manager.h"

using namespace boost;
using namespace SPARQL;
using namespace std::chrono;

void Session::run() {
    asio::dispatch(
        stream.get_executor(),
        beast::bind_front_handler(&Session::do_read, shared_from_this())
    );
}


void Session::do_read() {
    // Make the request empty before reading, otherwise the operation behavior is undefined.
    req = {};

    // Set the timeout.
    stream.expires_after(timeout); // TODO: not query timeout, should be different?

    // Read a request
    beast::http::async_read(stream,
                            buffer,
                            req,
                            beast::bind_front_handler(&Session::on_read, shared_from_this()));
}


std::unique_ptr<Op> Session::create_logical_plan(
    const std::string& query,
    std::chrono::duration<float, std::milli>* parser_duration)
{
    auto start_parser = std::chrono::system_clock::now();
    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().thread_info.interruption_requested = false; // used in query optimization
        get_query_ctx().thread_info.finished = false;
        get_query_ctx().thread_info.time_start = start_parser;
        get_query_ctx().thread_info.timeout = start_parser + timeout;
    }
    antlr4::MyErrorListener error_listener;
    auto logical_plan = SPARQL::QueryParser::get_query_plan(query, &error_listener);
    *parser_duration = std::chrono::system_clock::now() - start_parser;
    return logical_plan;
}


void Session::execute_plan(QueryExecutor& physical_plan,
                           std::ostream& os,
                           std::chrono::duration<float, std::milli>* execution_duration,
                           ResponseType response_type)
{
    auto execution_start = std::chrono::system_clock::now();
    try {
        os << "HTTP/1.1 200 OK\r\n"
           << "Server: MillenniumDB\r\n";

        switch (response_type) {
            case SPARQL::ResponseType::CSV:
                os << "Content-Type: text/csv; charset=utf-8\r\n";
                break;
            case SPARQL::ResponseType::TSV:
                os << "Content-Type: text/tab-separated-values; charset=utf-8\r\n";
                break;
            case SPARQL::ResponseType::JSON:
                os << "Content-Type: application/json; charset=utf-8\r\n";
                break;
            case SPARQL::ResponseType::XML:
                os << "Content-Type: application/sparql-results+xml; charset=utf-8\r\n";
                break;
            case SPARQL::ResponseType::TURTLE:
                os << "Content-Type: application/turtle; charset=utf-8\r\n";
                break;
            default: throw NotSupportedException("RESPONSE TYPE: " + SPARQL::response_type_to_string(response_type));
        }
        os << "Access-Control-Allow-Origin: *\r\n"
           << "Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept, Authorization\r\n"
           << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
           << "\r\n";

        std::cout << "\nPlan to execute:\n";
        physical_plan.analyze(std::cout);
        std::cout.flush();

        auto result_count = physical_plan.execute(os);
        *execution_duration = std::chrono::system_clock::now() - execution_start;

        std::cout << "\nPlan executed:\n";
        physical_plan.analyze(std::cout);

        std::cout << "\nResults: " << result_count << std::endl;
        get_query_ctx().thread_info.finished = true;
    }
    catch (const InterruptedException& e) {
        *execution_duration = std::chrono::system_clock::now() - execution_start;
        std::cout << "---------------------------------------\n";
        std::cout << "Timeout thrown after "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(*execution_duration).count()
                  << " milliseconds.\n";
        std::cout << "---------------------------------------" << std::endl;
        get_query_ctx().thread_info.finished = true;
        throw e;
    }
    catch (const QueryExecutionException& e) {
        *execution_duration = std::chrono::system_clock::now() - execution_start;
        get_query_ctx().thread_info.finished = true;
        std::cout << "---------------------------------------\n";
        std::cout << "Query Execution Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
        throw e;
    }
}


std::unique_ptr<QueryExecutor> Session::create_readonly_physical_plan(
    Op& logical_plan,
    std::chrono::duration<float, std::milli>* optimizer_duration,
    ResponseType response_type)
{
    // std::shared_lock s_lock(execution_mutex); // TODO:
    auto start_optimizer = std::chrono::system_clock::now();

    auto query_optimizer = SPARQL::ExecutorConstructor(response_type);
    logical_plan.accept_visitor(query_optimizer);

    *optimizer_duration = std::chrono::system_clock::now() - start_optimizer;
    return std::move(query_optimizer.executor);
}


void Session::on_read(beast::error_code ec, std::size_t /*bytes_transferred*/) {
    // This means they closed the connection
    if (ec == beast::http::error::end_of_stream) {
        stream.socket().shutdown(asio::ip::tcp::socket::shutdown_send, ec);
        return;
    }

    if (ec)
        return fail(ec, "read");

    HttpBuffer http_buffer(stream.socket());
    std::ostream os(&http_buffer);

    if (req.target().rfind("/sparql", 0) == std::string::npos) {
         os << "HTTP/1.1 404 Not Found\r\n"
            << "\r\n";
        http_buffer.finish();
        return;
    }

    auto [query, response_type] = RequestHandler::parse_request(req);

    // without this line ConnectionException won't be caught properly
    os.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::chrono::duration<float, std::milli> parser_duration;
    std::chrono::duration<float, std::milli> optimizer_duration;
    std::chrono::duration<float, std::milli> execution_duration;

    tmp_manager.reset();
    get_query_ctx().reset();
    std::unique_ptr<QueryExecutor> physical_plan;
    try {
        auto logical_plan = create_logical_plan(query, &parser_duration);
        if (logical_plan->read_only()) {
            physical_plan = create_readonly_physical_plan(*logical_plan, &optimizer_duration, response_type);
        } else {
            // TODO: execute update when its supported
        }
    }
    catch (const QueryException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Query Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
        os << "HTTP/1.1 400 Bad Request\r\n"
           << "Content-Type: text/plain\r\n"
           << "\r\n"
           << std::string(e.what());
    }
    catch (const LogicException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Logic Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
        os << "HTTP/1.1 500 Internal Server Error\r\n"
           << "Content-Type: text/plain\r\n"
           << "\r\n"
           << std::string(e.what());
    }

    if (physical_plan == nullptr) {
        http_buffer.finish();
        return;
    }

    try {
        execute_plan(*physical_plan, os, &execution_duration, response_type);

        std::cout << "Parser duration:" << parser_duration.count() << "ms.\n";
        std::cout << "Optimizer duration:" << optimizer_duration.count() << "ms.\n";
        std::cout << "Execution duration:" << execution_duration.count() << "ms.\n";
        std::cout << "---------------------------------------" << std::endl;
    }
    catch (const ConnectionException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Connection Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
    }
    catch (const InterruptedException& e) {
        // Handled in execute_plan
    }
    catch (const QueryExecutionException& e) {
        // Handled in execute_plan
    }
    http_buffer.finish();
}
