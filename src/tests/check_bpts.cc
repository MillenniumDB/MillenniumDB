#include <fstream>
#include <iostream>

#include "query_optimizer/quad_model/quad_model.h"
#include "storage/buffer_manager.h"
#include "storage/filesystem.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace std;

template <std::size_t N>
void check(const string& name, BPlusTree<N>& bpt) {
    std::cout << "Checking " << name << ":\n";

    if (bpt.check()) {
        std::cout << "  Good\n";
    }
}

int main(int argc, char **argv) {
    string db_folder;
    int buffer_size;

    // Parse arguments
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

    { // check if db_folder is empty or does not exists
        if (!Filesystem::exists(db_folder) ) {
            cerr << "Database folder doesn't exists.\n";
            return 1;
        } else if (Filesystem::is_empty(db_folder)) {
            cerr << "Database folder is empty.\n";
            return 1;
        }
    }

    auto model_destroyer = QuadModel::init(db_folder, buffer_size, 0, 0);

    check("node_label", *quad_model.node_label);
    check("label_node", *quad_model.label_node);

    check("object_key_value", *quad_model.object_key_value);
    check("key_value_object", *quad_model.key_value_object);

    check("from_to_type_edge", *quad_model.from_to_type_edge);
    check("to_type_from_edge", *quad_model.to_type_from_edge);
    check("type_from_to_edge", *quad_model.type_from_to_edge);

    check("equal_from_to",      *quad_model.equal_from_to);
    check("equal_from_type",    *quad_model.equal_from_type);
    check("equal_to_type",      *quad_model.equal_to_type);
    check("equal_from_to_type", *quad_model.equal_from_to_type);

    check("equal_from_to_inverted",   *quad_model.equal_from_to_inverted);
    check("equal_from_type_inverted", *quad_model.equal_from_type_inverted);
    check("equal_to_type_inverted",   *quad_model.equal_to_type_inverted);
}
