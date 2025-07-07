#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <unistd.h>

#include "macros/count_zeros.h"
#include "storage/index/hash/tensors_hash/tensors_hash.h"
#include "storage/index/hash/tensors_hash/tensors_hash_bucket.h"
#include "storage/page/unversioned_page.h"
#include "system/file_manager.h"
#include "third_party/hashes/hash_function_wrapper.h"

class TensorsHashBulkOnDiskImport {
    struct Bucket {
        uint32_t* const key_count;
        uint32_t* const local_depth;

        uint64_t* const arr1; // most significant 12bits:ID, 52 least significant bits:hash
        uint32_t* const arr2; // 32 least significant bits of ID

        Bucket(char* page) :
            key_count { reinterpret_cast<uint32_t*>(page) },
            local_depth { reinterpret_cast<uint32_t*>(page + sizeof(uint32_t)) },
            arr1 { reinterpret_cast<uint64_t*>(page + 2 * sizeof(uint32_t)) },
            arr2 { reinterpret_cast<uint32_t*>(
                page + 2 * sizeof(uint32_t) + sizeof(uint64_t) * TensorsHashBucket::MAX_KEYS
            ) }
        { }

        void create_id(uint64_t new_id, uint64_t hash, bool* const need_split)
        {
            assert(*key_count <= TensorsHashBucket::MAX_KEYS);

            if (*key_count == TensorsHashBucket::MAX_KEYS) {
                *need_split = true;
                return;
            }

            const uint64_t hash_ = (hash >> TensorsHash::MIN_GLOBAL_DEPTH) & TensorsHashBucket::HASH_MASK;
            uint64_t new_arr1_val = (new_id & 0x00'00'0F'FF'00'00'00'00ULL) << 20ULL;
            new_arr1_val |= hash_;

            arr1[*key_count] = new_arr1_val;
            arr2[*key_count] = static_cast<uint32_t>(new_id & 0xFF'FF'FF'FFUL);
            ++(*key_count);

            *need_split = false;
        }

        void redistribute(Bucket& other, const uint64_t mask, const uint64_t other_suffix)
        {
            uint32_t other_pos = 0;
            uint32_t this_pos = 0;

            const uint64_t mask_ = mask >> TensorsHash::MIN_GLOBAL_DEPTH;
            uint64_t other_suffix_ = other_suffix >> TensorsHash::MIN_GLOBAL_DEPTH;

            for (std::size_t i = 0; i < *key_count; ++i) {
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
    TensorsHashBulkOnDiskImport(char* buffer, uint64_t buffer_capacity) :
        buffer { buffer }
    {
        const auto dir_path = file_manager.get_file_path(TensorsHash::DIR_FILENAME);
        const auto buckets_path = file_manager.get_file_path(TensorsHash::BUCKETS_FILENAME);
        dir_fd = open(dir_path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if (dir_fd == -1) {
            throw std::runtime_error("Could not open file " + dir_path);
        }

        buckets_fd = open(
            buckets_path.c_str(),
            O_RDWR | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
        );
        if (buckets_fd == -1) {
            throw std::runtime_error("Could not open file " + buckets_path);
        }

        const uint64_t buffer_bits = 64ULL - (MDB_COUNT_LEADING_ZEROS_64(buffer_capacity) + 1);
        const uint64_t buffer_num_elements = (1ULL << buffer_bits) / UPage::SIZE;
        buffer_bitmask = buffer_num_elements - 1;

        buffer_page_number = new uint64_t[buffer_num_elements];

        global_depth = TensorsHash::MIN_GLOBAL_DEPTH;
        const auto dir_size = get_dir_size();
        dir = new uint32_t[dir_size];

        // if TensorsHash::MIN_GLOBAL_DEPTH = 8, import buffer is 2GB (2^31) and UPage::SIZE = 4KB (2^12)
        // dir_size is 2^8 and buffer_number_of_elements is 2^(31-12) => 2^19
        if (buffer_num_elements < dir_size) {
            throw std::invalid_argument("Need a bigger buffer to create the initial TensorsHash directory");
        }

        for (std::size_t i = 0; i < dir_size; ++i) {
            buffer_page_number[i] = i;
            auto page = buffer + (UPage::SIZE * i);
            Bucket bucket(page);
            *bucket.key_count = 0;
            *bucket.local_depth = TensorsHash::MIN_GLOBAL_DEPTH;
            dir[i] = i;
        }
        total_pages = dir_size;
        check_io(ftruncate(buckets_fd, dir_size * UPage::SIZE));
        for (uint64_t i = dir_size; i < buffer_num_elements; ++i) {
            // assign page to unassigned
            buffer_page_number[i] = UINT64_MAX;
        }
        split_buffer = new char[UPage::SIZE];
    }

    ~TensorsHashBulkOnDiskImport()
    {
        lseek(dir_fd, 0, SEEK_SET);
        check_io(write(dir_fd, reinterpret_cast<const char*>(&global_depth), sizeof(global_depth)));
        check_io(write(dir_fd, reinterpret_cast<const char*>(&total_pages), sizeof(total_pages)));

        const std::size_t dir_size = 1 << global_depth;
        check_io(write(dir_fd, reinterpret_cast<const char*>(dir), sizeof(uint32_t) * dir_size));
        delete[] (dir);
        close(dir_fd);

        // flush buffer
        auto current_page = buffer;
        for (uint64_t i = 0; i <= buffer_bitmask; ++i) {
            auto page_number = buffer_page_number[i];
            if (page_number != UINT64_MAX) {
                check_io(pwrite(buckets_fd, current_page, UPage::SIZE, page_number * UPage::SIZE));
            }
            current_page += UPage::SIZE;
        }
        close(buckets_fd);
        delete[] split_buffer;
        delete[] buffer_page_number;
    }

    // str is not null terminated
    void create_id(const char* bytes, std::size_t num_bytes, uint64_t id)
    {
        const uint64_t hash = HashFunctionWrapper(bytes, num_bytes);

        // After a bucket split, need to try insert again.
        while (true) {
            const uint64_t global_mask = get_global_mask();
            const uint64_t global_suffix = hash & global_mask;
            const auto bucket_number = dir[global_suffix];

            auto bucket_page = get_page(bucket_number);
            Bucket bucket(bucket_page);

            assert(*bucket.local_depth <= global_depth);

            bool need_split;
            bucket.create_id(id, hash, &need_split);

            if (need_split) {
                const auto new_bucket_number = total_pages;
                ++total_pages;

                ++(*bucket.local_depth);
                Bucket new_bucket(split_buffer);
                *new_bucket.key_count = 0;
                *new_bucket.local_depth = *bucket.local_depth;

                const auto new_local_mask = get_local_mask(*bucket.local_depth);
                const auto new_local_suffix = (hash & new_local_mask) | (1ULL << (*bucket.local_depth - 1));

                if (*bucket.local_depth <= global_depth) {
                    // update dirs having `new_local_suffix`, point them to the new_bucket
                    const uint64_t update_dir_count = 1ULL << (global_depth - (*bucket.local_depth));

                    for (std::size_t i = 0; i < update_dir_count; ++i) {
                        const uint64_t new_dir_pos = new_local_suffix | (i << *bucket.local_depth);
                        dir[new_dir_pos] = new_bucket_number;
                    }
                } else { // *bucket.local_depth == global_depth + 1
                    duplicate_dir();
                    dir[new_local_suffix] = new_bucket_number;
                }
                bucket.redistribute(new_bucket, new_local_mask, new_local_suffix);
                assert(
                    *bucket.key_count + *new_bucket.key_count == TensorsHashBucket::MAX_KEYS
                    && "EXTENDIBLE HASH INCONSISTENCY: sum of keys must be MAX_KEYS after a split"
                );

                lseek(buckets_fd, 0, SEEK_END);
                check_io(write(buckets_fd, split_buffer, UPage::SIZE));
            } else {
                return;
            }
        }
    }

private:
    char* buffer;

    uint64_t buffer_bitmask;

    uint64_t* buffer_page_number;

    char* split_buffer;

    uint32_t global_depth;

    int dir_fd;
    int buckets_fd;

    uint32_t total_pages;

    // array of size pow(2, global_depth)
    uint32_t* dir;

    static inline uint64_t get_local_mask(uint64_t local_depth)
    {
        assert(local_depth <= 64);
        return 0xFFFF'FFFF'FFFF'FFFF >> (64UL - local_depth);
    }

    inline uint64_t get_global_mask() const
    {
        assert(global_depth <= 64);
        return 0xFFFF'FFFF'FFFF'FFFF >> (64UL - global_depth);
    }

    inline std::size_t get_dir_size() const
    {
        return 1UL << global_depth;
    }

    void duplicate_dir()
    {
        const auto old_dir_size = get_dir_size();
        ++global_depth;
        const auto new_dir_size = get_dir_size();
        auto new_dir = new uint32_t[new_dir_size];

        std::memcpy(new_dir, dir, sizeof(uint32_t) * old_dir_size);

        std::memcpy(&new_dir[old_dir_size], dir, sizeof(uint32_t) * old_dir_size);

        delete[] (dir);
        dir = new_dir;
    }

    char* get_page(uint32_t page_number)
    {
        const auto buffer_idx = page_number & buffer_bitmask;

        const auto old_page_number = buffer_page_number[buffer_idx];
        auto page = buffer + (UPage::SIZE * buffer_idx);
        if (old_page_number == page_number) {
            // cache hit
            return page;
        } else if (old_page_number == UINT64_MAX) {
            // load new page from disk
            check_io(pread(buckets_fd, page, UPage::SIZE, page_number * UPage::SIZE));

            buffer_page_number[buffer_idx] = page_number;
            return page;
        } else {
            // flush old page to disk
            check_io(pwrite(buckets_fd, page, UPage::SIZE, old_page_number * UPage::SIZE));
            // load new page from disk
            check_io(pread(buckets_fd, page, UPage::SIZE, page_number * UPage::SIZE));

            buffer_page_number[buffer_idx] = page_number;
            return page;
        }
    }

    void check_io(int io_res)
    {
        if (io_res == -1) {
            throw std::runtime_error("Error creating tensors hash");
        }
    }
};
