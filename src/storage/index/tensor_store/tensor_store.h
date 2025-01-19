#pragma once

#include <filesystem>
#include <mutex>
#include <shared_mutex>
#include <string>

#include <boost/unordered/unordered_flat_map.hpp>

#include "graph_models/object_id.h"
#include "object2tensor_versioned_hash/object2tensor_versioned_hash.h"
#include "storage/disk_int_stack.h"
#include "storage/file_id.h"
#include "tensor_id.h"
#include "versioned_tensor.h"

class TensorStore {
public:
    using TombstoneStackType = DiskIntStack<uint32_t>;

    struct ObjectIdHasher {
        uint64_t operator()(ObjectId key) const
        {
            return key.id;
        }
    };

    struct TensorsHeader {
        uint64_t tensors_dim;
    };

    static constexpr char TENSORS_FILENAME[] = "tensors.dat";
    static constexpr char TOMBSTONES_FILENAME[] = "tombstones.dat";
    static constexpr char HASH_BUCKETS_FILENAME[] = "hash.dat";
    static constexpr char HASH_DIRECTORY_FILENAME[] = "hash.dir";

    // Initialize a new tensor_store
    static void create(
        const std::filesystem::path& db_directory,
        const std::string& tensor_store_name,
        uint64_t tensors_dim
    );

    // Load an existing tensor_store with a limited frame pool size
    static std::unique_ptr<TensorStore> load(
        const std::filesystem::path& db_directory,
        const std::string& tensor_store_name,
        uint64_t vtensor_frame_pool_size_in_bytes
    );

    inline static void unpin(VTensor& vtensor)
    {
        vtensor.unpin();
    }

    TensorStore(const TensorStore&) = delete;
    TensorStore& operator=(const TensorStore&) = delete;
    // TensorStore(TensorStore&&) = delete;
    // TensorStore& operator=(TensorStore&&) = delete;

    ~TensorStore();

    // Returns true if found, false otherwise
    bool get(ObjectId object_id, VTensor** vtensor);

    // Returns true if new object_id was inserted, false if object_id already existed and was overwritten
    bool insert(ObjectId object_id, const float* tensor);

    // Returns true if object_id was removed, false if object_id was not found
    bool remove(ObjectId object_id);

    uint64_t tensors_dim() const noexcept;

    const std::string& name() const noexcept;

    std::size_t size() const;

    bool empty() const;

    friend std::ostream& operator<<(std::ostream& os, const TensorStore& tensor_store);

private:
    const std::string tensor_store_name;

    const FileId tensors_file_id;

    Object2TensorVersionedHash object2tensor_index;

    TombstoneStackType tombstones_stack;

    const TensorsHeader tensors_header;

    // Frames for tensors
    const uint64_t vtensor_pool_size;

    VTensor* const vtensor_pool;

    float* const vtensor_data;

    uint64_t vtensor_clock { 0 };

    // Prevent concurrent modifications in vtensor_map
    std::mutex vtensor_mutex;

    // Prevent concurrent modifications in object2tensor_index
    std::shared_mutex object2tensor_index_mutex;

    boost::unordered_flat_map<TensorId, VTensor*, TensorId::Hasher> vtensor_map;

    // Must only be called by load
    TensorStore(
        const std::string& tensor_store_name,
        FileId tensors_file_id,
        FileId tombstones_file_id,
        FileId hash_buckets_file_id,
        FileId hash_dir_file_id,
        TensorsHeader tensors_header,
        uint64_t vtensor_frame_pool_size_in_bytes
    );

    VTensor& get_vtensor_readonly(TensorId tensor_id) noexcept;

    VTensor& get_vtensor_editable(TensorId tensor_id) noexcept;

    VTensor& append_vtensor();

    // Write all dirty frames to disk
    void flush();

    // TODO: File manager
    inline void flush_vtensor(VTensor& vtensor);

    // TODO: File manager
    void read_existing_vtensor(TensorId tensor_id, float* tensor);

    VTensor& get_vtensor_available();
};
