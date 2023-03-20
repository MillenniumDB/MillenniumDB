/*
 * server is a executable that listens for tcp conections asking for queries,
 * and it send the results to the client.
 *
 * There are 5 methods:
 *
 * - main: parses the program options (e.g: buffer size, port, database folder).
 *   Then it creates the proper GraphModel and calls the method `server`.
 *
 * - server: first it starts the server, then it launches a new thread that executes
 *   the method `execute_timeouts` and then it waits for new TCP connections in an
 *   infinite loop.
 *   When a connection is established, it generates a ThreadKey and put it into
 *   `running_threads` and `running_threads_queue`. Then calls the `session` method
 *   in a different thread.
 *
 * - session: read a query from the client, it parses the query, getting a logical
 *   plan and then a physical plan. Then it enumerates all results from the phisical plan,
 *   sending them to the client via TcpBuffer. It removes the ThreadKey from
 *   `running_threads` before ending.
 *
 * - execute_timeouts: it checks periodically the head of `running_threads_queue` to see
 *   if timeout should be thrown. If a timeout needs to be thrown, it will mark a boolean
 *   attribute and the physical plan is the responsable to check that attribute.
 */

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <memory>
#include <mutex>
#include <ostream>
#include <random>
#include <thread>
#include <unordered_map>

#include <boost/asio.hpp>

#include "base/binding/binding_iter.h"
#include "base/exceptions.h"
#include "base/thread/thread_info.h"
#include "network/tcp_buffer.h"
#include "parser/query/grammar/error_listener.h"
#include "parser/query/sparql_query_parser.h"
#include "query_optimizer/rdf_model/rdf_model.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace std;
using namespace SPARQL;
using boost::asio::ip::tcp;

std::queue<std::shared_ptr<ThreadInfo>> running_threads_queue;
std::mutex running_threads_queue_mutex;

bool shutdown_server = false;

void execute_query(ostream& os, unique_ptr<BindingIter> physical_plan) {
    uint64_t result_count = 0;
    auto execution_start = chrono::system_clock::now();
    try {
        physical_plan->begin(os);

        // get all results
        while (physical_plan->next()) {
            result_count++;
        }

        chrono::duration<float, std::milli> execution_duration = chrono::system_clock::now() - execution_start;

        // print execution stats in server console
        cout << "\nPlan Executed:\n";
        physical_plan->analyze(cout);
        cout << "\nResults: " << result_count << "\n";
        cout.flush();

        // write execution stats in output stream
        os << "---------------------------------------\n";
        os << "Found " << result_count << " results.\n";
        os << "Execution time: " << execution_duration.count() << " ms.\n";
    }
    catch (const InterruptedException& e) {
        cerr << "QueryInterrupted" << endl;
        os << "---------------------------------------\n";
        os << "Timeout thrown after "
           << chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - execution_start).count()
           << " milliseconds.\n";
        os << "Found " << result_count << " results before timeout.\n";
        throw e; // throw again to print parser and optimizer duration; and set error status in tcp_buffer
    }
}


void session(chrono::seconds timeout_duration, tcp::socket sock) {
    try {
        unsigned char query_size_b[CommunicationProtocol::BYTES_FOR_QUERY_LENGTH];
        boost::asio::read(sock, boost::asio::buffer(query_size_b, CommunicationProtocol::BYTES_FOR_QUERY_LENGTH));

        int query_size = 0;
        for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
            query_size += query_size_b[i] << offset;
        }
        std::string query;
        query.resize(query_size);
        boost::asio::read(sock, boost::asio::buffer(query.data(), query_size));
        cout << "---------------------------------------\n";
        cout << " Query received:\n";
        cout << "---------------------------------------\n";
        cout << query << "\n";
        cout << "---------------------------------------\n";

        TcpBuffer tcp_buffer = TcpBuffer(sock);
        std::ostream os(&tcp_buffer);

        // without this line ConnectionException won't be caught properly
        os.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        chrono::duration<float, std::milli> parser_duration;
        chrono::duration<float, std::milli> optimizer_duration;

        unique_ptr<BindingIter> physical_plan;

        try {
            antlr4::MyErrorListener error_listener;
            auto start_parser = chrono::system_clock::now();
            auto logical_plan = QueryParser::get_query_plan(query, &error_listener);
            parser_duration = chrono::system_clock::now() - start_parser;

            auto thread_info = std::make_shared<ThreadInfo>();
            auto start_optimizer = chrono::system_clock::now();
            physical_plan = rdf_model.exec(*logical_plan, thread_info.get());
            optimizer_duration = chrono::system_clock::now() - start_optimizer;

            {
                // TODO: stablish private buffer pos? what happens if there is no available private buffer?
                thread_info->timeout = chrono::system_clock::now() + timeout_duration;
                std::lock_guard<std::mutex> guard(running_threads_queue_mutex);
                running_threads_queue.push(thread_info);
            }

            execute_query(os, move(physical_plan));
            os << "Optimizer time: " << optimizer_duration.count() << " ms.\n";
            thread_info->finished = true;
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::success);
        }
        catch (const QueryException& e) {
            os << "---------------------------------------\n";
            os << "Query Exception: " << e.what() << "\n";
            os << "---------------------------------------" << endl;
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::query_error);
        }
        catch (const LogicException& e) {
            os << "---------------------------------------\n";
            os << "Logic Exception: " << e.what() << "\n";
            os << "---------------------------------------" << endl;
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
        }
        catch (const InterruptedException& e) {
            os << "Query Parser time: " << parser_duration.count() << " ms. "
               << "Optimizer time: " << optimizer_duration.count() << "ms." << endl;
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::timeout);
        }
    }
    catch (const ConnectionException& e) {
        cerr << "Lost connection with client: " << e.what() << endl;
    }
    catch (const LogicException& e) {
        cerr << "LogicException:" << e.what() << endl;
    }
    catch (...) {
        cerr << "Unknown exception." << endl;
    }
}


void execute_timeouts(chrono::seconds timeout_duration) {
    chrono::nanoseconds sleep_time;
    while (!shutdown_server) {
        bool should_sleep = false;
        {
            std::lock_guard<std::mutex> guard(running_threads_queue_mutex);
            if (running_threads_queue.empty()) {
                should_sleep = true;
                sleep_time = timeout_duration;
            } else {
                auto thread_info = running_threads_queue.front();
                auto now = chrono::system_clock::now();

                if (thread_info->timeout <= now || thread_info->finished) {
                    thread_info->interruption_requested = true; // if thread finished this don't matter
                    running_threads_queue.pop();
                } else {
                    // should't sleep here because the mutex is locked
                    sleep_time = thread_info->timeout - now;
                    should_sleep = true;
                }
            }
        }
        if (should_sleep) {
            this_thread::sleep_for(sleep_time);
        }
    }
}


void interruption_handler(const boost::system::error_code& error, int /*signal_number*/) {
    if (!error) {
        shutdown_server = true;
    }
}


void server(unsigned short port, chrono::seconds timeout_duration) {
    boost::asio::io_context io_context;

    std::thread(execute_timeouts, timeout_duration).detach();

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    cout << "Server running on port " << port << endl;
    cout << "To terminate press CTRL-C" << endl;

    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait(interruption_handler);

    while (!shutdown_server) {
        try {
            tcp::socket sock(io_context);
            acceptor.accept(sock);
            std::thread(session, timeout_duration, move(sock)).detach();
        }
        catch (boost::system::system_error& e) {
            // cout << "boost::system::system_error" << endl;
            break;
        }
    }
}


int main(int argc, char **argv) {
    int seconds_timeout;
    int port;
    int shared_buffer_size;
    int private_buffer_size;
    int max_threads;
    string db_folder;
    try {
        // Parse arguments
        cxxopts::Options options("server", "MillenniumDB server");
        options.add_options()
            ("h,help", "Print usage")
            ("d,db-folder", "set database folder path", cxxopts::value<string>(db_folder))
            ("p,port", "database server port",
                cxxopts::value<int>(port)->default_value(std::to_string(CommunicationProtocol::DEFAULT_PORT)))
            ("t,timeout", "timeout (in seconds)", cxxopts::value<int>(seconds_timeout)->default_value("60"))
            ("b,buffer-size", "set shared buffer pool size",
                cxxopts::value<int>(shared_buffer_size)->default_value(std::to_string(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE)))
            ("private-buffer-size", "set private buffer pool size for each thread",
                cxxopts::value<int>(private_buffer_size)->default_value(std::to_string(BufferManager::DEFAULT_PRIVATE_BUFFER_POOL_SIZE)))
            ("max-threads", "set max threads", cxxopts::value<int>(max_threads)->default_value("8"))
        ;
        options.positional_help("db-folder");
        options.parse_positional({"db-folder"});

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        // Validate params
        if (db_folder.empty()) {
            cerr << "Must specify a db-folder.\n";
            return 1;
        }

        if (port < 0) {
            cerr << "Port cannot be a negative number.\n";
            return 1;
        }

        if (seconds_timeout < 0) {
            cerr << "Timeout cannot be a negative number.\n";
            return 1;
        }

        if (shared_buffer_size < 0) {
            cerr << "Buffer size cannot be a negative number.\n";
            return 1;
        }

        if (private_buffer_size < 0) {
            cerr << "Private buffer size cannot be a negative number.\n";
            return 1;
        }

        if (!Filesystem::exists(db_folder)) {
            cerr << "Database folder does not exists.\n";
            return 1;
        } else if (!Filesystem::is_directory(db_folder)) {
            cerr << "Database folder is not a directory.\n";
            return 1;
        }

        // Initialize model
        auto model_destroyer = RdfModel::init(db_folder, shared_buffer_size, private_buffer_size, max_threads);

        cout << "Initializing server...\n";
        rdf_model.catalog().print();

        server(port, std::chrono::seconds(seconds_timeout));
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        cerr << "Exception of unknown type!\n";
        return 1;
    }
    return 0;
}
