#include "object2tensor_versioned_hash.h"

#include "misc/logger.h"
#include "object2tensor_versioned_hash_bucket.h"
#include "shuffle.h"
#include "system/buffer_manager.h"

Object2TensorVersionedHash::Object2TensorVersionedHash(FileId buckets_file_id_, FileId dir_file_id_) :
    buckets_file_id { buckets_file_id_ },
    dir_file_id { dir_file_id_ }
{
    auto read_res = pread(dir_file_id.id, &dir_header, sizeof(DirectoryHeader), 0);
    if (read_res == -1) {
        throw std::runtime_error("Could not read directory header");
    }

    const uint64_t dir_size = 1ULL << dir_header.global_depth;
    dir = new uint32_t[dir_size];
    read_res = pread(dir_file_id.id, &dir[0], sizeof(uint32_t) * dir_size, sizeof(DirectoryHeader));
    if (read_res == -1) {
        throw std::runtime_error("Could not read directory");
    }
}

Object2TensorVersionedHash::~Object2TensorVersionedHash()
{
    // Both conditions are useful to prevent writing the entire directory when only the number of entries has changed
    if (num_entries_modified) {
        // The number of entries has changed, update the header
        const auto write_res = pwrite(dir_file_id.id, &dir_header, sizeof(DirectoryHeader), 0);
        if (write_res == -1) {
            logger(Category::Error) << "Could not write hash directory header";
        }

        if (directory_modified) {
            // The directory has changed too, update it
            const uint64_t dir_size = 1ULL << dir_header.global_depth;
            const auto write_res = pwrite(
                dir_file_id.id,
                &dir[0],
                sizeof(uint32_t) * dir_size,
                sizeof(DirectoryHeader)
            );
            if (write_res == -1) {
                logger(Category::Error) << "Could not write hash directory";
            }
        }
    }

    delete[](dir);
}

bool Object2TensorVersionedHash::contains(ObjectId object_id) const
{
    const uint64_t hash = shuffle_hash(object_id);

    const auto global_mask = UINT64_MAX >> (64ULL - dir_header.global_depth);
    const auto global_suffix = hash & global_mask;

    const auto bucket_number = dir[global_suffix];
    auto& bucket_page = buffer_manager.get_page_readonly(buckets_file_id, bucket_number);
    Object2TensorVersionedHashBucket bucket(bucket_page);

    return bucket.contains(hash);
}

bool Object2TensorVersionedHash::get(ObjectId object_id, uint32_t* tensor_index) const
{
    const uint64_t hash = shuffle_hash(object_id);

    const auto global_mask = UINT64_MAX >> (64ULL - dir_header.global_depth);
    const auto global_suffix = hash & global_mask;

    const auto bucket_number = dir[global_suffix];
    auto& bucket_page = buffer_manager.get_page_readonly(buckets_file_id, bucket_number);
    Object2TensorVersionedHashBucket bucket(bucket_page);

    return bucket.get(hash, tensor_index);
}

void Object2TensorVersionedHash::insert(ObjectId object_id, uint32_t tensor_index)
{
    const uint64_t hash = shuffle_hash(object_id);

    // After a bucket split, need to try insert again
    while (true) {
        const auto global_mask = UINT64_MAX >> (64ULL - dir_header.global_depth);
        const auto global_suffix = hash & global_mask;

        const auto bucket_number = dir[global_suffix];
        auto& bucket_page = buffer_manager.get_page_editable(buckets_file_id, bucket_number);
        Object2TensorVersionedHashBucket bucket(bucket_page);

        if (bucket.header->num_entries < Object2TensorVersionedHashBucket::MAX_ENTRIES) {
            // New entry fits in bucket
            bucket.insert(hash, tensor_index);
            (++dir_header.num_entries);
            num_entries_modified = true;
            return;
        }

        // Split bucket
        directory_modified = true;
        const auto new_bucket_number = dir_header.total_pages;
        ++(dir_header.total_pages);

        ++(bucket.header->local_depth);
        auto& new_bucket_page = buffer_manager.append_vpage(buckets_file_id);
        assert(new_bucket_number == new_bucket_page.get_page_number());
        Object2TensorVersionedHashBucket new_bucket(new_bucket_page);
        new_bucket.header->num_entries = 0;
        new_bucket.header->local_depth = bucket.header->local_depth;

        const auto split_mask = UINT64_MAX >> (64ULL - bucket.header->local_depth);
        const auto split_suffix = (hash & split_mask) | (1ULL << (bucket.header->local_depth - 1));

        if (bucket.header->local_depth <= dir_header.global_depth) {
            // Update directories that have the `split_suffix`, point them to the new_bucket
            const auto update_dir_count = 1ULL << (dir_header.global_depth - bucket.header->local_depth);
            for (uint32_t i = 0; i < update_dir_count; ++i) {
                const auto new_dir_pos = split_suffix | (i << bucket.header->local_depth);
                dir[new_dir_pos] = new_bucket_number;
            }
        } else { // *bucket.local_depth == global_depth + 1
            duplicate_dir();
            dir[split_suffix] = new_bucket_number;
        }

        bucket.redistribute(new_bucket, split_mask, split_suffix);
        assert(
            bucket.header->num_entries + new_bucket.header->num_entries == bucket.MAX_ENTRIES
            && "Redistribution failed"
        );
    }
}

bool Object2TensorVersionedHash::remove(ObjectId object_id)
{
    const uint64_t hash = shuffle_hash(object_id);

    const auto global_mask = UINT64_MAX >> (64ULL - dir_header.global_depth);
    const auto global_suffix = hash & global_mask;

    const auto bucket_number = dir[global_suffix];
    auto& bucket_page = buffer_manager.get_page_editable(buckets_file_id, bucket_number);
    Object2TensorVersionedHashBucket bucket(bucket_page);

    const bool removed = bucket.remove(hash);
    if (removed) {
        --(dir_header.num_entries);
        num_entries_modified = true;
    }
    return removed;
}

void Object2TensorVersionedHash::duplicate_dir()
{
    const auto old_dir_size = 1ULL << dir_header.global_depth;

    ++(dir_header.global_depth);
    const auto new_dir_size = 1ULL << dir_header.global_depth;
    auto new_dir = new uint32_t[new_dir_size];

    std::memcpy(new_dir, dir, sizeof(uint32_t) * old_dir_size);
    std::memcpy(&new_dir[old_dir_size], dir, sizeof(uint32_t) * old_dir_size);

    delete[](dir);
    dir = new_dir;
}
