/**
 * Read all strings of a database and checks its present on the ExtendibleHash and can be found
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include "graph_models/rdf_model/rdf_model.h"
#include "query/exceptions.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/hash/strings_hash/strings_hash.h"
#include "storage/string_manager.h"
#include "third_party/cxxopts/cxxopts.h"


int main(int argc, char **argv) {
    std::string db_folder;

    cxxopts::Options options("create_db", "Import a database from a text file");
    options.add_options()
        ("h,help", "Print usage")
        ("d,db-folder", "path to the database folder to be created", cxxopts::value<std::string>(db_folder))
    ;

    options.positional_help("db-folder");
    options.parse_positional({"db-folder"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    auto model_destroyer = RdfModel::init(db_folder, 2, BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE, 0, 0);

    uint64_t current_block = 0;
    uint64_t total_strings = 0;
    uint64_t current_block_offset = StringManager::METADATA_SIZE;
    uint64_t total_blocks = string_manager.get_total_blocks();
    uint64_t last_block_max_offset = string_manager.get_last_block_offset();

    while (current_block < total_blocks) {
        total_strings++;
        std::stringstream ss;
        uint64_t str_id = current_block_offset + current_block*StringManager::STRING_BLOCK_SIZE;
        string_manager.print(ss, str_id);
        std::string str_val = ss.str();

        std::cout << str_val << '\n';

        // check reasonable size
        if (str_val.size() > StringManager::STRING_BLOCK_SIZE) {
            std::cerr << "Wrong string length [id: " << str_id << ", len: " << str_val.size() << "]" << std::endl;
            return 1;
        }

        // string has correct hash
        auto found_id = string_manager.get_str_id(str_val);

        if (found_id != str_id) {
            std::cerr << "Wrong string id [real: " << str_id << ", found: " << found_id << "]" << std::endl;
            return 1;
        }

        uint64_t bytes_for_len = StringManager::get_bytes_for_len(str_val.size());
        current_block_offset += bytes_for_len + str_val.size();

        if (current_block_offset >= StringManager::STRING_BLOCK_SIZE) {
            current_block_offset = current_block_offset % StringManager::STRING_BLOCK_SIZE;
            current_block++;
        }
        // Go to next block if there are no enough space
        if (StringManager::STRING_BLOCK_SIZE - current_block_offset < StringManager::MIN_PAGE_REMAINING_BYTES) {
            current_block++;
            current_block_offset = 0;
        }

        // Border case for last block
        if (current_block + 1 == total_blocks) {
            if (current_block_offset >= last_block_max_offset) {
                break;
            }
        }
    }

    // TODO: check all ids of triples that are external are a correct string (ie: have length < 64M, reconstructing it doesn't fail)
    std::cout << "All good. Total strings: " << total_strings << std::endl;
}
