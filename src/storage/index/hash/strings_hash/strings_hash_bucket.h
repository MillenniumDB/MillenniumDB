#pragma once

#include <cstdint>
#include <string>

#include "storage/file_id.h"
#include "storage/page.h"

class StringsHashBucket {
friend class StringsHash;
public:
    static constexpr auto MAX_KEYS = (Page::MDB_PAGE_SIZE - sizeof(uint64_t) ) / (2*sizeof(uint64_t));

    StringsHashBucket(FileId file_id, uint_fast32_t bucket_number);

    ~StringsHashBucket();

    uint64_t get_str_id(const std::string& str, uint64_t hash) const;

    uint64_t get_or_create_str_id(const std::string& str, uint64_t hash, bool* need_split);

private:
    Page& page;

    uint32_t* const key_count;
    uint32_t* const local_depth;
    uint64_t* const hashes;
    uint64_t* const ids;

    void redistribute(StringsHashBucket& other, uint64_t mask, uint64_t other_suffix);
};
