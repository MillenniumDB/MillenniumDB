#include "strings_hash.h"

#include <cassert>

#include "query/exceptions.h"
#include "storage/index/hash/strings_hash/strings_hash_bucket.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "third_party/hashes/hash_function_wrapper.h"

StringsHash::StringsHash(const std::string& filename) :
    buckets_file_id(file_manager.get_file_id(filename + ".dat"))
{
    // auto file_path = file_manager.get_file_path(filename + ".dir");

    // dir_file.open(file_path, std::ios::in | std::ios::out | std::ios::binary);
    // if (dir_file.fail()) {
    //     throw std::runtime_error("Could not open file " + filename);
    // }

    // dir_file.seekg(0, dir_file.end);
    // if (dir_file.tellg() == 0) {
    //     throw LogicException("String Hash must exists and cannot be empty");
    // }

    // dir_file.seekg(0, dir_file.beg);
    // dir_file.read(reinterpret_cast<char*>(&global_depth), sizeof(global_depth));
    // dir_file.read(reinterpret_cast<char*>(&total_pages), sizeof(total_pages));

    // uint_fast32_t dir_size = 1ULL << global_depth;
    // dir = new uint32_t[dir_size];
    // for (uint_fast32_t i = 0; i < dir_size; ++i) {
    //     dir_file.read(reinterpret_cast<char*>(&dir[i]), sizeof(dir[i]));
    // }

    // // check eofbit/failbit/badbit to ensure the values were read correctly
    // if (!dir_file.good()) {
    //     throw LogicException("Error reading StringsHash directory. File may be corrupted.");
    // }
}

void StringsHash::duplicate_dir()
{
    uint64_t old_dir_size = 1ULL << global_depth();
    set_global_depth(global_depth() + 1);
    auto new_dir_size = 2 * old_dir_size;

    for (auto i = old_dir_size; i < new_dir_size; i++) {
        set_dir(i, get_dir(i - old_dir_size));
    }
}

void StringsHash::create_str_id(const char* bytes, uint64_t size, uint64_t new_id)
{
    uint64_t hash = HashFunctionWrapper(bytes, size);

    // After a bucket split, need to try insert again.
    while (true) {
        auto global_mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64 - global_depth());
        auto global_suffix = hash & global_mask;
        auto bucket_number = get_dir(global_suffix);

        auto& bucket_page = buffer_manager.get_page_readonly(buckets_file_id, bucket_number);
        StringsHashBucket bucket(bucket_page);

        if (*bucket.key_count < bucket.MAX_KEYS) {
            bucket.create_str_id(new_id, hash);
            return;
        } else {
            // split bucket
            bucket.upgrade_page_to_editable();

            auto new_bucket_number = total_pages();
            set_total_pages(new_bucket_number + 1);

            ++(*bucket.local_depth);
            auto& new_bucket_page = buffer_manager.append_page(buckets_file_id);
            assert(new_bucket_number == new_bucket_page.get_page_number());
            StringsHashBucket new_bucket(new_bucket_page);
            *new_bucket.key_count = 0;
            *new_bucket.local_depth = *bucket.local_depth;

            auto new_mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64 - (*bucket.local_depth));
            auto new_suffix = (hash & new_mask) | (1ULL << (*bucket.local_depth - 1));

            if (*bucket.local_depth <= global_depth()) {
                // update dirs having `new_suffix`, point them to the new_bucket
                auto update_dir_count = 1ULL << (global_depth() - (*bucket.local_depth));
                for (uint_fast32_t i = 0; i < update_dir_count; ++i) {
                    auto new_dir_pos = new_suffix | (i << *bucket.local_depth);
                    set_dir(new_dir_pos, new_bucket_number);
                }
            } else { // *bucket.local_depth == global_depth + 1
                duplicate_dir();
                set_dir(new_suffix, new_bucket_number);
            }
            bucket.redistribute(new_bucket, new_mask, new_suffix);
            assert(
                *bucket.key_count + *new_bucket.key_count == StringsHashBucket::MAX_KEYS
                && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split"
            );
        }
    }
}

uint64_t StringsHash::get_str_id(const char* bytes, uint64_t size) const
{
    uint64_t hash = HashFunctionWrapper(bytes, size);

    // global_depth must be <= 64
    uint64_t mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64ULL - global_depth());
    uint64_t suffix = hash & mask;

    auto bucket_number = get_dir(suffix);

    auto& bucket_page = buffer_manager.get_page_readonly(buckets_file_id, bucket_number);
    StringsHashBucket bucket(bucket_page);

    return bucket.get_id(bytes, size, hash);
}
