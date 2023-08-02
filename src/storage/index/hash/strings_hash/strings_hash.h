#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "storage/file_id.h"

class StringsHash {
public:
    static constexpr auto DEFAULT_GLOBAL_DEPTH = 8;

    StringsHash(const std::string& filename);
    ~StringsHash();

    uint64_t get_str_id(const char* bytes, uint64_t size) const;

    uint64_t get_str_id(const std::string& str) const {
        return get_str_id(str.data(), str.size());
    }

    uint64_t get_or_create_str_id(const char* bytes, uint64_t size);

    uint64_t get_or_create_str_id(const std::string& str) {
        return get_or_create_str_id(str.data(), str.size());
    }

private:
    const FileId buckets_file_id;

    uint_fast8_t global_depth;

    std::fstream dir_file;

    // array of size 2^global_depth
    uint_fast32_t* dir;

    void duplicate_dirs();
};
