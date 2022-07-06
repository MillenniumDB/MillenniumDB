#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "storage/file_id.h"
#include "storage/index/object_file/object_file.h"
#include "storage/page.h"

class ObjectFileHashBucket {
friend class ObjectFileHash;
public:
    static constexpr auto MAX_KEYS = (Page::MDB_PAGE_SIZE - sizeof(uint64_t) ) / (2*sizeof(uint64_t));

    ObjectFileHashBucket(FileId file_id, uint_fast32_t bucket_number, ObjectFile& objecy_file);

    ~ObjectFileHashBucket();

    uint64_t get_id(const std::string& str, const uint64_t hash) const;

    uint64_t get_or_create_id(const std::string& str,
                              const uint64_t hash,
                              bool* const need_split,
                              bool* const created);

private:
    Page& page;
    ObjectFile& object_file;

    uint32_t* const key_count;
    uint32_t* const local_depth;
    uint64_t* const hashes;
    uint64_t* const ids;

    void redistribute(ObjectFileHashBucket& other, const uint64_t mask, const uint64_t other_suffix);
};
