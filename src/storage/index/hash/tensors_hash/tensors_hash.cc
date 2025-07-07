#include "tensors_hash.h"

#include "storage/index/hash/tensors_hash/tensors_hash_bucket.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "third_party/hashes/hash_function_wrapper.h"

TensorsHash::TensorsHash() :
    buckets_file_id { file_manager.get_file_id(BUCKETS_FILENAME) }
{
    const auto file_path = file_manager.get_file_path(DIR_FILENAME);

    dir_file.open(file_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!dir_file.good()) {
        throw std::runtime_error("Could not open tensors directory");
    }

#ifndef NDEBUG
    dir_file.seekg(0, dir_file.end);
    assert(dir_file.tellg() != 0 && "tensors directory cannot be empty");
    dir_file.seekg(0, dir_file.beg);
#endif

    dir_file.read(reinterpret_cast<char*>(&global_depth), sizeof(global_depth));
    dir_file.read(reinterpret_cast<char*>(&total_pages), sizeof(total_pages));

    const auto dir_size = get_dir_size();
    dir = new uint32_t[dir_size];
    dir_file.read(reinterpret_cast<char*>(dir), sizeof(uint32_t) * dir_size);

    if (!dir_file.good()) {
        throw std::runtime_error("Error reading tensors directory");
    }
}

TensorsHash::~TensorsHash()
{
    if (dir_modified) {
        dir_file.seekg(0, dir_file.beg);
        dir_file.write(reinterpret_cast<const char*>(&global_depth), sizeof(global_depth));
        dir_file.write(reinterpret_cast<const char*>(&total_pages), sizeof(total_pages));

        const auto dir_size = get_dir_size();
        dir_file.write(reinterpret_cast<const char*>(dir), sizeof(uint32_t) * dir_size);
        assert(dir_file.good() && "error writing dir_file");
    }
    delete[](dir);
    dir_file.close();
}

uint64_t TensorsHash::get_bytes_id(const char* bytes, uint64_t num_bytes) const
{
    const uint64_t hash = HashFunctionWrapper(bytes, num_bytes);
    const uint64_t global_mask = get_global_mask();
    const uint64_t global_suffix = hash & global_mask;

    const auto bucket_number = dir[global_suffix];
    auto& bucket_page = buffer_manager.get_unversioned_page(buckets_file_id, bucket_number);
    TensorsHashBucket bucket(bucket_page);

    return bucket.get_id(bytes, num_bytes, hash);
}

// must be called only if tensor does not exist
void TensorsHash::create_bytes_id(const char* bytes, uint64_t num_bytes, uint64_t new_id)
{
    const uint64_t hash = HashFunctionWrapper(bytes, num_bytes);

    // after a bucket split, need to try insert again
    while (true) {
        const uint64_t global_mask = get_global_mask();
        const uint64_t global_suffix = hash & global_mask;
        const auto bucket_number = dir[global_suffix];

        auto& bucket_page = buffer_manager.get_unversioned_page(buckets_file_id, bucket_number);
        TensorsHashBucket bucket(bucket_page);

        if (*bucket.key_count < TensorsHashBucket::MAX_KEYS) {
            bucket.create_id(new_id, hash);
            return;
        }

        // split bucket
        dir_modified = true;
        const auto new_bucket_number = total_pages;
        ++total_pages;

        ++(*bucket.local_depth);
        auto& new_bucket_page = buffer_manager.append_unversioned_page(buckets_file_id);
        assert(new_bucket_number == new_bucket_page.get_page_number());
        TensorsHashBucket new_bucket(new_bucket_page);
        *new_bucket.key_count = 0;
        *new_bucket.local_depth = *bucket.local_depth;

        const uint64_t new_local_mask = get_local_mask(*bucket.local_depth);
        const uint64_t new_local_suffix = (hash & new_local_mask) | (1ULL << (*bucket.local_depth - 1));

        if (*bucket.local_depth <= global_depth) {
            const uint64_t update_dir_count = 1ULL << (global_depth - (*bucket.local_depth));
            for (std::size_t i = 0; i < update_dir_count; ++i) {
                const uint64_t new_dir_pos = new_local_suffix | (i << *bucket.local_depth);
                dir[new_dir_pos] = new_bucket_number;
            }
        } else {
            duplicate_dir();
            dir[new_local_suffix] = new_bucket_number;
        }
        bucket.redistribute(new_bucket, new_local_mask, new_local_suffix);

        assert(
            *bucket.key_count + *new_bucket.key_count == TensorsHashBucket::MAX_KEYS
            && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split"
        );
    }
}

void TensorsHash::duplicate_dir()
{
    const auto old_dir_size = get_dir_size();
    ++global_depth;
    const auto new_dir_size = get_dir_size();
    auto new_dir = new uint32_t[new_dir_size];

    std::memcpy(new_dir, dir, sizeof(uint32_t) * old_dir_size);

    std::memcpy(&new_dir[old_dir_size], dir, sizeof(uint32_t) * old_dir_size); // unnecessary?

    delete[](dir);
    dir = new_dir;
    dir_modified = true;
}
