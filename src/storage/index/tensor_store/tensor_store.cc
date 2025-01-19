#include "tensor_store.h"

#include <fcntl.h>
#include <sys/stat.h>

#include "misc/fatal_error.h"
#include "object2tensor_versioned_hash/object2tensor_versioned_hash_bucket.h"
#include "query/query_context.h"
#include "system/file_manager.h"
#include "tensor_store_manager.h"

namespace fs = std::filesystem;

void TensorStore::create(
    const fs::path& db_directory,
    const std::string& tensor_store_name,
    uint64_t tensors_dim
)
{
    if (tensors_dim == 0) {
        throw std::runtime_error("Tensors dimension cannot be 0");
    }

    const auto relative_base_path = fs::path(TensorStoreManager::TENSOR_STORES_DIR) / tensor_store_name;
    const auto absolute_base_path = db_directory / relative_base_path;

    if (!fs::create_directories(absolute_base_path)) {
        throw std::runtime_error("Could not create directories: " + absolute_base_path.string());
    }

    // Tensors File
    const auto tensors_file_id = file_manager.get_file_id(relative_base_path / TENSORS_FILENAME);

    TensorStore::TensorsHeader tensors_header {};
    tensors_header.tensors_dim = tensors_dim;
    auto write_res = pwrite(tensors_file_id.id, &tensors_header, sizeof(TensorStore::TensorsHeader), 0);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into file");
    }

    // Tombstones File
    auto tombstones_file_id = file_manager.get_file_id(relative_base_path / TOMBSTONES_FILENAME);
    TombstoneStackType::create(tombstones_file_id);

    // Object2Tensor Hash
    const auto hash_buckets_file_id = file_manager.get_file_id(relative_base_path / HASH_BUCKETS_FILENAME);
    const auto hash_dir_file_id = file_manager.get_file_id(relative_base_path / HASH_DIRECTORY_FILENAME);

    const uint64_t dir_size = 1ULL << Object2TensorVersionedHash::MIN_GLOBAL_DEPTH;
    auto dir = new uint32_t[dir_size];
    // Write empty buckets
    char empty_page[VPage::SIZE];
    auto empty_page_header = reinterpret_cast<Object2TensorVersionedHashBucket::Header*>(&empty_page[0]);
    empty_page_header->num_entries = 0;
    empty_page_header->local_depth = Object2TensorVersionedHash::MIN_GLOBAL_DEPTH;
    for (uint64_t i = 0; i < dir_size; i++) {
        // TODO: Could this be done with a single write call instead?
        const auto write_res = pwrite(hash_buckets_file_id.id, &empty_page[0], VPage::SIZE, i * VPage::SIZE);
        if (write_res == -1) {
            throw std::runtime_error("Could not write bucket #" + std::to_string(i) + " to buckets file");
        }
        dir[i] = i;
    }
    // Write directory
    Object2TensorVersionedHash::DirectoryHeader dir_header;
    dir_header.global_depth = Object2TensorVersionedHash::MIN_GLOBAL_DEPTH;
    dir_header.total_pages = dir_size;
    dir_header.num_entries = 0;
    write_res = pwrite(
        hash_dir_file_id.id,
        &dir_header,
        sizeof(Object2TensorVersionedHash::DirectoryHeader),
        0
    );
    if (write_res == -1) {
        throw std::runtime_error("Could not write hash directory header");
    }
    write_res = pwrite(
        hash_dir_file_id.id,
        &dir[0],
        sizeof(uint32_t) * dir_size,
        sizeof(Object2TensorVersionedHash::DirectoryHeader)
    );
    if (write_res == -1) {
        throw std::runtime_error("Could not write hash directory");
    }

    delete[](dir);
}

std::unique_ptr<TensorStore> TensorStore::load(
    const fs::path& db_directory,
    const std::string& tensor_store_name,
    uint64_t vtensor_frame_pool_size_in_bytes
)
{
    const auto relative_base_path = fs::path(TensorStoreManager::TENSOR_STORES_DIR) / tensor_store_name;
    const auto absolute_base_path = db_directory / relative_base_path;

    // Tensors File
    // Read header as is needed for TensorStore initialization
    const auto tensors_file_id = file_manager.get_file_id(relative_base_path / TENSORS_FILENAME);
    TensorStore::TensorsHeader tensors_header {};
    const auto read_res = pread(tensors_file_id.id, &tensors_header, sizeof(TensorStore::TensorsHeader), 0);
    if (read_res == -1) {
        throw std::runtime_error("Could not read tensors file");
    }

    // Tombstones File
    const auto tombstones_file_id = file_manager.get_file_id(relative_base_path / TOMBSTONES_FILENAME);

    // ObjectId2TensorId
    const auto hash_buckets_file_id = file_manager.get_file_id(relative_base_path / HASH_BUCKETS_FILENAME);
    const auto hash_dir_file_id = file_manager.get_file_id(relative_base_path / HASH_DIRECTORY_FILENAME);

    return std::unique_ptr<TensorStore>(new TensorStore(
        tensor_store_name,
        tensors_file_id,
        tombstones_file_id,
        hash_buckets_file_id,
        hash_dir_file_id,
        tensors_header,
        vtensor_frame_pool_size_in_bytes
    ));
}

TensorStore::TensorStore(
    const std::string& tensor_store_name_,
    FileId tensors_file_id_,
    FileId tombstones_file_id_,
    FileId hash_buckets_file_id_,
    FileId hash_dir_file_id_,
    TensorsHeader tensors_header_,
    uint64_t vtensor_frame_pool_size_in_bytes
) :
    tensor_store_name { tensor_store_name_ },
    tensors_file_id { tensors_file_id_ },
    object2tensor_index { hash_buckets_file_id_, hash_dir_file_id_ },
    tombstones_stack { tombstones_file_id_ },
    tensors_header { tensors_header_ },
    // Plus one to prevent zero
    vtensor_pool_size {
        1 + vtensor_frame_pool_size_in_bytes / (sizeof(VTensor) + sizeof(float) * tensors_header.tensors_dim)
    },
    vtensor_pool { new VTensor[vtensor_pool_size] },
    vtensor_data { new float[tensors_header.tensors_dim * vtensor_pool_size]

    }
{
    if (vtensor_data == nullptr || vtensor_pool == nullptr) {
        FATAL_ERROR("Could not allocate versioned tensor frame buffers, try using a smaller size");
    }

    for (uint64_t i = 0; i < vtensor_pool_size; ++i) {
        vtensor_pool[i].set_tensor(&vtensor_data[tensors_header.tensors_dim * i]);
    }

    vtensor_map.reserve(vtensor_pool_size);
}

TensorStore::~TensorStore()
{
    flush();
    delete[] vtensor_pool;
    delete[] vtensor_data;
}

bool TensorStore::get(ObjectId object_id, VTensor** vtensor)
{
    TensorId tensor_id(tensors_file_id, 0);

    std::shared_lock lck(object2tensor_index_mutex);
    const bool found = object2tensor_index.get(object_id, &tensor_id.tensor_index);
    if (!found) {
        return false;
    }

    *vtensor = &get_vtensor_readonly(tensor_id);
    return true;
}

bool TensorStore::insert(ObjectId object_id, const float* tensor)
{
    std::unique_lock lck(object2tensor_index_mutex);

    TensorId tensor_id(tensors_file_id, 0);
    const bool found = object2tensor_index.get(object_id, &tensor_id.tensor_index);
    if (found) {
        // Existing ObjectId, just overwrite its tensor
        auto& vtensor = get_vtensor_editable(tensor_id);
        memcpy(vtensor.tensor, tensor, sizeof(float) * tensors_dim());
        vtensor.unpin();
        return false;
    }

    // New ObjectId
    if (tombstones_stack.empty()) {
        // Append a new tensor
        auto& vtensor = append_vtensor();
        memcpy(vtensor.tensor, tensor, sizeof(float) * tensors_dim());
        vtensor.unpin();
        object2tensor_index.insert(object_id, vtensor.tensor_id.tensor_index);
    } else {
        // Reuse a tombstone
        const auto available_tensor_index = tombstones_stack.pop();
        const auto available_tensor_id = TensorId(tensors_file_id, available_tensor_index);
        auto& vtensor = get_vtensor_editable(available_tensor_id);
        memcpy(vtensor.tensor, tensor, sizeof(float) * tensors_dim());
        vtensor.unpin();
        object2tensor_index.insert(object_id, available_tensor_id.tensor_index);
    }

    return true;
}

bool TensorStore::remove(ObjectId object_id)
{
    std::unique_lock lck(object2tensor_index_mutex);

    TensorId tensor_id(tensors_file_id, 0);
    auto found = object2tensor_index.get(object_id, &tensor_id.tensor_index);
    if (!found) {
        return false;
    }

    tombstones_stack.push(tensor_id.tensor_index);
    object2tensor_index.remove(object_id);

    return true;
}

uint64_t TensorStore::tensors_dim() const noexcept
{
    return tensors_header.tensors_dim;
}

const std::string& TensorStore::name() const noexcept
{
    return tensor_store_name;
}

std::size_t TensorStore::size() const
{
    return object2tensor_index.size();
}

bool TensorStore::empty() const
{
    return object2tensor_index.empty();
}

VTensor& TensorStore::get_vtensor_readonly(TensorId tensor_id) noexcept
{
    const auto start_version = get_query_ctx().start_version;
    const auto result_version = get_query_ctx().result_version;

    vtensor_mutex.lock();
    auto it = vtensor_map.find(tensor_id);

    if (it == vtensor_map.end()) {
        auto& vtensor = get_vtensor_available();

        vtensor.reassign(tensor_id);
        vtensor.version_number = start_version;
        vtensor.prev_version = nullptr;
        vtensor.next_version = nullptr;
        vtensor_map.insert({ tensor_id, &vtensor });

        read_existing_vtensor(tensor_id, vtensor.tensor);
        vtensor_mutex.unlock();

        return vtensor;
    } else {
        VTensor* vtensor = it->second;

        while (vtensor->next_version != nullptr && vtensor->next_version->version_number <= result_version) {
            vtensor = vtensor->next_version;
        }

        assert(vtensor->version_number <= result_version);

        vtensor->pin();
        vtensor_mutex.unlock();

        return *vtensor;
    }
}

VTensor& TensorStore::get_vtensor_editable(TensorId tensor_id) noexcept
{
    const auto start_version = get_query_ctx().start_version;
    const auto result_version = get_query_ctx().result_version;

    std::lock_guard<std::mutex> lck(vtensor_mutex);
    auto it = vtensor_map.find(tensor_id);

    if (it == vtensor_map.end()) {
        auto& new_vtensor = get_vtensor_available();
        new_vtensor.reassign(tensor_id); // this will pin the tensor

        auto& old_vtensor = get_vtensor_available();
        old_vtensor.reassign_tensor_id(tensor_id);
        old_vtensor.version_number = start_version;
        old_vtensor.prev_version = nullptr;
        old_vtensor.next_version = &new_vtensor;

        new_vtensor.version_number = result_version;
        new_vtensor.prev_version = &old_vtensor;
        new_vtensor.next_version = nullptr;
        new_vtensor.dirty = true;

        vtensor_map.insert({ tensor_id, &old_vtensor });

        read_existing_vtensor(tensor_id, old_vtensor.tensor);

        // NOTE: The current implementation does not need an editable copy of the old version, if needed
        // activate this std::memcpy(new_vtensor.tensor, old_vtensor.tensor, sizeof(float) * tensors_dim());

        return new_vtensor;
    } else {
        // tensor is in the buffer, search for the corresponding version
        VTensor* vtensor_head = it->second;

        VTensor* vtensor_tail = vtensor_head;
        while (vtensor_tail->next_version != nullptr) {
            vtensor_tail = vtensor_tail->next_version;
        }

        assert(vtensor_tail->version_number <= result_version);

        vtensor_head->pin();
        vtensor_tail->pin();

        if (vtensor_tail->version_number != result_version) {
            auto& new_vtensor = get_vtensor_available();

            vtensor_head->unpin();
            vtensor_tail->unpin();

            new_vtensor.reassign(tensor_id);

            vtensor_tail->next_version = &new_vtensor;

            new_vtensor.version_number = result_version;
            new_vtensor.prev_version = vtensor_tail;
            new_vtensor.next_version = nullptr;
            new_vtensor.dirty = true;

            // NOTE: The current implementation does not need an editable copy of the old version, if needed
            // activate this std::memcpy(new_vtensor.tensor, vtensor->tensor, sizeof(float) * tensors_dim());
            return new_vtensor;
        } else {
            // vtensor_tail is already pinned
            vtensor_head->unpin();
            return *vtensor_tail;
        }
    }
}

VTensor& TensorStore::append_vtensor()
{
    const auto result_version = get_query_ctx().result_version;

    std::lock_guard<std::mutex> lck(vtensor_mutex);

    auto& new_vtensor = get_vtensor_available();

    const auto fd = tensors_file_id.id;
    // TODO: Check
    const uint32_t new_vtensor_index = (lseek(fd, 0, SEEK_END) - sizeof(TensorsHeader))
                                     / (sizeof(float) * tensors_dim());

    memset(new_vtensor.tensor, 0, sizeof(float) * tensors_dim());
    const auto write_res = write(fd, new_vtensor.tensor, sizeof(float) * tensors_dim());

    if (write_res == -1) {
        throw std::runtime_error("Could not write into file");
    }

    const TensorId new_tensor_id(tensors_file_id, new_vtensor_index);
    new_vtensor.reassign(new_tensor_id);

    new_vtensor.version_number = result_version;
    new_vtensor.prev_version = nullptr;
    new_vtensor.next_version = nullptr;

    new_vtensor.dirty = true;

    vtensor_map.insert({ new_tensor_id, &new_vtensor });

    return new_vtensor;
}

void TensorStore::flush()
{
    assert(vtensor_pool != nullptr);

    for (uint64_t i = 0; i < vtensor_pool_size; ++i) {
        VTensor& vtensor = vtensor_pool[i];
        assert(vtensor.pins == 0);
        if (vtensor.dirty && vtensor.next_version == nullptr) {
            flush_vtensor(vtensor);
        }
    }
}

void TensorStore::flush_vtensor(VTensor& vtensor)
{
    const auto fd = vtensor.tensor_id.file_id.id;
    const auto offset = sizeof(TensorsHeader)
                      + sizeof(float) * tensors_dim() * vtensor.tensor_id.tensor_index;
    const auto write_res = pwrite(fd, vtensor.get_tensor(), sizeof(float) * tensors_dim(), offset);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into file");
    }
    vtensor.dirty = false;
}

void TensorStore::read_existing_vtensor(TensorId tensor_id, float* tensor)
{
    const auto fd = tensor_id.file_id.id;

#ifndef NDEBUG
    struct stat buf;
    fstat(fd, &buf);
    const auto file_size = buf.st_size;
    assert(tensor_id.tensor_index < (file_size - sizeof(TensorsHeader)) / (sizeof(float) * tensors_dim()));
#endif

    const auto read_res = pread(
        fd,
        tensor,
        sizeof(float) * tensors_dim(),
        sizeof(TensorsHeader) + sizeof(float) * tensors_dim() * tensor_id.tensor_index
    );
    if (read_res == -1) {
        throw std::runtime_error("Could not read file tensor");
    }
}

VTensor& TensorStore::get_vtensor_available()
{
    while (true) {
        ++vtensor_clock;
        vtensor_clock = vtensor_clock < vtensor_pool_size ? vtensor_clock : 0;

        auto& vtensor = vtensor_pool[vtensor_clock];

        if (vtensor.pins != 0) {
            continue;
        }

        if (vtensor.second_chance) {
            vtensor.second_chance = false;
            continue;
        }

        if (vtensor.prev_version == nullptr && vtensor.next_version == nullptr) {
            if (vtensor.tensor_id.file_id.id != FileId::UNASSIGNED) {
                vtensor_map.erase(vtensor.tensor_id);
            }

            if (vtensor.dirty) {
                // TODO: reduce counter of version writing pending for page version
                // (we know this is the last version and there is no previous version)
                flush_vtensor(vtensor);
            }

            return vtensor;
        }

        bool version_not_being_used = buffer_manager.version_not_being_used(vtensor.version_number);

        if (version_not_being_used) {
            if (vtensor.dirty) {
                // TODO: reduce counter of version writing pending for page version
                // (we know this is the last version and there is no previous version)
            }

            if (vtensor.prev_version != nullptr) {
                vtensor.prev_version->next_version = vtensor.next_version;
            } else {
                // vtensor is the first in the linked list
                // We know vtensor.next_version != nullptr
                // If it is the first version and there are more versions, we need to edit vtensor_map to
                // point to the new oldest version
                if (vtensor.tensor_id.file_id.id != FileId::UNASSIGNED) {
                    auto it2 = vtensor_map.find(vtensor.tensor_id);
                    assert(it2 != vtensor_map.end());
                    vtensor_map.erase(it2);
                    vtensor_map.insert({ vtensor.tensor_id, vtensor.next_version });
                }
            }

            if (vtensor.next_version != nullptr) {
                vtensor.next_version->prev_version = vtensor.prev_version;
                vtensor.dirty = false;
            } else {
                // vtensor is the last in the linked list
                // flush when dirty and there is no next version
                if (vtensor.dirty) {
                    flush_vtensor(vtensor);

                    // We know vtensor.prev_version != nullptr
                    VTensor* curr_prev = vtensor.prev_version;
                    // All previous dirty vtensor_map are no longer dirty because a newer version was written
                    // to disk, and we only have one update at a time, so previous versions must be ended
                    do {
                        if (curr_prev->dirty) {
                            curr_prev->dirty = false;
                        }

                        curr_prev = curr_prev->prev_version;
                    } while (curr_prev != nullptr);
                }
            }

            return vtensor;
        }
    }

    return vtensor_pool[vtensor_clock];
}

std::ostream& operator<<(std::ostream& os, const TensorStore& tensor_store)
{
    os << "TensorStore(tensors_dim=" << tensor_store.tensors_dim();
    os << ", size=" << tensor_store.size();
    os << ", vtensor_pool_size=" << tensor_store.vtensor_pool_size;
    return os << ")";
}