#include <chrono>
#include <iostream>

#include "graph_models/quad_model/quad_model.h"
#include "storage/filesystem.h"
#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/tensor_buffer_manager.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "system/string_manager.h"
#include "system/system.h"
#include "third_party/cli11/CLI11.hpp"

using namespace LSH;

void build_forest_index(TensorStore& tensor_store,
                        MetricType   metric_type,
                        uint64_t     num_trees,
                        uint64_t     max_bucket_size,
                        uint64_t     max_depth) {
    std::cout << "Building Forest Index..." << std::endl;
    auto start_build_forest_index = std::chrono::system_clock::now();
    tensor_store.build_forest_index(metric_type, num_trees, max_bucket_size, max_depth);
    auto end_build_forest_index = std::chrono::system_clock::now();
    auto duration_build_forest_index =
      std::chrono::duration_cast<std::chrono::seconds>(end_build_forest_index - start_build_forest_index);
    std::cout << "Build Forest Index took: " << duration_build_forest_index.count() << " seconds\n";
    std::cout << "Serializing..." << std::endl;
    tensor_store.serialize();
    auto end_serialize = std::chrono::system_clock::now();
    auto duration_serialize = std::chrono::duration_cast<std::chrono::seconds>(end_serialize - end_build_forest_index);
    std::cout << "Serialization took: " << duration_serialize.count() << " seconds\n";
}


int main(int argc, char* argv[]) {
    std::string db_directory;
    std::string tensor_store_name;
    MetricType  metric_type     = MetricType::ANGULAR;
    uint64_t    num_trees       = 1;
    uint64_t    max_bucket_size = 1024;
    uint64_t    max_depth       = UINT64_MAX;
    uint64_t    tensor_buffer   = TensorBufferManager::DEFAULT_TENSOR_PAGES_BUFFER_SIZE;
    bool        preload         = false;

    std::unordered_map<std::string, MetricType> name2metric_type = { { "angular", MetricType::ANGULAR },
                                                                     { "euclidean", MetricType::EUCLIDEAN },
                                                                     { "manhattan", MetricType::MANHATTAN } };

    CLI::App app("MillenniumDB Import Tensors");
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("db-directory", db_directory)
      ->description("path to an existing database directory")
      ->type_name("<path>")
      ->check(CLI::ExistingDirectory.description(""))
      ->required();

    app.add_option("tensor-store-name", tensor_store_name)
      ->description("name of the tensor store")
      ->type_name("<name>")
      ->required();

    app.add_option("metric-type", metric_type)
      ->description("metric type for the LSH Forest Index")
      ->type_name("<type>")
      ->transform(CLI::CheckedTransformer(name2metric_type, CLI::ignore_case))
      ->required();

    app.add_option("num-trees", num_trees)
      ->description("number of trees in the LSH Forest Index")
      ->type_name("<num>")
      ->check(CLI::Range(uint64_t(1), uint64_t(UINT64_MAX)))
      ->required();

    app.add_option("--max-bucket-size", max_bucket_size)
      ->description("maximum bucket size for the leaves in the LSH Forest Index trees")
      ->type_name("<num>")
      ->check(CLI::Range(uint64_t(1), uint64_t(UINT64_MAX)));

    app.add_option("--max-depth", max_depth)
      ->description("maximum depth of the trees in the LSH Forest Index")
      ->type_name("<num>")
      ->check(CLI::Range(uint64_t(1), uint64_t(UINT64_MAX)));

    app.add_option("--tensor-buffer", tensor_buffer)
      ->description("size of buffer for tensor pages shared between threads\nAllows units such as MB and GB")
      ->option_text("<bytes> [2GB]")
      ->transform(CLI::AsSizeValue(false))
      ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_flag("--preload-tensors", preload)
      ->description("Fill the tensor buffer before building the forest index");

    CLI11_PARSE(app, argc, argv);

    std::cout << "Building forest index...\n";
    std::cout << "  db directory      : " << db_directory << "\n";
    std::cout << "  tensor store name : " << tensor_store_name << "\n";
    std::cout << "  metric type       : ";
    switch (metric_type) {
    case MetricType::ANGULAR:
        std::cout << "Angular";
        break;
    case MetricType::EUCLIDEAN:
        std::cout << "Euclidean";
        break;
    case MetricType::MANHATTAN:
        std::cout << "Manhattan";
        break;
    default:
        std::cout << "Unknown";
        break;
    }
    std::cout << "\n";
    std::cout << "  num trees         : " << num_trees << "\n";
    std::cout << "  max bucket size   : " << max_bucket_size << "\n";
    std::cout << "  max depth         : " << max_depth << "\n";

    std::cout << "Initializing a QuadModel...\n";

    System system(
        db_directory,
        StringManager::DEFAULT_LOAD_STR,
        BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE,
        1
    );
    auto model_destroyer = QuadModel::init();
    std::cout << "QuadModel initialized\n";

    if (!TensorStore::exists(tensor_store_name)) {
        std::cerr << "The tensor store \"" << tensor_store_name << "\" does not exist\n";
        return EXIT_FAILURE;
    }

    if (Filesystem::is_regular_file(
          file_manager.get_file_path(TensorStore::TENSOR_STORES_DIR + "/" + tensor_store_name + ".index")))
    {
        std::cerr << "An index for the tensor store \"" << tensor_store_name
                  << "\" already exists. Please delete the .index file first\n";
        return EXIT_FAILURE;
    }

    TensorStore tensor_store(tensor_store_name, tensor_buffer, preload);

    build_forest_index(tensor_store, metric_type, num_trees, max_bucket_size, max_depth);

    return EXIT_SUCCESS;
}