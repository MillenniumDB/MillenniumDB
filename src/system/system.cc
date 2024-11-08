#include "system.h"

#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

System::System(
        const std::string& db_folder,
        uint64_t str_initial_populate_size,
        uint64_t shared_buffer_size,
        uint64_t private_buffer_size,
        uint64_t str_hash_buffer_size,
        uint64_t workers)
{
    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_size, private_buffer_size, str_hash_buffer_size, workers);
    PathManager::init(workers);
    StringManager::init(str_initial_populate_size);
    TmpManager::init(workers);
}

System::~System() {
    tmp_manager.~TmpManager();
    string_manager.~StringManager();
    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}
