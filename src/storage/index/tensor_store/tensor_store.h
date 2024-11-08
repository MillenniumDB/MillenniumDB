#pragma once
/*
 * TensorStore is an on-disk map between object ids and float tensors. The supported operations are insert (also
 * with replacement) and get. It has its own buffer manager for each instance. Once created the TensorStore is a
 * read-only structure, but in nothing in our architecture prevents the implementation of a writeable TensorStore
 * in the future.
 *
 * The TensorStore disk storage are the .tensors and .mapping files:
 *
 * {tensor_store_name}.tensors - stores all the tensor data as an adjacently-arranged vector of floats
 *
 * {tensor_store_name}.mapping - stores the header (bool has_forest_index and uint64 tensors_dim) and the mapping
 * between object_id2tensor_offset map for being loaded into memory
 *
 * {tensor_store_name}.index   - stores the forest index serialized if it was previously built
 */

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "storage/file_id.h"

class TensorBufferManager;

namespace LSH {
class ForestIndex;
class ForestIndexQueryIter;
class Tree;
enum class MetricType;
} // namespace LSH

class TensorStore {
public:
    friend class LSH::Tree;

    inline static const std::string TENSOR_STORES_DIR = "tensor_stores";

    // Check if a tensor store with a given name exists. At least the mapping and tensor files must exist
    static bool exists(const std::string& name);

    // Create a new tensor store
    static void bulk_import(const std::string& tensors_csv_path, const std::string& tensor_store_name, uint64_t tensors_dim);

    // Load all existing tensor stores
    static void load_tensor_stores(uint64_t tensor_page_buffer_size_in_bytes, bool preload);

    const std::string name;
    uint64_t          tensors_dim;

    // Vector index
    std::unique_ptr<LSH::ForestIndex> forest_index;

    // Initialize a new tensor store
    TensorStore(const std::string& name, uint64_t tensors_dim, uint64_t tensor_page_buffer_size_in_bytes);

    // Load an existing tensor store with a given name
    TensorStore(const std::string& name, uint64_t tensor_page_buffer_size_in_bytes, bool preload);

    // Trigger the serialization before being destroyed
    ~TensorStore();

    // Check if a tensor with the given id exists
    bool contains(uint64_t object_id) const;

    // Write a tensor with the given object id into vec. It is assumed that vec.size() == tensors_dim. Return true
    // if the tensor is found, false otherwise
    bool get(uint64_t object_id, std::vector<float>& vec) const;

    size_t size() const;

    // Build and set a forest index with the entire tensor store
    void build_forest_index(LSH::MetricType metric_type, uint64_t num_trees, uint64_t max_bucket_size, uint64_t max_depth);

    std::vector<std::pair<uint64_t, float>> query_top_k(const std::vector<float>& query_tensor, int64_t k, int64_t search_k) const;

    std::unique_ptr<LSH::ForestIndexQueryIter> query_iter(const std::vector<float>& query_tensor) const;

    // Serialize the tensor store (mapping and forest index if it exists)
    void serialize() const;

private:
    FileId tensors_file_id;

    std::string mapping_path;
    std::string index_path;

    std::unique_ptr<TensorBufferManager> tensor_buffer_manager;


    // Mapping between object id and its tensor bytes offset in the file (bytes)
    std::unordered_map<uint64_t, uint64_t> object_id2tensor_offset;

    // Deserialize the tensor store
    void deserialize();

    void set_tensor_buffer_manager(uint64_t tensor_buffer_page_size_in_bytes, bool preload);
};
