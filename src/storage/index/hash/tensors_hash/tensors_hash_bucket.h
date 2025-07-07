#pragma once

#include <cstdint>

#include "storage/page/unversioned_page.h"

class TensorsHashBucket {
public:
    static constexpr auto MAX_KEYS = (UPage::SIZE - 2 * sizeof(uint32_t))
                                   / (sizeof(uint64_t) + sizeof(uint32_t));

    static constexpr uint64_t HASH_MASK = 0x00'0F'FF'FF'FF'FF'FF'FFULL;

    UPage& page;

    uint32_t* const key_count;
    uint32_t* const local_depth;

    uint64_t* const arr1; // most significant 12bits:ID, 52 least significant bits:hash
    uint32_t* const arr2; // 32 least significant bits of ID

    TensorsHashBucket(UPage& page);

    ~TensorsHashBucket();

    uint64_t get_id(const char* bytes, uint64_t num_bytes, uint64_t hash) const;

    // only call this when no split is needed (*key_count < MAX_KEYS)
    void create_id(uint64_t new_id, uint64_t hash);

    void redistribute(TensorsHashBucket& other, uint64_t mask, uint64_t other_suffix);
};
