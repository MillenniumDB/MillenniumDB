#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "storage/index/hash/strings_hash/strings_hash.h"
#include "storage/index/hash/strings_hash/strings_hash_bucket.h"
#include "storage/page.h"
#include "third_party/hashes/hash_function_wrapper.h"


class StringsHashBulkOnDiskImport {
struct Bucket {
    Bucket(char* page) :
        key_count   (reinterpret_cast<uint32_t*>(page)),
        local_depth (reinterpret_cast<uint32_t*>(page + sizeof(uint32_t))),
        hashes      (reinterpret_cast<uint64_t*>(page + 2*sizeof(uint32_t))),
        ids         (reinterpret_cast<uint64_t*>(page + sizeof(uint64_t) + sizeof(uint64_t)*StringsHashBucket::MAX_KEYS)) { }

    void create_id(uint64_t new_id, const uint64_t hash, bool* const need_split) {
        assert(*key_count <= StringsHashBucket::MAX_KEYS);
        if (*key_count == StringsHashBucket::MAX_KEYS) {
            *need_split = true;
            return;
        }

        hashes[*key_count] = hash;
        ids[*key_count] = new_id;
        ++(*key_count);

        *need_split = false;
    }

    uint32_t* const key_count;
    uint32_t* const local_depth;
    uint64_t* const hashes;
    uint64_t* const ids;

    void redistribute(Bucket& other, const uint64_t mask, const uint64_t other_suffix) {
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

public:
    StringsHashBulkOnDiskImport(const std::string& filename,
                                char* buffer,
                                uint64_t buffer_capacity) :
        buffer (buffer)
    {
        dir_file.open(filename + ".dir", std::ios::out|std::ios::binary);
        buckets_file.open(filename + ".dat", std::ios::out|std::ios::app); // only to create new file
        buckets_file.close();
        buckets_file.open(filename + ".dat", std::ios::in|std::ios::out|std::ios::binary);

        uint64_t buffer_bits = 64UL - (__builtin_clzll(buffer_capacity)+1);

        uint64_t buffer_number_of_elements = (1UL << buffer_bits)/Page::MDB_PAGE_SIZE;
        buffer_bitmask = buffer_number_of_elements - 1;

        buffer_page_number = new uint64_t[buffer_number_of_elements];

        global_depth = StringsHash::DEFAULT_GLOBAL_DEPTH;
        uint_fast32_t dir_size = 1 << global_depth;
        dir = new uint_fast32_t[dir_size];

        // TODO: if buffer_number_of_elements > dir_size we put all in buffer immediately
        // TODO: if buffer_number_of_elements < dir_size we put what we can
        // TODO: assuming buffer_number_of_elements > dir_size
        for (uint_fast32_t i = 0; i < dir_size; ++i) {
            buffer_page_number[i] = i;
            auto page = buffer + (Page::MDB_PAGE_SIZE*i);
            Bucket bucket(page);
            *bucket.key_count = 0;
            *bucket.local_depth = StringsHash::DEFAULT_GLOBAL_DEPTH;
            dir[i] = i;
        }
        buckets_file.write(buffer, dir_size*Page::MDB_PAGE_SIZE); // only to resize file
        for (uint64_t i = dir_size; i < buffer_number_of_elements; ++i) {
            // assign page to unassigned
            buffer_page_number[i] = UINT64_MAX;
        }
        split_buffer = new char[Page::MDB_PAGE_SIZE];
    }

    ~StringsHashBulkOnDiskImport() {
        dir_file.seekg(0, dir_file.beg);
        dir_file.write(reinterpret_cast<const char*>(&global_depth), sizeof(uint8_t));
        uint_fast32_t dir_size = 1 << global_depth;
        for (uint64_t i = 0; i < dir_size; ++i) {
            uint32_t tmp = dir[i];
            dir_file.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
        }
        delete[](dir);
        dir_file.close();

        // flush buffer
        auto current_page = buffer;
        for (uint64_t i = 0; i <= buffer_bitmask; ++i) {
            auto page_number = buffer_page_number[i];
            if (page_number != UINT64_MAX) {
                buckets_file.seekp(page_number*Page::MDB_PAGE_SIZE, buckets_file.beg);
                buckets_file.write(current_page, Page::MDB_PAGE_SIZE);
            }
            current_page += Page::MDB_PAGE_SIZE;
        }
        buckets_file.close();
        delete[] split_buffer;
        delete[] buffer_page_number;
    }

    // str is not null terminated
    void create_id(const char* str, uint64_t id, size_t strlen) {
        uint64_t hash = HashFunctionWrapper(str, strlen);

        // After a bucket split, need to try insert again.
        while (true) {
            // global_depth must be <= 64
            auto mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
            auto suffix = hash & mask;
            auto bucket_page_number = dir[suffix];
            auto bucket_page = get_page(bucket_page_number); // TODO:
            Bucket bucket(bucket_page);

            bool need_split;
            bucket.create_id(id, hash, &need_split);

            if (need_split) {
                buckets_file.seekp(0, buckets_file.end);
                uint64_t new_page_number = buckets_file.tellp() / Page::MDB_PAGE_SIZE;

                Bucket new_bucket(split_buffer);
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
                        dir[(i << (*bucket.local_depth)) | new_suffix] = new_page_number;
                    }

                    assert(*bucket.key_count + *new_bucket.key_count == StringsHashBucket::MAX_KEYS
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
                    dir[new_suffix] = new_page_number;

                    assert(*bucket.key_count + *new_bucket.key_count == StringsHashBucket::MAX_KEYS
                        && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split");
                }
                buckets_file.write(split_buffer, Page::MDB_PAGE_SIZE);
            } else {
                return;
            }
        }
    }

private:
    char* buffer;

    uint64_t buffer_bitmask;

    uint64_t* buffer_page_number; // array

    char* split_buffer;

    std::fstream dir_file;
    std::fstream buckets_file;

    uint_fast8_t global_depth;

    // array of size 2^global_depth
    uint_fast32_t* dir;

    char* get_page(uint_fast32_t page_number) {
        auto buffer_idx = page_number & buffer_bitmask;

        auto old_page_number = buffer_page_number[buffer_idx];
        auto page = buffer + (Page::MDB_PAGE_SIZE*buffer_idx);
        if (old_page_number == page_number) {
            // cache hit
            return page;
        } else if (old_page_number == UINT64_MAX) {
            // load new page from disk
            buckets_file.seekg(page_number*Page::MDB_PAGE_SIZE, buckets_file.beg);
            buckets_file.read(page, Page::MDB_PAGE_SIZE);

            buffer_page_number[buffer_idx] = page_number;
            return page;
        } else {
            // flush old page to disk
            buckets_file.seekp(old_page_number*Page::MDB_PAGE_SIZE, buckets_file.beg);
            buckets_file.write(page, Page::MDB_PAGE_SIZE);
            // load new page from disk
            buckets_file.seekg(page_number*Page::MDB_PAGE_SIZE, buckets_file.beg);
            buckets_file.read(page, Page::MDB_PAGE_SIZE);

            buffer_page_number[buffer_idx] = page_number;
            return page;
        }
    }

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
};