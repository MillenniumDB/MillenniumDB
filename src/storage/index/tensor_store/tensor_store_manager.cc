#include "tensor_store_manager.h"

#include <mutex>
#include <filesystem>

#include "query/exceptions.h"
#include "system/file_manager.h"

namespace fs = std::filesystem;

// Set default tensor buffer size. It will be overriden at src/system/system.cc
uint64_t TensorStoreManager::tensor_buffer_size_bytes = DEFAULT_TENSOR_BUFFER_SIZE;

void TensorStoreManager::init()
{
    const auto tensor_stores_path = file_manager.get_file_path(TENSOR_STORES_DIR);
    if (!fs::is_directory(tensor_stores_path)) {
        // Tensor stores directory did not exist
        fs::create_directories(tensor_stores_path);
    }
}

void TensorStoreManager::load_tensor_store(const std::string& name, const TensorStoreMetadata& metadata)
{
    try {
        auto tensor_store = TensorStore::load(name, TensorStoreManager::tensor_buffer_size_bytes);
        name2tensor_store[name] = std::move(tensor_store);
        name2metadata[name] = metadata;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load TensorStore \"" + name + "\": " + e.what());
    } catch (...) {
        throw std::runtime_error("Failed to load TensorStore \"" + name + "\": Unknown error");
    }
}

bool TensorStoreManager::get_tensor_store(const std::string& tensor_store_name, TensorStore** tensor_store)

{
    std::shared_lock lck(name2tensor_store_mutex);

    auto it = name2tensor_store.find(tensor_store_name);
    if (it == name2tensor_store.end()) {
        return false;
    }

    // Useful when just want to check if the index exists
    if (tensor_store != nullptr) {
        *tensor_store = it->second.get();
    }

    return true;
}

void TensorStoreManager::create_tensor_store(const std::string& tensor_store_name, uint64_t tensors_dim)
{
    try {
        auto tensor_store = TensorStore::create(
            tensor_store_name,
            tensors_dim,
            TensorStoreManager::tensor_buffer_size_bytes
        );

        std::unique_lock lck(name2tensor_store_mutex);
        name2tensor_store[tensor_store_name] = std::move(tensor_store);
        name2metadata[tensor_store_name] = { tensors_dim };

        has_changes_ = true;
    } catch (const std::exception& e) {
        throw QueryExecutionException(
            "Failed to create TensorStore \"" + tensor_store_name + "\": " + e.what()
        );
    } catch (...) {
        throw QueryExecutionException(
            "Failed to create TensorStore \"" + tensor_store_name + "\": Unknown error"
        );
    }
}

// std::vector<float> get_tensor(uint64_t external_id) {
//     const uint16_t tensor_store_id = 0;
//     const uint64_t tensor_index = 0;

//     auto it = tensor_store_id2tensor_store.find(tensor_store_id);
//     if (it == tensor_store_id2tensor_store.end()) {
//         // TODO: error
//     }

//     const TensorId()
//     it->second->get()
// }