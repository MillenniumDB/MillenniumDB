#pragma once

#include <cstdint>

#include "storage/file_id.h"
#include "storage/page/unversioned_page.h"

class StringsHashBucket {
public:
    static constexpr auto MAX_KEYS = (UPage::SIZE - 2*sizeof(uint32_t) ) / (sizeof(uint64_t) + sizeof(uint32_t));

    StringsHashBucket(UPage& page);

    ~StringsHashBucket();

    uint64_t get_id(const char* bytes, uint64_t size, uint64_t hash) const;

    // only call this when no split is needed (*key_count < MAX_KEYS)
    void create_str_id(uint64_t new_id, uint64_t hash);

    void redistribute(StringsHashBucket& other, uint64_t mask, uint64_t other_suffix);

    UPage& page;

    uint32_t* const key_count;
    uint32_t* const local_depth;

    uint64_t* const arr1; // most significant 12bits:ID, 52 least significant bits:hash
    uint32_t* const arr2; // 32 least significant bits of ID
};
