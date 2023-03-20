/******************************************************************************
Server is the executable that listens for queries from a client and sends back
the results.

The communication is via TCP. The server expects two messages from the client,
first message contains the query length, the other contains the query.
The response from the server will be in blocks of fixed size. The last block
is marked so the client knows when to stop listening.

Server can execute multiple read-only queries at the same time, but queries
that modify the database are executed isolated. The maximum of threads
executing at the same time is a parameter received when the server starts.
Each query is executed in its own thread and we have two additional threads,
one that listens for new TCP connections and launches a new query thread.
The other thread is periodically marking queries as timed out.
Marking a query as timed out won't stop it immediately, but long operations
should check regularly if the query timed out, it is their responsibility to
stop the execution throwing a timeout exception.
******************************************************************************/

#include <chrono>
#include <fstream>
#include <queue>
#include <memory>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

#include <boost/asio.hpp>

#include "base/binding/binding_iter.h"
#include "base/exceptions.h"
#include "base/thread/thread_info.h"
#include "network/tcp_buffer.h"
#include "parser/query/grammar/error_listener.h"
#include "parser/query/mdb_query_parser.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/buffer_manager.h"
#include "storage/filesystem.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace MDB;
using namespace std;
using boost::asio::ip::tcp;

// Threads must be ordered by timeout inside the queue
std::queue<std::shared_ptr<ThreadInfo>> running_threads_queue;
std::mutex running_threads_queue_mutex;

std::shared_mutex execution_mutex;

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
        throw; // throw again to print parser and optimizer duration; and set error status in tcp_buffer
    }
}


// Receives the query from the client, parses it into a physical plan
void session(chrono::seconds timeout_duration, tcp::socket sock) {
    try {
        unsigned char query_size_b[CommunicationProtocol::BYTES_FOR_QUERY_LENGTH];
        boost::asio::read(sock, boost::asio::buffer(query_size_b, CommunicationProtocol::BYTES_FOR_QUERY_LENGTH));

        int query_size = 0;
        for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
            query_size += query_size_b[i] << offset;
        }
        string query;
        query.resize(query_size);
        boost::asio::read(sock, boost::asio::buffer(query.data(), query_size));
        cout << "---------------------------------------\n";
        cout << " Query received:\n";
        cout << "---------------------------------------\n";
        cout << query << "\n";
        cout << "---------------------------------------" << endl;

        TcpBuffer tcp_buffer = TcpBuffer(sock);
        ostream os(&tcp_buffer);

        // without this line ConnectionException won't be caught properly
        os.exceptions(ifstream::failbit | ifstream::badbit);

        chrono::duration<float, std::milli> parser_duration;
        chrono::duration<float, std::milli> optimizer_duration;
        try {
            // Create logical plan
            antlr4::MyErrorListener error_listener;
            auto start_parser = chrono::system_clock::now();
            auto logical_plan = QueryParser::get_query_plan(query, &error_listener);
            parser_duration = chrono::system_clock::now() - start_parser;

            if (logical_plan->read_only()) {
                std::shared_lock s_lock(execution_mutex);

                auto thread_info = std::make_shared<ThreadInfo>();

                auto start_optimizer = chrono::system_clock::now();
                auto physical_plan = quad_model.exec(*logical_plan, thread_info.get());
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
            } else {
                std::unique_lock u_lock(execution_mutex);
                quad_model.exec_inserts(*reinterpret_cast<OpInsert*>(logical_plan.get()));
            }
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::success);
        }
        catch (const QueryException& e) {
            os << "---------------------------------------\n";
            os << "Query Exception: " << e.what() << "\n";
            os << "---------------------------------------\n";
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::query_error);
        }
        catch (const LogicException& e) {
            os << "---------------------------------------\n";
            os << "Logic Exception: " << e.what() << "\n";
            os << "---------------------------------------\n";
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
        }
        catch (const InterruptedException& e) {
            os << "Query Parser time: " << parser_duration.count() << " ms. "
               << "Optimizer time: " << optimizer_duration.count() << "ms.\n";
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

    // Flags
    string path_mode;  // "bfs" or "dfs"
    string index_mode;  // "cache" or "naive"
    string index_type;  // "btree" or "trie"

    // Create statistics file for Trie index
    fstream stats_file;
    stats_file.open("trie_stats.csv", ios::out);
    stats_file.close();

    ios_base::sync_with_stdio(false);

    // TODO: We would like to receive shared and private buffer param in MB or GB
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
            ("path-mode", "set path mode (\"bfs\" or \"dfs\")", cxxopts::value<string>(path_mode)->default_value("bfs"))
            ("index-mode", "set index mode (\"cache\" or \"naive\")", cxxopts::value<string>(index_mode)->default_value("cache"))
            ("index-type", "set index type (\"btree\" or \"trie\")", cxxopts::value<string>(index_type)->default_value("btree"))
        ;
        options.positional_help("db-folder");
        options.parse_positional({"db-folder"});

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            cout << options.help() << endl;
            exit(0);
        }

        // Validate params
        if (db_folder.empty()) {
            cerr << "Must specify a db-folder.\n";
            return 1;
        }

        if (port <= 0) {
            cerr << "Port must be a positive number.\n";
            return 1;
        }

        if (seconds_timeout <= 0) {
            cerr << "Timeout must be a positive number.\n";
            return 1;
        }

        if (shared_buffer_size <= 0) {
            cerr << "Buffer size must be a positive number.\n";
            return 1;
        }

        if (private_buffer_size <= 0) {
            cerr << "Private buffer size must be a positive number.\n";
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
        cout << "Initializing server...\n";

        auto model_destroyer = QuadModel::init(db_folder, shared_buffer_size, private_buffer_size, max_threads,
                                               path_mode, index_mode, index_type);
        quad_model.catalog().print();

        server(port, chrono::seconds(seconds_timeout));
        cout << "\nServer shutting down\n";
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
