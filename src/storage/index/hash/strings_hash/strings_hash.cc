#include "strings_hash.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>

#include "base/exceptions.h"
#include "storage/file_manager.h"
#include "storage/index/hash/strings_hash/strings_hash_bucket.h"
#include "third_party/xxhash/xxhash.h"


StringsHash::StringsHash(const std::string& filename) :
    buckets_file_id (file_manager.get_file_id(filename + ".dat"))
{
    auto file_path = file_manager.get_file_path(filename + ".dir");

    dir_file.open(file_path, std::ios::in|std::ios::out|std::ios::binary);
     if (dir_file.fail()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    dir_file.seekg(0, dir_file.end);

    // If the file is not empty, read the values
    if (dir_file.tellg() != 0) {
        dir_file.seekg(0, dir_file.beg);

        dir_file.read(reinterpret_cast<char*>(&global_depth), sizeof(global_depth));

        uint_fast32_t dir_size = 1 << global_depth;
        dir = new uint_fast32_t[dir_size];
        for (uint_fast32_t i = 0; i < dir_size; ++i) {
            uint32_t tmp;
            dir_file.read(reinterpret_cast<char*>(&tmp), sizeof(tmp));
            dir[i] = tmp;
        }
        // check eofbit/failbit/badbit to ensure the values were read correctly
        if (!dir_file.good()) {
            throw LogicException("Error reading ObjectFile hash directory. File may be corrupted.");
        }
    } else {
        global_depth = DEFAULT_GLOBAL_DEPTH;
        uint_fast32_t dir_size = 1 << global_depth;
        dir = new uint_fast32_t[dir_size];
        for (uint_fast32_t i = 0; i < dir_size; ++i) {
            StringsHashBucket bucket(buckets_file_id, i);
            *bucket.key_count = 0;
            *bucket.local_depth = DEFAULT_GLOBAL_DEPTH;
            dir[i] = i;
            bucket.page.make_dirty();
        }
    }
}


StringsHash::~StringsHash() {
    dir_file.seekg(0, dir_file.beg);

    dir_file.write(reinterpret_cast<const char*>(&global_depth), sizeof(global_depth));

    uint_fast32_t dir_size = 1 << global_depth;
    for (uint64_t i = 0; i < dir_size; ++i) {
        uint32_t tmp = dir[i];
        dir_file.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
    }
}


void StringsHash::duplicate_dirs() {
    uint64_t old_dir_size = 1UL << global_depth;
    ++global_depth;
    auto new_dir_size = 1UL << global_depth;
    auto new_dir = new uint_fast32_t[new_dir_size];

    std::memcpy(
        new_dir,
        dir,
        old_dir_size * sizeof(uint_fast32_t)
    );

    std::memcpy(
        &new_dir[old_dir_size],
        dir,
        old_dir_size * sizeof(uint_fast32_t)
    );

    delete[](dir);
    dir = new_dir;
}


uint64_t StringsHash::get_or_create_str_id(const std::string& str) {
    uint64_t hash = XXH3_64bits(str.data(), str.length());

    // After a bucket split, need to try insert again.
    while (true) {
        // global_depth must be <= 64
        auto mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
        auto suffix = hash & mask;
        auto bucket_number = dir[suffix];
        auto bucket = StringsHashBucket(buckets_file_id, bucket_number);

        bool need_split;
        auto id = bucket.get_or_create_str_id(str, hash, &need_split);

        if (need_split) {
            if (*bucket.local_depth < global_depth) {
                auto new_bucket_number = bucket_number | (1 << (*bucket.local_depth));
                ++(*bucket.local_depth);
                auto new_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - (*bucket.local_depth));
                auto new_bucket = StringsHashBucket(buckets_file_id, new_bucket_number);
                *new_bucket.key_count = 0;
                (*new_bucket.local_depth) = (*bucket.local_depth);

                // redistribute keys between buckets `0|bucket_number` and `1|bucket_number`
                bucket.redistribute(new_bucket, new_mask, new_bucket_number);

                // update dirs having `new_bucket_number` suffix and point to the new_bucket
                auto update_dir_count = 1 << (global_depth - (*bucket.local_depth));
                for (auto i = 0; i < update_dir_count; ++i) {
                    dir[(i << (*bucket.local_depth)) | new_bucket_number] = new_bucket_number;
                }

                assert(*bucket.key_count + *new_bucket.key_count== StringsHashBucket::MAX_KEYS
                    && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split");

            } else {
                assert(suffix == bucket_number && "EXTENDIBLE HASH INCONSISTENCY: suffix != bucket_number");
                assert(*bucket.local_depth == global_depth && "EXTENDIBLE HASH INCONSISTENCY: *bucket.local_depth != global_depth");
                ++(*bucket.local_depth);

                auto new_bucket_number = bucket_number | (1 << global_depth);
                auto new_bucket = StringsHashBucket(buckets_file_id, new_bucket_number);
                *new_bucket.key_count = 0;
                *new_bucket.local_depth = *bucket.local_depth;

                duplicate_dirs();

                // redistribute keys between buckets `0|bucket_number` and `1|bucket_number`
                auto new_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
                bucket.redistribute(new_bucket, new_mask, new_bucket_number);

                // update dir for `1|bucket_number`
                dir[new_bucket_number] = new_bucket_number;

                assert(*bucket.key_count + *new_bucket.key_count== StringsHashBucket::MAX_KEYS
                    && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split");
            }
        } else {
            return id;
        }
    }
}


uint64_t StringsHash::get_str_id(const std::string& str) const {
    uint64_t hash = XXH3_64bits(str.data(), str.length());

    // After a bucket split, need to try insert again.
    while (true) {
        // global_depth must be <= 64
        auto mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
        auto suffix = hash & mask;
        auto bucket_number = dir[suffix];
        auto bucket = StringsHashBucket(buckets_file_id, bucket_number);

        return bucket.get_str_id(str, hash);
    }
}
