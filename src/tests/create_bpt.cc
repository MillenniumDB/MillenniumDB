#include <climits>
#include <experimental/filesystem>
#include <iostream>
#include <memory>

#include "storage/file_manager.h"
#include "storage/buffer_manager.h"
#include "storage/index/bplus_tree/bplus_tree.h"

using namespace std;

void create_ascending(int size, std::string bpt_name) {
    std::cout << "Creating bpt: " <<  bpt_name << " ...\n";
    auto bpt = BPlusTree<3>(bpt_name);

    uint64_t n = 0;
    for (int i = 1; i <= size; i++) {
        uint64_t c[3] = {};
        c[0] = n++;
        c[1] = n++;
        c[2] = n++;

        bpt.insert( RecordFactory::get(c[0], c[1], c[2]) );
    }

    std::cout << "bpt created. Now checking...\n";

    if (!bpt.check()) {
        std::cout << "IMPORTANT: errors found while checking.\n";
        return;
    } else {
        std::cout << "No errors found.\n";
    }
}


void create_descending(int size, std::string bpt_name) {
    std::cout << "Creating bpt: " <<  bpt_name << " ...\n";
    auto bpt = BPlusTree<3>(bpt_name);

    uint64_t n = UINT64_MAX;
    for (int i = 1; i <= size; i++) {
        uint64_t c[3] = {};
        c[0] = n--;
        c[1] = n--;
        c[2] = n--;

        bpt.insert( RecordFactory::get(c[0], c[1], c[2]) );
    }

    std::cout << "bpt created. Now checking...\n";

    if (!bpt.check()) {
        std::cout << "IMPORTANT: errors found while checking.\n";
        return;
    } else {
        std::cout << "No errors found.\n";
    }
}


void create_random(int size, std::string bpt_name) {
    std::cout << "Creating bpt: " <<  bpt_name << " ...\n";
    auto bpt = BPlusTree<3>(bpt_name);

    for (int i = 1; i <= size; i++) {
        uint64_t c[3] = {};
        c[0] = (uint64_t) rand();
        c[1] = (uint64_t) rand();
        c[2] = (uint64_t) rand();

        bpt.insert( RecordFactory::get(c[0], c[1], c[2]) );
    }

    std::cout << "bpt created. Now checking...\n";

    if (!bpt.check()) {
        std::cout << "IMPORTANT: errors found while checking.\n";
        return;
    } else {
        std::cout << "No errors found.\n";
    }
}


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "needed size\n";
        exit(1);
    }
    int size = atoi(argv[1]);

    string db_folder = "tests/dbs/test_bpt";
    { // check db_folder is empty or does not exists
        namespace fs = std::experimental::filesystem;
        if (fs::exists(db_folder) && !fs::is_empty(db_folder)) {
            cerr << "Database folder is not empty.\n";
            return 1;
        }
    }

    FileManager::init(db_folder);
    BufferManager::init(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE, 0, 0);

    try {
        create_random(size, "bpt_random");
        create_ascending(size, "bpt_ascending");
        create_descending(size, "bpt_descending");
    } catch (exception& e) {
        std::cerr << e.what() << "\n";
    }

    buffer_manager.~BufferManager();
    file_manager.~FileManager();
    return 0;
}
