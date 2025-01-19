#pragma once

#include "graph_models/object_id.h"
#include "storage/file_id.h"

/**
 * On-disk hash table from ObjectId to a tensor index. This uses a bit shuffling function for ObjectId,
 * because a regular hash function is not collision free and would break the structure. It is assummed
 * that there will be a mutex over this structure.
 */
class Object2TensorVersionedHash {
public:
    struct DirectoryHeader {
        uint32_t global_depth;
        uint32_t total_pages;
        uint32_t num_entries;
    };

    static constexpr auto MIN_GLOBAL_DEPTH = 8;

    Object2TensorVersionedHash(FileId buckets_file_id, FileId dir_file_id);

    Object2TensorVersionedHash(const Object2TensorVersionedHash& other) = delete;
    Object2TensorVersionedHash& operator=(const Object2TensorVersionedHash& other) = delete;
    Object2TensorVersionedHash(Object2TensorVersionedHash&& other) = delete;
    Object2TensorVersionedHash& operator=(Object2TensorVersionedHash&& other) = delete;

    ~Object2TensorVersionedHash();

    bool contains(ObjectId object_id) const;

    // Write into value if there is an entry for object_id. Returns false if there is no such entry
    bool get(ObjectId object_id, uint32_t* tensor_index) const;

    // Insert a new entry into the hash table. Returns false if there is already an entry for object_id
    void insert(ObjectId object_id, uint32_t tensor_index);

    // Remove an entry from the hash table
    bool remove(ObjectId object_id);

    std::size_t size() const
    {
        return dir_header.num_entries;
    }

    std::size_t empty() const
    {
        return size() == 0;
    }

private:
    const FileId buckets_file_id;
    const FileId dir_file_id;

    DirectoryHeader dir_header;

    // Array of size 2^global_depth
    uint32_t* dir;

    bool num_entries_modified { false };
    bool directory_modified { false };

    void duplicate_dir();
};