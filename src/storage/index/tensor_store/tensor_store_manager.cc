// TODO: Check locks
#include "tensor_store_manager.h"

#include "misc/logger.h"
#include "query/exceptions.h"
#include "tensor_store.h"

namespace fs = std::filesystem;

static typename std::aligned_storage<sizeof(TensorStoreManager), alignof(TensorStoreManager)>::type
    tensor_store_manager_buf;
TensorStoreManager& tensor_store_manager = reinterpret_cast<TensorStoreManager&>(tensor_store_manager_buf);

TensorStoreManager::~TensorStoreManager() = default;

void TensorStoreManager::init(const std::string& db_directory, uint64_t vtensor_frame_pool_size_in_bytes)
{
    new (&tensor_store_manager) TensorStoreManager(db_directory, vtensor_frame_pool_size_in_bytes);
}

TensorStoreManager::TensorStoreManager(
    const std::string& db_directory_,
    uint64_t vtensor_frame_pool_size_in_bytes_
) :
    db_directory { db_directory_ },
    vtensor_frame_pool_size_in_bytes { vtensor_frame_pool_size_in_bytes_ }
{
    const auto tensor_stores_path = db_directory / TENSOR_STORES_DIR;
    if (!fs::is_directory(tensor_stores_path)) {
        // Tensor stores directory did not exist, nothing to load
        fs::create_directories(tensor_stores_path);
        return;
    }

    auto it = fs::directory_iterator(tensor_stores_path);
    for (const auto& entry : it) {
        if (!entry.is_directory()) {
            // Ignore non-directories
            continue;
        }

        const auto tensor_store_name = entry.path().filename().string();
        try {
            auto tensor_store = TensorStore::load(
                db_directory,
                tensor_store_name,
                vtensor_frame_pool_size_in_bytes
            );
            name2tensor_store[tensor_store_name] = std::move(tensor_store);

            logger(Category::Info) << "Loaded TensorStore \"" << tensor_store_name
                                   << "\": " << *name2tensor_store[tensor_store_name];
        } catch (const std::exception& e) {
            logger(Category::Error) << "Load failed for TensorStore \"" << tensor_store_name
                                   << "\": " << e.what();
        } catch (...) {
            logger(Category::Error) << "Load failed for TensorStore \"" << tensor_store_name
                                   << "\": Unknown error";
        }
    }
}

bool TensorStoreManager::get_tensor_store(const std::string& tensor_store_name, TensorStore** tensor_store) const
{
    std::shared_lock lck(name2tensor_store_mutex);

    auto it = name2tensor_store.find(tensor_store_name);
    if (it == name2tensor_store.end()) {
        // Tensor store not found
        return false;
    }

    *tensor_store = it->second.get();
    return true;
}

void TensorStoreManager::create_tensor_store(const std::string& tensor_store_name, uint64_t tensors_dim)
{
    {
        std::shared_lock lck(name2tensor_store_mutex);
        auto it = name2tensor_store.find(tensor_store_name);
        if (it != name2tensor_store.end()) {
            throw QuerySemanticException("TensorStore \"" + tensor_store_name + "\" already exists");
        }
    }

    try {
        // TODO: Implement create-and-load?
        TensorStore::create(db_directory, tensor_store_name, tensors_dim);
        auto tensor_store = TensorStore::load(
            db_directory,
            tensor_store_name,
            vtensor_frame_pool_size_in_bytes
        );
        std::unique_lock lck(name2tensor_store_mutex);
        name2tensor_store[tensor_store_name] = std::move(tensor_store);
    } catch (const std::exception& e) {
        throw QueryExecutionException("Failed to create TensorStore \"" + tensor_store_name + "\": " + e.what());
    } catch (...) {
        throw QueryExecutionException("Failed to create TensorStore \"" + tensor_store_name + "\": Unknown error");
    }
}

