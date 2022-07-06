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
#include "base/thread/thread_key.h"
#include "base/thread/thread_info.h"
#include "network/tcp_buffer.h"
#include "parser/query/grammar/error_listener.h"
#include "parser/query/query_parser.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace std;
using boost::asio::ip::tcp;

// For random generation
std::uniform_int_distribution<uint64_t> random_uint64;
std::random_device rd;
std::mt19937_64 gen(rd());

std::queue<ThreadKey> running_threads_queue;
std::unordered_map<ThreadKey, ThreadInfo, ThreadKeyHasher> running_threads;
std::mutex running_threads_mutex;


void session(ThreadKey thread_key, ThreadInfo* thread_info, tcp::socket sock) {
    auto remove_thread_from_running_threads = [&]() {
        std::lock_guard<std::mutex> guard(running_threads_mutex);
        auto it = running_threads.find(thread_key);
        if (it != running_threads.end()) {
            running_threads.erase(it);
        }
    };

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
        antlr4::MyErrorListener error_listener;

        // without this line ConnectionException won't be catched propertly
        os.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        unique_ptr<BindingIter> physical_plan;

        // start timer
        auto start = chrono::system_clock::now();
        try {
            auto logical_plan = QueryParser::get_query_plan(query, &error_listener);
            physical_plan = quad_model.exec(*logical_plan, thread_info);
        }
        catch (const QueryParsingException& e) {
            os << "---------------------------------------\n";
            os << "Query Parsing Exception: " << e.what() << "\n";
            os << "---------------------------------------\n";
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::query_error);
            remove_thread_from_running_threads();
            return;
        }
        catch (const QueryException& e) {
            os << "---------------------------------------\n";
            os << "Query Exception: " << e.what() << "\n";
            os << "---------------------------------------\n";
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::query_error);
            remove_thread_from_running_threads();
            return;
        }
        catch (const LogicException& e) {
            os << "---------------------------------------\n";
            os << "Logic Exception: " << e.what() << "\n";
            os << "---------------------------------------\n";
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
            remove_thread_from_running_threads();
            return;
        }
        chrono::duration<float, std::milli> parser_duration = chrono::system_clock::now() - start;
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
            os << "Query Parser/Optimizer time: " << parser_duration.count() << " ms.\n";

            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::success);
        }
        catch (const LogicException& e) {
            os << "---------------------------------------\n";
            os << "LogicException:" << e.what() << endl;
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
        }
        catch (const InterruptedException& e) {
            std::cerr << "QueryInterrupted" << endl;
            auto t = chrono::system_clock::now() - start;
            os << "---------------------------------------\n";
            os << "Timeout thrown after "
               << std::chrono::duration_cast<std::chrono::milliseconds>(t).count()
               << " milliseconds.\n";
            os << "Found " << result_count << " results before timeout.\n";
            os << "Query Parser/Optimizer time: " << parser_duration.count() << " ms.\n";
            tcp_buffer.set_status(CommunicationProtocol::StatusCodes::timeout);
        }
    }
    catch (const ConnectionException& e) {
        std::cerr << "Lost connection with client: " << e.what() << endl;
    }
    catch (const LogicException& e) {
        std::cerr << "LogicException:" << e.what() << endl;
    }
    catch (...) {
        std::cerr << "Unknown exception." << endl;
    }

    remove_thread_from_running_threads();
}


void execute_timeouts(std::chrono::seconds timeout_duration) {
    std::chrono::nanoseconds sleep_time;
    while (true) {
        bool should_sleep = false;
        if (running_threads_queue.empty()) {
            should_sleep = true;
            sleep_time = timeout_duration;
        } else {
            auto thread_key = running_threads_queue.front();
            std::lock_guard<std::mutex> guard(running_threads_mutex);
            auto it = running_threads.find(thread_key);
            if (it == running_threads.end()) {
                // if thread_key is not in running_threads, it means the thread already finished
                running_threads_queue.pop();
            } else {
                auto now = chrono::system_clock::now();
                if (it->second.timeout <= now) {
                    it->second.interruption_requested = true;
                    running_threads_queue.pop();
                }
                else {
                    // can't sleep here because the mutex is active
                    auto remaining = it->second.timeout - now;
                    should_sleep = true;
                    sleep_time = remaining;
                }
            }
        }
        if (should_sleep) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}


void server(unsigned short port, std::chrono::seconds timeout_duration) {
    boost::asio::io_context io_context;

    std::thread(execute_timeouts, timeout_duration).detach();

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    cout << "Server running on port " << port << endl;
    cout << "To terminate press CTRL-C" << endl;
    while (true) {
        tcp::socket sock(io_context);
        acceptor.accept(sock);

        auto now = chrono::system_clock::now();

        auto timeout = now + timeout_duration;
        uint64_t timestamp = now.time_since_epoch().count();
        uint64_t rand = random_uint64(gen);

        ThreadKey thread_key(timestamp, rand);
        ThreadInfo thread_info(timeout);

        running_threads_queue.push(thread_key);
        auto insertion = running_threads.insert({thread_key, thread_info});
        std::thread(session, thread_key, &insertion.first->second, std::move(sock)).detach();
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
        auto model_destroyer = QuadModel::init(db_folder, shared_buffer_size, private_buffer_size, max_threads);

        cout << "Initializing server...\n";
        quad_model.catalog().print();

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
