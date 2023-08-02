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

#include <iostream>

#include <boost/asio.hpp>
#include <string>

#include "graph_models/quad_model/quad_model.h"
#include "network/mql/server.h"
#include "storage/buffer_manager.h"
#include "storage/filesystem.h"
#include "third_party/cxxopts/cxxopts.h"

int main(int argc, char **argv) {
    int seconds_timeout;
    int port;
    int str_max_initial_populate_size;
    int shared_buffer_size;
    int private_buffer_size;
    int threads;
    int limit;
    std::string db_folder;

    std::ios_base::sync_with_stdio(false);
    auto default_threads = std::thread::hardware_concurrency();

    // TODO: We would like to receive shared and private buffer param in MB or GB
    try {
        // Parse arguments
        cxxopts::Options options("server", "MillenniumDB server");
        options.add_options()
            ("h,help", "Print usage")
            ("d,db-folder", "set database folder path", cxxopts::value<std::string>(db_folder))
            ("p,port", "database server port",
                cxxopts::value<int>(port)->default_value(std::to_string(MQL::Server::DEFAULT_PORT)))
            ("t,timeout", "timeout (in seconds)", cxxopts::value<int>(seconds_timeout)->default_value("60"))
            ("s,string-initial-populate-size", "set how many GB of strings are loaded into memory during server start",
                cxxopts::value<int>(str_max_initial_populate_size)->default_value("2"))
            ("b,buffer-size", "set shared buffer pool size",
                cxxopts::value<int>(shared_buffer_size)->default_value(std::to_string(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE)))
            ("private-buffer-size", "set private buffer pool size for each thread",
                cxxopts::value<int>(private_buffer_size)->default_value(std::to_string(BufferManager::DEFAULT_PRIVATE_BUFFER_POOL_SIZE)))
            ("threads", "set worker threads", cxxopts::value<int>(threads)->default_value(std::to_string(default_threads)))
            ("l,limit", "set maximum output size", cxxopts::value<int>(limit)->default_value("0"))
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
            std::cerr << "Must specify a db-folder.\n";
            return 1;
        }

        if (port <= 0) {
            std::cerr << "Port must be a positive number.\n";
            return 1;
        }

        if (threads <= 0) {
            std::cerr << "Worker threads must be a positive number.\n";
            return 1;
        }

        if (seconds_timeout <= 0) {
            std::cerr << "Timeout must be a positive number.\n";
            return 1;
        }

        if (shared_buffer_size <= 0) {
            std::cerr << "Buffer size must be a positive number.\n";
            return 1;
        }

        if (private_buffer_size <= 0) {
            std::cerr << "Private buffer size must be a positive number.\n";
            return 1;
        }

        if (!Filesystem::exists(db_folder)) {
            std::cerr << "Database folder does not exists.\n";
            return 1;
        } else if (!Filesystem::is_directory(db_folder)) {
            std::cerr << "Database folder is not a directory.\n";
            return 1;
        }

        // Initialize model
        std::cout << "Initializing server...\n";

        auto model_destroyer = QuadModel::init(db_folder,
                                               str_max_initial_populate_size,
                                               shared_buffer_size,
                                               private_buffer_size,
                                               threads);

        if (limit != 0) {
            quad_model.MAX_LIMIT = limit;
        }

        quad_model.catalog().print();

        MQL::Server server;
        server.run(port, threads, std::chrono::seconds(seconds_timeout));

        return EXIT_SUCCESS;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Exception of unknown type!\n";
        return 1;
    }
    return 0;
}
