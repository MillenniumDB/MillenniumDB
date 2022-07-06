/**
 * Read all strings of a database and checks its present on the ExtendibleHash and can be found
 */

#include <fstream>
#include <iostream>

#include "base/exceptions.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace std;

int main(int argc, char **argv) {
    string db_folder;
    int buffer_size;

    cxxopts::Options options("create_db", "Import a database from a text file");
    options.add_options()
        ("h,help", "Print usage")
        ("d,db-folder", "path to the database folder to be created", cxxopts::value<string>(db_folder))
        ("b,buffer-size", "set buffer pool size", cxxopts::value<int>(buffer_size)->default_value(
            std::to_string(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE)))
    ;

    options.positional_help("db-folder");
    options.parse_positional({"db-folder"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    auto model_destroyer = QuadModel::init(db_folder, buffer_size, 0, 0);

    auto& object_file  = quad_model.object_file();
    auto& strings_hash = quad_model.strings_hash();

    uint64_t current_id = 1;

    uint64_t correct = 0;
    uint64_t wrong = 0;
    uint64_t not_found = 0;
    try {
        while (true) {
            auto str = object_file.get_string(current_id);

            auto id_found = strings_hash.get_id(str);
            if (current_id != id_found) {
                if (id_found == ObjectId::OBJECT_ID_NOT_FOUND) {
                    ++not_found;
                     cerr << "string \"" << str << "\", real id: " << current_id
                        << ", not found\n";
                } else {
                    ++wrong;
                    auto diff = current_id - id_found;
                    cerr << "Wrong ID for string \"" << str << "\", real id: " << current_id
                        << ", found: " << id_found << ", diff: " << diff << "\n";
                }
                exit(1);
            } else {
                ++correct;
            }

            current_id += str.size() + 1;
        }
    }
    catch (LogicException&) {
        // This exceptions is expected when we want to retrieve something out of bounds
        cout << "Reached end of ObjectFile.\n";
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }
    cout << "Wrong:   " << wrong << "\n";
    cout << "Not found: " << not_found << "\n";
    cout << "Correct: " << correct << "\n";
}
