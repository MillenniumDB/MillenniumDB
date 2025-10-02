#include "strings_hash.h"

#include <cassert>

#include "storage/index/hash/strings_hash/strings_hash_bucket.h"
#include "storage/index/hash/strings_hash/strings_hash_dir.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "third_party/hashes/hash_function_wrapper.h"

StringsHash::StringsHash(const std::string& filename) :
    dir_file_id(file_manager.get_file_id(filename + ".dir")),
    buckets_file_id(file_manager.get_file_id(filename + ".dat"))
{ }

uint64_t StringsHash::get_str_id(const char* bytes, uint64_t size) const
{
    StringsHashDir dir(dir_file_id);

    uint64_t hash = HashFunctionWrapper(bytes, size);

    // global_depth must be <= 64
    uint64_t mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64ULL - dir.global_depth());
    uint64_t suffix = hash & mask;

    auto bucket_number = dir.get_bucket_number(suffix);

    auto& bucket_page = buffer_manager.get_page_readonly(buckets_file_id, bucket_number);
    StringsHashBucket bucket(bucket_page);

    return bucket.get_id(bytes, size, hash);
}

void StringsHash::create_str_id(const char* bytes, uint64_t size, uint64_t new_id)
{
    uint64_t hash = HashFunctionWrapper(bytes, size);

    StringsHashDir dir(dir_file_id);

    // After a bucket split, need to try insert again.
    while (true) {
        auto global_mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64 - dir.global_depth());
        auto global_suffix = hash & global_mask;
        auto bucket_number = dir.get_bucket_number(global_suffix);

        auto& bucket_page = buffer_manager.get_page_editable(buckets_file_id, bucket_number);
        StringsHashBucket bucket(bucket_page);

        if (*bucket.key_count < bucket.MAX_KEYS) {
            bucket.create_id(new_id, hash);
            return;
        } else {
            // split bucket
            auto new_bucket_number = dir.get_new_bucket_number();

            ++(*bucket.local_depth);
            auto& new_bucket_page = buffer_manager.append_page(buckets_file_id);
            assert(new_bucket_number == new_bucket_page.get_page_number());
            StringsHashBucket new_bucket(new_bucket_page);
            *new_bucket.key_count = 0;
            *new_bucket.local_depth = *bucket.local_depth;

            auto new_mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64 - (*bucket.local_depth));
            auto new_suffix = (hash & new_mask) | (1ULL << (*bucket.local_depth - 1));

            if (*bucket.local_depth <= dir.global_depth()) {
                // update dirs having `new_suffix`, point them to the new_bucket
                auto update_dir_count = 1ULL << (dir.global_depth() - *bucket.local_depth);
                for (uint_fast32_t i = 0; i < update_dir_count; ++i) {
                    auto new_dir_pos = new_suffix | (i << *bucket.local_depth);
                    dir.set_bucket_number(new_dir_pos, new_bucket_number);
                }
            } else { // *bucket.local_depth == global_depth + 1
                dir.duplicate();
                dir.set_bucket_number(new_suffix, new_bucket_number);
            }
            bucket.redistribute(new_bucket, new_mask, new_suffix);
            assert(
                *bucket.key_count + *new_bucket.key_count == StringsHashBucket::MAX_KEYS
                && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split"
            );
        }
    }
}
