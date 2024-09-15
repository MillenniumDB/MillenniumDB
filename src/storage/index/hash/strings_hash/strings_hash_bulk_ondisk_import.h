#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "macros/count_zeros.h"
#include "storage/index/hash/strings_hash/strings_hash.h"
#include "storage/index/hash/strings_hash/strings_hash_bucket.h"
#include "storage/page/unversioned_page.h"
#include "third_party/hashes/hash_function_wrapper.h"


class StringsHashBulkOnDiskImport {
static constexpr uint64_t hash_mask = 0x00'0F'FF'FF'FF'FF'FF'FFULL;
struct Bucket {
    uint32_t* const key_count;
    uint32_t* const local_depth;

    uint64_t* const arr1; // most significant 12bits:ID, 52 least significant bits:hash
    uint32_t* const arr2; // 32 least significant bits of ID

    Bucket(char* page) :
        key_count   (reinterpret_cast<uint32_t*>(page)),
        local_depth (reinterpret_cast<uint32_t*>(page + sizeof(uint32_t))),
        arr1        (reinterpret_cast<uint64_t*>(page + 2*sizeof(uint32_t))),
        arr2        (reinterpret_cast<uint32_t*>(page + 2*sizeof(uint32_t)
                    + sizeof(uint64_t)*StringsHashBucket::MAX_KEYS)) { }

    void create_id(uint64_t new_id, uint64_t hash, bool* const need_split) {
        assert(*key_count <= StringsHashBucket::MAX_KEYS);
        if (*key_count == StringsHashBucket::MAX_KEYS) {
            *need_split = true;
            return;
        }

        auto hash_ = (hash >> StringsHash::MIN_GLOBAL_DEPTH) & hash_mask;
        uint64_t new_arr1_val = (new_id & 0x00'00'0F'FF'00'00'00'00ULL) << 20ULL;
        new_arr1_val |= hash_;

        arr1[*key_count] = new_arr1_val;
        arr2[*key_count] = static_cast<uint32_t>(new_id & 0xFF'FF'FF'FFUL);
        ++(*key_count);

        *need_split = false;
    }


    void redistribute(Bucket& other, const uint64_t mask, const uint64_t other_suffix) {
        uint32_t other_pos = 0;
        uint32_t this_pos = 0;

        auto mask_ = mask >> StringsHash::MIN_GLOBAL_DEPTH;
        auto other_suffix_ = other_suffix >> StringsHash::MIN_GLOBAL_DEPTH;

        for (size_t i = 0; i < *key_count; i++) {
            if ((mask_ & arr1[i]) == other_suffix_) {
                other.arr1[other_pos] = arr1[i];
                other.arr2[other_pos] = arr2[i];

                ++other_pos;
            } else {
                arr1[this_pos] = arr1[i];
                arr2[this_pos] = arr2[i];

                ++this_pos;
            }
        }
        *this->key_count = this_pos;
        *other.key_count = other_pos;
    }
};

public:
    StringsHashBulkOnDiskImport(
        const std::string& filename,
        char*              buffer,
        uint64_t           buffer_capacity
    ) :
        buffer (buffer)
    {
        dir_file.open(filename + ".dir", std::ios::out|std::ios::binary);
        buckets_file.open(filename + ".dat", std::ios::out|std::ios::app); // only to create new file
        buckets_file.close();
        buckets_file.open(filename + ".dat", std::ios::in|std::ios::out|std::ios::binary);

        uint64_t buffer_bits = 64UL - (MDB_COUNT_LEADING_ZEROS_64(buffer_capacity)+1);

        uint64_t buffer_number_of_elements = (1UL << buffer_bits)/UPage::SIZE;
        buffer_bitmask = buffer_number_of_elements - 1;

        buffer_page_number = new uint64_t[buffer_number_of_elements];

        global_depth = StringsHash::MIN_GLOBAL_DEPTH;
        uint_fast32_t dir_size = 1 << global_depth;
        dir = new uint32_t[dir_size];


        // if StringsHash::MIN_GLOBAL_DEPTH = 8, import buffer is 2GB (2^31) and UPage::SIZE = 4KB (2^12)
        // dir_size is 2^8 and buffer_number_of_elements is 2^(31-12) => 2^19
        if (buffer_number_of_elements < dir_size) {
            throw std::invalid_argument("Need bigger buffer to create the initial directory for StringsHash");
        }

        for (uint_fast32_t i = 0; i < dir_size; ++i) {
            buffer_page_number[i] = i;
            auto page = buffer + (UPage::SIZE*i);
            Bucket bucket(page);
            *bucket.key_count = 0;
            *bucket.local_depth = StringsHash::MIN_GLOBAL_DEPTH;
            dir[i] = i;
        }
        total_pages = dir_size;
        buckets_file.write(buffer, dir_size*UPage::SIZE); // only to resize file
        for (uint64_t i = dir_size; i < buffer_number_of_elements; ++i) {
            // assign page to unassigned
            buffer_page_number[i] = UINT64_MAX;
        }
        split_buffer = new char[UPage::SIZE];
    }

    ~StringsHashBulkOnDiskImport() {
        dir_file.seekg(0, dir_file.beg);
        dir_file.write(reinterpret_cast<const char*>(&global_depth), sizeof(global_depth));
        dir_file.write(reinterpret_cast<const char*>(&total_pages), sizeof(total_pages));

        uint_fast32_t dir_size = 1 << global_depth;
        for (uint_fast32_t i = 0; i < dir_size; ++i) {
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
                buckets_file.seekp(page_number*UPage::SIZE, buckets_file.beg);
                buckets_file.write(current_page, UPage::SIZE);
            }
            current_page += UPage::SIZE;
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
            auto global_mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64 - global_depth);
            auto global_suffix = hash & global_mask;
            auto bucket_number = dir[global_suffix];
            auto bucket_page = get_page(bucket_number);
            Bucket bucket(bucket_page);

            assert(*bucket.local_depth <= global_depth);

            bool need_split;
            bucket.create_id(id, hash, &need_split);

            if (need_split) {
                auto new_bucket_number = total_pages;
                total_pages++;

                ++(*bucket.local_depth);
                Bucket new_bucket(split_buffer);
                *new_bucket.key_count = 0;
                *new_bucket.local_depth = *bucket.local_depth;

                auto new_mask = 0xFFFF'FFFF'FFFF'FFFFULL >> (64 - (*bucket.local_depth));
                auto new_suffix = (hash & new_mask) | (1ULL << (*bucket.local_depth-1));

                if (*bucket.local_depth <= global_depth) {
                    // update dirs having `new_suffix`, point them to the new_bucket
                    auto update_dir_count = 1ULL << (global_depth - (*bucket.local_depth));

                    for (uint_fast32_t i = 0; i < update_dir_count; ++i) {
                        auto new_dir_pos = new_suffix | (i << *bucket.local_depth);
                        dir[new_dir_pos] = new_bucket_number;
                    }
                } else { // *bucket.local_depth == global_depth + 1
                    duplicate_dir();
                    dir[new_suffix] = new_bucket_number;
                }
                bucket.redistribute(new_bucket, new_mask, new_suffix);
                assert(*bucket.key_count + *new_bucket.key_count == StringsHashBucket::MAX_KEYS
                    && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split");

                buckets_file.seekp(0, buckets_file.end);
                buckets_file.write(split_buffer, UPage::SIZE);
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

    uint_fast8_t global_depth;

    std::fstream dir_file;
    std::fstream buckets_file;

    uint32_t total_pages;

    // array of size 2^global_depth
    uint32_t* dir;

    char* get_page(uint_fast32_t page_number) {
        auto buffer_idx = page_number & buffer_bitmask;

        auto old_page_number = buffer_page_number[buffer_idx];
        auto page = buffer + (UPage::SIZE*buffer_idx);
        if (old_page_number == page_number) {
            // cache hit
            return page;
        } else if (old_page_number == UINT64_MAX) {
            // load new page from disk
            buckets_file.seekg(page_number*UPage::SIZE, buckets_file.beg);
            buckets_file.read(page, UPage::SIZE);

            buffer_page_number[buffer_idx] = page_number;
            return page;
        } else {
            // flush old page to disk
            buckets_file.seekp(old_page_number*UPage::SIZE, buckets_file.beg);
            buckets_file.write(page, UPage::SIZE);
            // load new page from disk
            buckets_file.seekg(page_number*UPage::SIZE, buckets_file.beg);
            buckets_file.read(page, UPage::SIZE);

            buffer_page_number[buffer_idx] = page_number;
            return page;
        }
    }

    void duplicate_dir() {
        uint64_t old_dir_size = 1UL << global_depth;
        ++global_depth;
        auto new_dir_size = 1UL << global_depth;
        auto new_dir = new uint32_t[new_dir_size];

        std::memcpy(new_dir,
                    dir,
                    old_dir_size * sizeof(uint32_t));

        std::memcpy(&new_dir[old_dir_size],
                    dir,
                    old_dir_size * sizeof(uint32_t));

        delete[](dir);
        dir = new_dir;
    }
};
