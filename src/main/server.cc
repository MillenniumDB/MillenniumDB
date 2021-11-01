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
#include <boost/program_options.hpp>

#include "base/exceptions.h"
#include "base/binding/binding.h"
#include "base/binding/binding_iter.h"
#include "base/graph/graph_model.h"
#include "base/exceptions.h"
#include "base/parser/logical_plan/op/op_select.h"
#include "base/parser/query_parser.h"
#include "base/thread/thread_key.h"
#include "relational_model/models/quad_model/quad_model.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "server/tcp_buffer.h"

using namespace std;
using boost::asio::ip::tcp;
namespace po = boost::program_options;

// For random generation
std::uniform_int_distribution<uint64_t> random_uint64;
std::random_device rd;
std::mt19937_64 gen(rd());

std::queue<ThreadKey> running_threads_queue;
std::unordered_map<ThreadKey, ThreadInfo, ThreadKeyHasher> running_threads;
std::mutex running_threads_mutex;


void session(ThreadKey thread_key, ThreadInfo* thread_info, tcp::socket sock, GraphModel* model) {
    auto remove_thread_from_running_threads = [&]() {
        std::lock_guard<std::mutex> guard(running_threads_mutex);
        auto it = running_threads.find(thread_key);
        if (it != running_threads.end()) {
            running_threads.erase(it);
        }
    };

    try {
        unsigned char query_size_b[db_server::BYTES_FOR_QUERY_LENGTH];
        boost::asio::read(sock, boost::asio::buffer(query_size_b, db_server::BYTES_FOR_QUERY_LENGTH));

        int query_size = 0;
        for (int i = 0, offset = 0; i < db_server::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
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

        // without this line ConnectionException won't be catched propertly
        os.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        unique_ptr<BindingIter> physical_plan;

        // start timer
        auto start = chrono::system_clock::now();
        try {
            auto logical_plan = QueryParser::get_query_plan(query);
            physical_plan = model->exec(*logical_plan, thread_info);
        }
        // catch (QueryParsingException& e) {
        //     // Try with manual plan
        //     try {
        //         auto manual_plan = QueryParser::get_manual_plan(query);
        //         physical_plan = model->exec(manual_plan);
        //     }
        //     catch (QueryException& e) {
        //         os << "(Manual Plan) Query Parsing Exception: " << e.what() << "\n";
        //         tcp_buffer.set_error();
        //         remove_thread_from_running_threads();
        //         return;
        //     }
        // }
        catch (const QueryException& e) {
            os << "---------------------------------------\n";
            os << "Query Exception: " << e.what() << "\n";
            os << "---------------------------------------\n";
            tcp_buffer.set_error(db_server::ErrorCode::query_error);
            remove_thread_from_running_threads();
            return;
        }
        chrono::duration<float, std::milli> parser_duration = chrono::system_clock::now() - start;
        uint64_t result_count = 0;
        try {
            auto execution_start = chrono::system_clock::now();

            auto& binding = physical_plan->get_binding();
            binding.print_header(os);
            os << "---------------------------------------\n";

            physical_plan->begin();

            // get all results
            while (physical_plan->next()) {
                os << binding << '\n';
                result_count++;
            }

            chrono::duration<float, std::milli> execution_duration = chrono::system_clock::now() - execution_start;

            // print execution stats in server console
            cout << "\nPlan Executed:\n";
            physical_plan->analyze(cout, 2);
            cout << "\nResults:" << result_count << "\n";

            // write execution stats in output stream
            os << "---------------------------------------\n";
            os << "Found " << result_count << " results.\n";
            os << "Execution time: " << execution_duration.count() << " ms.\n";
            os << "Query Parser/Optimizer time: " << parser_duration.count() << " ms.\n";
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
            tcp_buffer.set_error(db_server::ErrorCode::timeout);
        }
    }
    catch (const ConnectionException& e) {
        std::cerr << "Lost connection with client: " << e.what() << endl;
    }
    // catch (...) {
    //     std::cerr << "Unknown exception." << endl;
    // }

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


void server(unsigned short port, GraphModel* model, std::chrono::seconds timeout_duration) {
    boost::asio::io_context io_context;

    std::thread(execute_timeouts, timeout_duration).detach();

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    cout << "Server running on port " << port << endl;
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
        std::thread(session, thread_key, &insertion.first->second, std::move(sock), model).detach();
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
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "show this help message")
            ("db-folder,d", po::value<string>(&db_folder)->required(), "set database folder path")
            ("port,p", po::value<int>(&port)->default_value(db_server::DEFAULT_PORT), "database server port")
            ("timeout", po::value<int>(&seconds_timeout)->default_value(60), "timeout (in seconds)")
            (
                "buffer-size,b",
                po::value<int>(&shared_buffer_size)->default_value(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE),
                "set shared buffer pool size"
            )
            (
                "private-buffer-size,",
                po::value<int>(&private_buffer_size)->default_value(BufferManager::DEFAULT_PRIVATE_BUFFER_POOL_SIZE),
                "set private buffer pool size for each thread"
            )
            ("max-threads,", po::value<int>(&max_threads)->default_value(8), "set max threads")
        ;

        po::positional_options_description p;
        p.add("db-folder", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

        if (vm.count("help")) {
            cout << "Usage: server [options] DB_FOLDER\n";
            cout << desc << "\n";
            return 0;
        }
        po::notify(vm);

        // Validate params
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

        // Initialize model
        QuadModel model(db_folder, shared_buffer_size, private_buffer_size, max_threads);

        cout << "Initializing server...\n";
        model.catalog().print();

        server(port, &model, std::chrono::seconds(seconds_timeout));
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
