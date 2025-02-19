/******************************************************************************
******************************************************************************/

#pragma once

#include <shared_mutex>

#include <boost/unordered/unordered_map.hpp>

#include "tensor_store.h"

class TensorStoreManager {
public:
    static constexpr uint64_t DEFAULT_TENSOR_BUFFER_SIZE = 1024 * 1024 * 1024; // 1 GB

    static constexpr char TENSOR_STORES_DIR[] = "tensor_stores";

    static uint64_t tensor_buffer_size_bytes;

    struct TensorStoreMetadata {
        uint64_t tensors_dim;

        friend std::ostream& operator<<(std::ostream& os, const TensorStoreMetadata& metadata)
        {
            os << "{\"tensors_dim\": " << metadata.tensors_dim << '}';
            return os;
        }
    };

    // Initialize the tensor store manager
    void init();

    void load_tensor_store(const std::string& name, const TensorStoreMetadata& metadata);

    // Returns true if the tensor store was found
    bool get_tensor_store(const std::string& name, TensorStore** tensor_store);

    void create_tensor_store(const std::string& name, uint64_t tensors_dim);

    const boost::unordered_map<std::string, TensorStoreMetadata>& get_name2metadata() const
    {
        return name2metadata;
    }

    bool has_changes() const
    {
        return has_changes_;
    }

    std::vector<float> get_tensor(uint64_t external_id);

private:
    bool has_changes_ = false;

    // Prevents concurrent access to name2tensor_store
    std::shared_mutex name2tensor_store_mutex;

    boost::unordered_map<std::string, uint64_t> name2tensor_store_id;

    boost::unordered_map<std::string, std::unique_ptr<TensorStore>> name2tensor_store;

    boost::unordered_map<std::string, TensorStoreMetadata> name2metadata;
    // auto id = name2tensor_store_id["hola"];
    // tensor_stores[id];

    // std::vector<std::unique_ptr<TensorStore>> tensor_stores;

    // std::vector<TensorStoreMetadata> tensor_store_metadatas;
};
