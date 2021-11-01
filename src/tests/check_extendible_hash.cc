/**
 * Read all strings of a database and checks its present on the ExtendibleHash and can be found
 */

#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

#include "relational_model/models/quad_model/quad_model.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv) {
    string db_folder;
    int buffer_size;

    // Parse arguments
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "show this help message")
        ("buffer-size,b", po::value<int>(&buffer_size)->default_value(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE),
                "set shared buffer pool size")
        ("db-folder,d", po::value<string>(&db_folder)->required(), "set database folder path")
    ;

    po::positional_options_description p;
    p.add("db-folder", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

    if (vm.count("help")) {
        cout << "Usage: check_extendible_hash DB_FOLDER [OPTIONS]\n";
        cout << desc << "\n";
        return 0;
    }
    po::notify(vm);

    QuadModel model(db_folder, buffer_size, 0, 0);

    auto& object_file  = model.object_file();
    auto& strings_hash = model.strings_hash();

    uint64_t current_id = 1;

    uint64_t correct = 0;
    uint64_t wrong = 0;
    uint64_t not_found = 0;
    try {
        while (true) {
            auto c_str = object_file.read(current_id);
            string str(c_str);

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
            } else {
                ++correct;
            }

            current_id += str.size() + 1;
        }
    }
    catch (ObjectFileOutOfBounds&) {
        cout << "Reached end of ObjectFile.\n";
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }
    cout << "Wrong:   " << wrong << "\n";
    cout << "Not found: " << not_found << "\n";
    cout << "Correct: " << correct << "\n";
}
