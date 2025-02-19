#include "system.h"

#include "storage/index/tensor_store/tensor_store_manager.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

System::System(
    const std::string& db_folder,
    uint64_t str_static_size,
    uint64_t str_dynamic_size,
    uint64_t shared_buffer_size,
    uint64_t private_buffer_size,
    uint64_t str_hash_buffer_size,
    uint64_t tensor_buffer_size_bytes,
    uint64_t workers
)
{
    TensorStoreManager::tensor_buffer_size_bytes = tensor_buffer_size_bytes;

    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_size, private_buffer_size, str_hash_buffer_size, workers);
    PathManager::init(workers);
    StringManager::init(str_static_size, str_dynamic_size);
    TmpManager::init(workers);
}

System::~System()
{
    tmp_manager.~TmpManager();
    string_manager.~StringManager();
    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}
