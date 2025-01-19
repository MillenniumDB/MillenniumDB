/******************************************************************************
******************************************************************************/

#pragma once

#include <filesystem>
#include <shared_mutex>

#include <boost/unordered/unordered_map.hpp>

class TensorStore;

class TensorStoreManager {
public:
    static constexpr uint64_t DEFAULT_TENSOR_BUFFER_SIZE = 1024 * 1024 * 1024; // 1 GB

    static constexpr char TENSOR_STORES_DIR[] = "tensor_stores";

    ~TensorStoreManager();

    static void init(const std::string& db_directory, uint64_t vtensor_frame_pool_size_in_bytes);

    // Returns true if the tensor store was found
    bool get_tensor_store(const std::string& name, TensorStore** tensor_store) const;

    void create_tensor_store(const std::string& name, uint64_t tensors_dim);

    std::vector<std::string> get_tensor_store_names() const {
        std::vector<std::string> names;
        for (const auto& [name, _] : name2tensor_store) {
            names.emplace_back(name);
        }

        return names;
    }

private:
    const std::filesystem::path db_directory;
    const uint64_t vtensor_frame_pool_size_in_bytes;

    // Prevents concurrent access to name2tensor_store
    mutable std::shared_mutex name2tensor_store_mutex;

    boost::unordered_map<std::string, std::unique_ptr<TensorStore>> name2tensor_store;

    explicit TensorStoreManager(const std::string& db_directory, uint64_t vtensor_frame_pool_size_in_bytes);
};

extern TensorStoreManager& tensor_store_manager;