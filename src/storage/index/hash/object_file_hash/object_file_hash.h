#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "storage/file_id.h"
#include "storage/index/object_file/object_file.h"

class ObjectFileHash {
public:
    static constexpr auto DEFAULT_GLOBAL_DEPTH = 8;

    ObjectFileHash(ObjectFile& object_file, const std::string& filename);
    ~ObjectFileHash();

    uint64_t get_id(const std::string& str) const;
    uint64_t get_or_create_id(const std::string& str, bool* const created);

private:
    ObjectFile& object_file;
    const FileId buckets_file_id;

    uint_fast8_t global_depth;

    std::fstream dir_file;

    // array of size 2^global_depth
    uint_fast32_t* dir;

    void duplicate_dirs();
};
