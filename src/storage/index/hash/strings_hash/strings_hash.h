#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "storage/file_id.h"

class StringsHash {
public:
    static constexpr auto MIN_GLOBAL_DEPTH = 8;

    StringsHash(const std::string& filename);
    ~StringsHash();

    // returns ObjectId::MASK_NOT_FOUND if string does not exist
    uint64_t get_str_id(const char* bytes, uint64_t size) const;

    // returns ObjectId::MASK_NOT_FOUND if string does not exist
    uint64_t get_str_id(const std::string& str) const {
        return get_str_id(str.data(), str.size());
    }

    // only call when you know string does not exist
    void create_str_id(const char* bytes, uint64_t size, uint64_t new_id);

private:
    const FileId buckets_file_id;

    // MIN_GLOBAL_DEPTH <= global_depth < 32
    uint_fast8_t global_depth;

    std::fstream dir_file;

    uint32_t total_pages;

    // array of size 2^global_depth
    uint32_t* dir;

    bool directory_modified = false;

    void duplicate_dir();
};
