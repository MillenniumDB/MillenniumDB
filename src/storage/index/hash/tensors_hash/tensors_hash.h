#pragma once

#include <cstdint>
#include <string>

#include "storage/file_id.h"

class TensorsHash {
public:
    static constexpr auto MIN_GLOBAL_DEPTH = 8;

    TensorsHash(const std::string& filename);

    void create_bytes_id(const char* bytes, uint64_t size, uint64_t new_id);

    // returns ObjectId::MASK_NOT_FOUND if tensor does not exist
    uint64_t get_bytes_id(const char* bytes, uint64_t size) const;

private:
    const FileId dir_file_id;

    const FileId buckets_file_id;
};
