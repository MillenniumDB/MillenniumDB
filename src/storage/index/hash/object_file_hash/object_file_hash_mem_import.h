#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

#include "storage/index/hash/object_file_hash/object_file_hash.h"
#include "storage/index/hash/object_file_hash/object_file_hash_bucket.h"
#include "storage/page.h"
#include "third_party/murmur3/murmur3.h"

class ObjectFileHashMemImportBucket {
friend class ObjectFileHashMemImport;
public:
    ObjectFileHashMemImportBucket(char* page) :
        key_count   (reinterpret_cast<uint32_t*>(page)),
        local_depth (reinterpret_cast<uint32_t*>(page + sizeof(uint32_t))),
        hashes      (reinterpret_cast<uint64_t*>(page + 2*sizeof(uint32_t))),
        ids         (reinterpret_cast<uint64_t*>(page + sizeof(uint64_t) + sizeof(uint64_t)*ObjectFileHashBucket::MAX_KEYS)) { }

    void create_id(uint64_t new_id, const uint64_t hash, bool* const need_split)
    {
        if (*key_count == ObjectFileHashBucket::MAX_KEYS) {
            *need_split = true;
            return;
        }

        hashes[*key_count] = hash;
        ids[*key_count] = new_id;
        ++(*key_count);

        *need_split = false;
    }

private:
    uint32_t* const key_count;
    uint32_t* const local_depth; // used by object_file_hash
    uint64_t* const hashes;
    uint64_t* const ids;

    // TODO: duplicated code with object_file_hash, should be in one place
    void redistribute(ObjectFileHashMemImportBucket& other, const uint64_t mask, const uint64_t other_suffix) {
        uint32_t other_pos = 0;
        uint32_t this_pos = 0;

        for (size_t i = 0; i < *key_count; i++) {
            auto suffix = mask & hashes[i];

            if (suffix == other_suffix) {
                other.hashes[other_pos] = hashes[i];
                other.ids[other_pos] = ids[i];

                ++other_pos;
            } else {
                hashes[this_pos] = hashes[i];
                ids[this_pos] = ids[i];

                ++this_pos;
            }
        }
        *this->key_count = this_pos;
        *other.key_count = other_pos;
    }
};

class ObjectFileHashMemImport {
public:
    ObjectFileHashMemImport(const std::string& filename) {
        dir_file.open(filename + ".dir", std::ios::out|std::ios::binary);
        buckets_file.open(filename + ".dat", std::ios::out|std::ios::binary);

        global_depth = ObjectFileHash::DEFAULT_GLOBAL_DEPTH;
        uint_fast32_t dir_size = 1 << global_depth;
        dir = new uint_fast32_t[dir_size];
        for (uint_fast32_t i = 0; i < dir_size; ++i) {
            auto new_page = new char[Page::MDB_PAGE_SIZE];
            memset(new_page, 0, Page::MDB_PAGE_SIZE);
            pages.push_back(new_page);
            ObjectFileHashMemImportBucket bucket(new_page);
            *bucket.key_count = 0;
            *bucket.local_depth = ObjectFileHash::DEFAULT_GLOBAL_DEPTH;
            dir[i] = i;
        }
    }

    ~ObjectFileHashMemImport() {
        dir_file.seekg(0, dir_file.beg);
        buckets_file.seekg(0, buckets_file.beg);

        dir_file.write(reinterpret_cast<const char*>(&global_depth), sizeof(uint8_t));
        uint_fast32_t dir_size = 1 << global_depth;
        for (uint64_t i = 0; i < dir_size; ++i) {
            uint32_t tmp = dir[i];
            dir_file.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
        }
        delete[](dir);
        dir_file.close();

        for (auto page : pages) {
            buckets_file.write(page, Page::MDB_PAGE_SIZE);
            delete[](page);
        }
        buckets_file.close();
    }

    void create_id(const char* str, uint64_t id) {
        uint64_t hash[2];
        MurmurHash3_x64_128(str, strlen(str), 0, hash);

        // After a bucket split, need to try insert again.
        while (true) {
            // global_depth must be <= 64
            auto mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
            auto suffix = hash[0] & mask;
            ObjectFileHashMemImportBucket bucket(pages[dir[suffix]]);

            bool need_split;
            bucket.create_id(id, hash[0], &need_split);

            if (need_split) {
                auto new_page = new char[Page::MDB_PAGE_SIZE];
                memset(new_page, 0, Page::MDB_PAGE_SIZE);
                auto new_bucket_number = pages.size();
                pages.push_back(new_page);
                ObjectFileHashMemImportBucket new_bucket(new_page);
                *new_bucket.key_count   = 0;
                *new_bucket.local_depth = *bucket.local_depth + 1;

                if (*bucket.local_depth < global_depth) {
                    auto local_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - *bucket.local_depth);
                    auto new_suffix = (1 << (*bucket.local_depth)) | (local_mask & suffix); // 1|local_suffix

                    ++(*bucket.local_depth);
                    auto new_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - (*bucket.local_depth));

                    // redistribute keys between buckets `0|suffix` and `1|suffix`
                    bucket.redistribute(new_bucket, new_mask, new_suffix);

                    // update dirs having `new_suffix` as suffix and point to the new bucket number
                    auto update_dir_count = 1 << (global_depth - (*bucket.local_depth));
                    for (auto i = 0; i < update_dir_count; ++i) {
                        dir[(i << (*bucket.local_depth)) | new_suffix] = new_bucket_number;
                    }

                    assert(*bucket.key_count + *new_bucket.key_count == ObjectFileHashBucket::MAX_KEYS
                        && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split");

                } else {
                    assert(*bucket.local_depth == global_depth && "EXTENDIBLE HASH INCONSISTENCY: *bucket.local_depth != global_depth");
                    ++(*bucket.local_depth);

                    auto new_suffix = (1 << global_depth) | suffix; // 1|suffix

                    duplicate_dirs(); // increases global_depth

                    // redistribute keys between buckets `0|suffix` and `1|suffix`
                    auto new_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
                    bucket.redistribute(new_bucket, new_mask, new_suffix);

                    // update dir for `1|suffix`
                    dir[new_suffix] = new_bucket_number;

                    assert(*bucket.key_count + *new_bucket.key_count == ObjectFileHashBucket::MAX_KEYS
                        && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split");
                }
            } else {
                return;
            }
        }
    }

private:
    std::fstream dir_file;
    std::fstream buckets_file;

    uint_fast8_t global_depth;

    // array of size 2^global_depth
    uint_fast32_t* dir;

    void duplicate_dirs() {
        uint64_t old_dir_size = 1UL << global_depth;
        ++global_depth;
        auto new_dir_size = 1UL << global_depth;
        auto new_dir = new uint_fast32_t[new_dir_size];

        std::memcpy(new_dir,
                    dir,
                    old_dir_size * sizeof(uint_fast32_t));

        std::memcpy(&new_dir[old_dir_size],
                    dir,
                    old_dir_size * sizeof(uint_fast32_t));

        delete[](dir);
        dir = new_dir;
    }

    std::vector<char*> pages;
};



