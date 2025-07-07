#pragma once

#include <cassert>
#include <cstdint>
#include <fstream>

#include "storage/file_id.h"

class TensorsHash {
public:
    static constexpr char BUCKETS_FILENAME[] = "tensor_hash.dat";
    static constexpr char DIR_FILENAME[] = "tensor_hash.dir";

    static constexpr auto MIN_GLOBAL_DEPTH = 8;

    TensorsHash();

    ~TensorsHash();

    void create_bytes_id(const char* bytes, uint64_t num_bytes, uint64_t new_id);

    // returns ObjectId::MASK_NOT_FOUND if tensor does not exist
    uint64_t get_bytes_id(const char* bytes, uint64_t num_bytes) const;

private:
    const FileId buckets_file_id;

    // MIN_GLOBAL_DEPTH <= global_depth < 32
    uint32_t global_depth;

    bool dir_modified { false };

    uint32_t total_pages;

    // array of size pow(global_depth, 2)
    uint32_t* dir;

    std::fstream dir_file;

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

    void duplicate_dir();
};
