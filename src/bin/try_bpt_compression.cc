#include <cassert>
#include <iostream>
#include <string>

#include "graph_models/rdf_model/rdf_model.h"
#include "network/sparql/server.h"
#include "storage/buffer_manager.h"
#include "storage/filesystem.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace std;

uint64_t get_prefix_size(unsigned char* r1, unsigned char* r2) {
    // for (uint64_t i = 0; i < 24; i++) {
    //     if (r1[i] != r2[i]) {
    //         return i;
    //     }
    // }
    // return 24;
    uint64_t res = 0;
    for (int64_t i = 7; i >= 0; i--) {
        if (r1[i] != r2[i]) {
            return res;
        } else {
            res++;
        }
    }

    for (int64_t i = 15; i >= 8; i--) {
        if (r1[i] != r2[i]) {
            return res;
        } else {
            res++;
        }
    }
    for (int64_t i = 23; i >= 16; i--) {
        if (r1[i] != r2[i]) {
            return res;
        } else {
            res++;
        }
    }
    return res;
}

void calculate_bpt_size(const string& bpt_name, BPlusTree<3>& bpt) {
    // Attributes:
    // TupleCount: 4B can be less (2B)
    // Next Page Number: 4B
    // PrefixSize: 1B (?)
    // Prefix: variable (PrefixSize Bytes)
    // Records ( (24-PrefixSize)*TupleCount Bytes )

    // Max Records:
    // floor(4096 - (4+4+1+PrefixSize))/(24-PrefixSize)
    // PrefixSize = 23 -> MaxRecords: 4064
    bool interruption_requested = false;
    array<uint64_t, 3> min = {0,0,0};
    array<uint64_t, 3> max = {UINT64_MAX,UINT64_MAX,UINT64_MAX};
    auto it = bpt.get_range(&interruption_requested, Record<3>(min), Record<3>(max));


    auto record = it.next();
    if (record == nullptr) {
        cout << "BPT " << bpt_name << " empty\n";
        return;
    }
    array<uint64_t, 3> current_page_first_record = *record;
    array<uint64_t, 3> last_record = *record;
    uint64_t total_pages = 1;
    uint64_t current_page_tuple_count = 1;
    uint64_t repeated_triples = 0;
    uint64_t total_triples = 1;

    for (record = it.next(); record != nullptr; record = it.next()) {
        total_triples++;
        uint64_t new_prefix_size = get_prefix_size((unsigned char*) &((*record))[0], (unsigned char*) &current_page_first_record[0]);
        // assert(new_prefix_size < 24);
        if (last_record == *record) {
            repeated_triples++;
            // return;
            continue;
        }
        last_record = *record;
        uint64_t new_max_records = (4096-(4+4+1+new_prefix_size))/(24-new_prefix_size);

        if (current_page_tuple_count + 1 <= new_max_records) {
            current_page_tuple_count += 1;
        } else {
            total_pages++;
            current_page_tuple_count = 1;
            current_page_first_record = *record;
        }
    }

    cout << "BPT " << bpt_name << " would have " << total_pages << " pages\n";
    cout << "repeated triples: " << repeated_triples << " triples\n";
    cout << "total triples: " << total_triples << " triples\n";
}

int main(int argc, char* argv[]) {
    int seconds_timeout;
    int port;
    int str_max_initial_populate_size;
    int shared_buffer_size;
    int private_buffer_size;
    int threads;
    string db_folder;

    try {
        // Parse arguments
        cxxopts::Options options("server", "MillenniumDB server");
        options.add_options()
            ("h,help", "Print usage")
            ("d,db-folder", "set database folder path", cxxopts::value<string>(db_folder))
            ("p,port", "database server port",
                cxxopts::value<int>(port)->default_value(std::to_string(SPARQL::Server::DEFAULT_PORT)))
            ("t,timeout", "timeout (in seconds)", cxxopts::value<int>(seconds_timeout)->default_value("60"))
            ("s,string-initial-populate-size", "set how many GB of strings are loaded into memory during server start",
                cxxopts::value<int>(str_max_initial_populate_size)->default_value("2"))
            ("b,buffer-size", "set shared buffer pool size",
                cxxopts::value<int>(shared_buffer_size)->default_value(std::to_string(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE)))
            ("private-buffer-size", "set private buffer pool size for each thread",
                cxxopts::value<int>(private_buffer_size)->default_value(std::to_string(BufferManager::DEFAULT_PRIVATE_BUFFER_POOL_SIZE)))
            ("threads", "set worker threads", cxxopts::value<int>(threads)->default_value("8")) // TODO: default depending on cpu?
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

        if (threads <= 0) {
            cerr << "Worker threads must be a positive number.\n";
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

        std::cout << "Initializing server..." << std::endl;
        auto model_destroyer = RdfModel::init(db_folder,
                                              str_max_initial_populate_size,
                                              shared_buffer_size,
                                              private_buffer_size,
                                              threads);

        rdf_model.catalog().print();

        // SPARQL::Server server;
        // server.run(port, threads, std::chrono::seconds(seconds_timeout));
        calculate_bpt_size("spo", *rdf_model.spo);
        calculate_bpt_size("pos", *rdf_model.pos);
        calculate_bpt_size("pso", *rdf_model.pso);
        calculate_bpt_size("osp", *rdf_model.osp);

        return EXIT_SUCCESS;
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        cerr << "Exception of unknown type!\n";
        return 1;
    }
}
