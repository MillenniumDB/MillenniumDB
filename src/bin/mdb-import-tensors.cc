#include <iostream>
#include <thread>

#include "graph_models/quad_model/quad_model.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"
#include "third_party/cli11/CLI11.hpp"

using namespace LSH;


int main(int argc, char* argv[]) {
    std::string db_directory;
    std::string tensors_file;
    std::string tensor_store_name;
    uint64_t    tensors_dim   = 1;

    CLI::App app("MillenniumDB Import Tensors");
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("db-directory", db_directory)
      ->description("path to an existing database directory")
      ->type_name("<path>")
      ->check(CLI::ExistingDirectory.description(""))
      ->required();

    app.add_option("tensors-file", tensors_file)
      ->description("file containing tensors to be imported")
      ->type_name("<path>")
      ->check(CLI::ExistingFile.description(""))
      ->required();

    app.add_option("tensor-store-name", tensor_store_name)
      ->description("name of the tensor store")
      ->type_name("<name>")
      ->required();

    app.add_option("tensors-dim", tensors_dim)
      ->description("dimension of the tensors")
      ->type_name("<num>")
      ->check(CLI::Range(1, 8162))
      ->required();

    CLI11_PARSE(app, argc, argv);

    if (tensor_store_name.empty()) {
        std::cerr << "Tensor store name cannot be empty\n";
        return EXIT_FAILURE;
    }

    std::cout << "Importing tensors to a QuadModel database\n";
    std::cout << "  db directory      : " << db_directory << "\n";
    std::cout << "  tensors file      : " << tensors_file << "\n";
    std::cout << "  tensor store name : " << tensor_store_name << "\n";
    std::cout << "  tensors dim       : " << tensors_dim << "\n";

    std::cout << "Initializing a QuadModel...\n";

    System system(
        db_directory,
        StringManager::DEFAULT_LOAD_STR,
        BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE,
        std::thread::hardware_concurrency()
    );

    auto model_destroyer = QuadModel::init();
    std::cout << "QuadModel initialized\n";

    if (TensorStore::exists(tensor_store_name)) {
        std::cerr << "A tensor store with the name \"" << tensor_store_name << "\" already exists\n";
        return EXIT_FAILURE;
    }

    TensorStore::bulk_import(tensors_file, tensor_store_name, tensors_dim);

    return EXIT_SUCCESS;
}
