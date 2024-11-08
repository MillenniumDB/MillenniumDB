#include "tensor_store.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "graph_models/quad_model/quad_model.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "query/exceptions.h"
#include "system/file_manager.h"
#include "storage/filesystem.h"
#include "storage/index/tensor_store/lsh/forest_index.h"
#include "storage/index/tensor_store/lsh/forest_index_query_iter.h"
#include "storage/index/tensor_store/lsh/tree.h"
#include "storage/index/tensor_store/serialization.h"
#include "storage/index/tensor_store/tensor_buffer_manager.h"

TensorStore::TensorStore(const std::string& name_, uint64_t tensors_dim_, uint64_t tensor_page_buffer_size_in_bytes) :
    name(name_),
    tensors_dim(tensors_dim_),
    tensors_file_id(FileId::UNASSIGNED),
    mapping_path(file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name_ + ".mapping")),
    index_path(file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name_ + ".index")) {
    if (tensors_dim < 1)
        throw std::invalid_argument("Tensor dimension must be at least 1");

    // Create the tensor stores directory if it does not exist
    std::string tensor_stores_path = file_manager.get_file_path(TENSOR_STORES_DIR);
    if (!Filesystem::is_directory(tensor_stores_path))
        Filesystem::create_directories(tensor_stores_path);

    // Create a new tensors file
    std::string tensors_path = file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name + ".tensors");
    if (Filesystem::is_regular_file(tensors_path))
        std::remove(tensors_path.c_str());
    tensors_file_id = file_manager.get_file_id(TENSOR_STORES_DIR + "/" + name + ".tensors");

    // Create a new mapping file with the initial header
    std::fstream ofs;
    ofs.open(mapping_path, std::ios::out | std::ios::binary | std::ios::trunc);
    Serialization::write_bool(ofs, false);
    Serialization::write_uint64(ofs, tensors_dim);
    Serialization::write_uint64(ofs, 0ULL);
    ofs.close();

    set_tensor_buffer_manager(tensor_page_buffer_size_in_bytes, false);
}


TensorStore::TensorStore(const std::string& name_, uint64_t tensor_buffer_page_size_in_bytes, bool preload) :
    name(name_),
    tensors_file_id (FileId::UNASSIGNED),
    mapping_path    (file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name_ + ".mapping")),
    index_path      (file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name_ + ".index")) {
    if (!exists(name))
        throw std::invalid_argument("Tensor store " + name + " does not exist");

    // Load tensors file
    tensors_file_id = file_manager.get_file_id(TENSOR_STORES_DIR + "/" + name + ".tensors");

    // Load object_id2tensor_offset and forest_index (if it exists) from disk
    deserialize();
    assert(tensors_dim > 0);

    set_tensor_buffer_manager(tensor_buffer_page_size_in_bytes, preload);
}


TensorStore::~TensorStore() {
    // Save object_id2tensor_offset and forest_index to disk
    // Disabled because no updates will occur during server execution
    // serialize();
}


bool TensorStore::exists(const std::string& name) {
    if (!Filesystem::is_regular_file(file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name + ".tensors")))
        return false;
    else if (!Filesystem::is_regular_file(file_manager.get_file_path(TENSOR_STORES_DIR + "/" + name + ".mapping")))
        return false;
    return true;
}


void TensorStore::bulk_import(const std::string& tensors_csv_path,
                              const std::string& tensor_store_name,
                              uint64_t           tensors_dim) {
    if (tensors_dim < 1) {
        throw std::invalid_argument("Tensor dimension must be at least 1");
    }

    // Create the tensor stores directory if it does not exist
    const auto tensor_stores_path = file_manager.get_file_path(TENSOR_STORES_DIR);
    if (!Filesystem::is_directory(tensor_stores_path)) {
        Filesystem::create_directories(tensor_stores_path);
    }


    // Create the files
    const auto tensors_file_path = file_manager.get_file_path(TENSOR_STORES_DIR + "/" + tensor_store_name + ".tensors");
    const auto mapping_file_path = file_manager.get_file_path(TENSOR_STORES_DIR + "/" + tensor_store_name + ".mapping");
    std::fstream tensors_file_ofs(tensors_file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    std::fstream mapping_file_ofs(mapping_file_path, std::ios::out | std::ios::binary | std::ios::trunc);

    std::string   line;
    std::string   cell;
    uint_fast32_t current_line_index = 0;

    robin_hood::unordered_flat_set<uint64_t> object_ids;

    std::fstream tensors_csv_ifs(tensors_csv_path, std::ios::in | std::ios::binary);
    Serialization::write_uint64(mapping_file_ofs, tensors_dim);
    Serialization::write_uint64(mapping_file_ofs, 0);

    // Process the csv line by line and insert the tensors
    std::cout << "Inserting tensors...\n";
    const auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(tensors_csv_ifs, line)) {
        std::stringstream ss(line);

        // ObjectId
        std::getline(ss, cell, ',');
        const auto object_id = QuadObjectId::get_fixed_node_inside(cell);
        if (object_ids.contains(object_id.id)) {
            std::cout << "Skipping duplicate ObjectId: \"" << cell << "\" at line " << current_line_index << "\n";
            continue;
        }

        // Write object_id2tensor_offset
        Serialization::write_uint64(mapping_file_ofs, object_id.id);
        Serialization::write_uint64(mapping_file_ofs, tensors_file_ofs.tellp());

        object_ids.insert(object_id.id);

        // Tensor entries
        for (auto i = 0u; i < tensors_dim; i++) {
            std::getline(ss, cell, ',');
            const auto f = std::strtof(cell.c_str(), nullptr);
            Serialization::write_float(tensors_file_ofs, f);
        }
        ++current_line_index;
    }
    tensors_csv_ifs.close();

    // Add zeroes in order to make the file size a multiple of TensorPage::SIZE (nesessary for the tensor_buffer_manager)
    const auto tensors_file_size = tensors_file_ofs.tellp();
    if (tensors_file_size % TensorPage::SIZE != 0) {
        size_t remaining_bytes = TensorPage::SIZE - (tensors_file_size % TensorPage::SIZE);
        while (remaining_bytes > 0) {
            tensors_file_ofs.write("\0", 1);
            --remaining_bytes;
        }
    }
    tensors_file_ofs.close();

    // Update the metadata in the mapping file
    mapping_file_ofs.seekp(sizeof(uint64_t));
    Serialization::write_uint64(mapping_file_ofs, object_ids.size());
    mapping_file_ofs.close();

    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "TensorStore::bulk_import took: "
              << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds for "
              << object_ids.size() << " tensors\n";
}


void TensorStore::load_tensor_stores(uint64_t tensor_page_buffer_size_in_bytes, bool preload) {
    const auto tensor_stores_path = file_manager.get_file_path(TensorStore::TENSOR_STORES_DIR);

    if (Filesystem::is_directory(tensor_stores_path)) {
        robin_hood::unordered_set<std::string> tensor_store_names;
        // Get the stem of each file
        for (const auto& file : Filesystem::directory_iterator(tensor_stores_path)) {
            tensor_store_names.insert(file.path().filename().stem());
        }

        // Load the tensor stores
        for (const auto& tensor_store_name : tensor_store_names) {
            if (TensorStore::exists(tensor_store_name)) {
                std::cout << "Loading Tensor Store \"" << tensor_store_name << "\"...\n";
                auto tensor_store = std::make_unique<TensorStore>(tensor_store_name, tensor_page_buffer_size_in_bytes, preload);
                std::cout << "  num_tensors     : " << tensor_store->size() << "\n";
                std::cout << "  tensors_dim     : " << tensor_store->tensors_dim << "\n";
                if (tensor_store->forest_index != nullptr) {
                    std::cout << "  num_trees       : " << tensor_store->forest_index->num_trees << "\n";
                    std::cout << "  max_bucket_size : " << tensor_store->forest_index->max_bucket_size << "\n";
                    std::cout << "  max_depth       : " << tensor_store->forest_index->max_depth << "\n";
                }
                quad_model.catalog.name2tensor_store.emplace(tensor_store_name, std::move(tensor_store));
            }
        }
    }

    if (quad_model.catalog.name2tensor_store.size() > 0) {
        std::cout << "Successfully loaded " << quad_model.catalog.name2tensor_store.size() << " Tensor Store(s)\n";
        std::cout << "-------------------------------------\n";
    }
}


bool TensorStore::contains(uint64_t object_id) const {
    return object_id2tensor_offset.find(object_id) != object_id2tensor_offset.end();
}


bool TensorStore::get(uint64_t object_id, std::vector<float>& vec) const {
    assert(vec.size() == tensors_dim && "Vector size must match tensor dimension");
    auto it = object_id2tensor_offset.find(object_id);
    if (it == object_id2tensor_offset.end())
        return false;

    auto vec_bytes            = reinterpret_cast<char*>(vec.data());
    const auto vec_bytes_size = sizeof(float) * vec.size();

    // Start from the corresponding page and offset
    auto page_number         = it->second / TensorPage::SIZE;
    auto page_offset         = it->second % TensorPage::SIZE;
    TensorPage* current_page = &tensor_buffer_manager->get_page(page_number);

    // Read the tensor directly to the vector bytes
    size_t remaining_bytes = sizeof(float) * vec.size();
    while (remaining_bytes > 0) {
        size_t max_read_bytes = (TensorPage::SIZE - page_offset);
        auto   current_ptr    = current_page->get_bytes() + page_offset;
        if (remaining_bytes <= max_read_bytes) {
            // All the remaining bytes are in the current page
            std::memcpy(&vec_bytes[vec_bytes_size - remaining_bytes], current_ptr, remaining_bytes);
            tensor_buffer_manager->unpin(*current_page);
            break;
        } else {
            // There are remaining bytes in the next page
            std::memcpy(&vec_bytes[vec_bytes_size - remaining_bytes], current_ptr, max_read_bytes);
            tensor_buffer_manager->unpin(*current_page);
            remaining_bytes -= max_read_bytes;
            ++page_number;
            page_offset  = 0;
            current_page = &tensor_buffer_manager->get_page(page_number);
        }
    }

    return true;
}


size_t TensorStore::size() const {
    return object_id2tensor_offset.size();
}


void TensorStore::build_forest_index(LSH::MetricType metric_type, uint64_t num_trees, uint64_t max_bucket_size, uint64_t max_depth) {
    if (size() == 0)
        throw std::logic_error("Cannot build forest index because the store is empty!");
    forest_index = std::make_unique<LSH::ForestIndex>(*this, metric_type, num_trees, max_bucket_size, max_depth);
    forest_index->build();

}


std::vector<std::pair<uint64_t, float>>
  TensorStore::query_top_k(const std::vector<float>& query_tensor, int64_t k, int64_t search_k) const {
    if (forest_index == nullptr)
        throw LogicException("Forest index is not built for this tensor store \"" + name + "\"");
    if (k <= 0)
        throw LogicException("k must be positive");
    return forest_index->query_top_k(query_tensor, uint64_t(k), search_k);
}


std::unique_ptr<LSH::ForestIndexQueryIter> TensorStore::query_iter(const std::vector<float>& query_tensor) const {
    if (forest_index == nullptr)
        throw LogicException("Forest index is not built for this tensor store \"" + name + "\"");
    return forest_index->query_iter(query_tensor);
}


void TensorStore::serialize() const {
    // Serialize mapping
    std::fstream ofs(mapping_path, std::ios::out | std::ios::binary | std::ios::trunc);
    Serialization::write_uint64(ofs, tensors_dim);
    Serialization::write_uint64(ofs, object_id2tensor_offset.size());
    Serialization::write_uint642uint64_unordered_map(ofs, object_id2tensor_offset);
    ofs.close();

    // Serialize forest index
    if (forest_index != nullptr)
        forest_index->serialize(index_path);
}


void TensorStore::deserialize() {
    // Deserialize mapping
    std::fstream ifs(mapping_path, std::ios::in | std::ios::binary);
    tensors_dim                       = Serialization::read_uint64(ifs);
    auto object_id2tensor_offset_size = Serialization::read_uint64(ifs);
    object_id2tensor_offset = Serialization::read_uint642uint64_unordered_map(ifs, object_id2tensor_offset_size);
    ifs.close();

    // Deserialize forest index
    if (Filesystem::is_regular_file(index_path))
        forest_index = std::make_unique<LSH::ForestIndex>(index_path, *this);
}


void TensorStore::set_tensor_buffer_manager(uint64_t tensor_buffer_page_size_in_bytes, bool preload) {
    const auto tensor_page_buffer_pool_size = tensor_buffer_page_size_in_bytes / TensorPage::SIZE;
    tensor_buffer_manager= std::make_unique<TensorBufferManager>(tensors_file_id, tensor_page_buffer_pool_size, preload);
}
