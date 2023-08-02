#include "session.h"

#include <memory>
#include <mutex>

#include <boost/beast/core/bind_handler.hpp>

#include "graph_models/quad_model/quad_model.h"
#include "network/exceptions.h"
#include "network/mql/server.h"
#include "network/mql/tcp_buffer.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "query/parser/grammar/error_listener.h"
#include "query/parser/mql_query_parser.h"
#include "storage/tmp_manager.h"


using namespace boost;
using namespace MQL;
using namespace std::chrono;

void Session::run() {
    asio::dispatch(
        socket.get_executor(),
        beast::bind_front_handler(&Session::do_read, shared_from_this())
    );
}


void Session::do_read() {
    unsigned char query_size_b[CommunicationProtocol::BYTES_FOR_QUERY_LENGTH];
    std::string query;

    try {
        boost::asio::read(socket, boost::asio::buffer(query_size_b, CommunicationProtocol::BYTES_FOR_QUERY_LENGTH));

        int query_size = 0;
        for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
            query_size += query_size_b[i] << offset;
        }
        query.resize(query_size);
        boost::asio::read(socket, boost::asio::buffer(query.data(), query_size));
        std::cout << "---------------------------------------\n";
        std::cout << " Query received:\n";
        std::cout << "---------------------------------------\n";
        std::cout << query << "\n";
        std::cout << "---------------------------------------" << std::endl;
    } catch (...) {
        std::cerr << "Error receiving the query." << std::endl;
        return;
    }

    TcpBuffer tcp_buffer(socket);
    std::ostream os(&tcp_buffer);

    // without this line ConnectionException won't be caught properly
    os.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    tmp_manager.reset();
    get_query_ctx().reset();
    std::unique_ptr<QueryExecutor> physical_plan;
    try {
        std::shared_lock s_lock(server.execution_mutex);
        auto logical_plan = create_logical_plan(query);
        if (logical_plan->read_only()) {
            physical_plan = create_readonly_physical_plan(*logical_plan);
        } else {
            std::unique_lock u_lock(server.execution_mutex);
            quad_model.exec_inserts(*reinterpret_cast<OpInsert*>(logical_plan.get()));
        }
    }
    catch (const QueryException& e) {
        os << "---------------------------------------\n";
        os << "Query Exception: " << e.what() << "\n";
        os << "---------------------------------------" << std::endl;
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::query_error);
    }
    catch (const LogicException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Logic Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
    }

    if (physical_plan == nullptr) {
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
        return;
    }

    try {
        execute_plan(*physical_plan, os);

        std::cout << "Parser duration:" << parser_duration.count() << "ms.\n";
        std::cout << "Optimizer duration:" << optimizer_duration.count() << "ms.\n";
        std::cout << "Execution duration:" << execution_duration.count() << "ms.\n";
        std::cout << "---------------------------------------" << std::endl;
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::success);
    }
    catch (const ConnectionException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Connection Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
    }
    catch (const InterruptedException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Timeout thrown after "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(execution_duration).count()
                  << " milliseconds.\n";
        std::cout << "---------------------------------------" << std::endl;
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::timeout);
    }
    catch (const QueryExecutionException& e) {
        std::cout << "---------------------------------------\n";
        std::cout << "Query Execution Exception: " << e.what() << "\n";
        std::cout << "---------------------------------------" << std::endl;
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::unexpected_error);
    }
}


std::unique_ptr<Op> Session::create_logical_plan(const std::string& query) {
    auto start_parser = std::chrono::system_clock::now();
    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().thread_info.interruption_requested = false; // used in query optimization
        get_query_ctx().thread_info.finished = false;
        get_query_ctx().thread_info.time_start = start_parser;
        get_query_ctx().thread_info.timeout = start_parser + timeout;
    }
    antlr4::MyErrorListener error_listener;
    auto logical_plan = MQL::QueryParser::get_query_plan(query, &error_listener);
    parser_duration = std::chrono::system_clock::now() - start_parser;
    return logical_plan;
}


void Session::execute_plan(QueryExecutor& physical_plan, std::ostream& os) {
    auto execution_start = std::chrono::system_clock::now();
    try {
        std::cout << "\nPlan to execute:\n";
        physical_plan.analyze(std::cout);
        std::cout.flush();

        auto result_count = physical_plan.execute(os);
        execution_duration = std::chrono::system_clock::now() - execution_start;

        std::cout << "\nPlan executed:\n";
        physical_plan.analyze(std::cout);

        std::cout << "\nResults: " << result_count << std::endl;
        get_query_ctx().thread_info.finished = true;
    }
    catch (const InterruptedException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        get_query_ctx().thread_info.finished = true;
        throw e;
    }
    catch (const QueryExecutionException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        get_query_ctx().thread_info.finished = true;
        throw e;
    }
}


std::unique_ptr<QueryExecutor> Session::create_readonly_physical_plan(Op& logical_plan) {
    auto start_optimizer = std::chrono::system_clock::now();

    MQL::ExecutorConstructor query_optimizer;
    logical_plan.accept_visitor(query_optimizer);

    optimizer_duration = std::chrono::system_clock::now() - start_optimizer;
    return std::move(query_optimizer.executor);
}

