#include <iostream>

#include "graph_models/rdf_model/rdf_model.h"
#include "network/sparql/server.h"
#include "storage/buffer_manager.h"
#include "storage/filesystem.h"
#include "third_party/cxxopts/cxxopts.h"

int main(int argc, char* argv[]) {
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
                cxxopts::value<int>(port)->default_value(std::to_string(SPARQL::Server::DEFAULT_PORT)))
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

        std::cout << "Initializing server..." << std::endl;
        auto model_destroyer = RdfModel::init(db_folder,
                                              str_max_initial_populate_size,
                                              shared_buffer_size,
                                              private_buffer_size,
                                              threads);

        if (limit != 0) {
            rdf_model.MAX_LIMIT = limit;
        }

        rdf_model.catalog().print();

        SPARQL::Server server;
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
}
