#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "storage/file_id.h"
#include "storage/page.h"

template <class U> class DistinctBindingHash;

template <class T>
class DistinctBindingHashBucket {

friend class DistinctBindingHash<T>;

public:
    DistinctBindingHashBucket(const TmpFileId file_id, const uint_fast32_t bucket_number, std::size_t tuple_size);
    ~DistinctBindingHashBucket();

    bool is_in(const std::vector<T>& tuple, uint64_t hash1, uint64_t hash2);
    bool is_in_or_insert(const std::vector<T>& tuple, uint64_t hash1, uint64_t hash2, bool* const need_split);

private:
    Page& page;

    const uint_fast32_t max_tuples;

    T*        const tuples;
    uint64_t* const hashes; // each tuple is (hash1, hash2)
    uint16_t* const tuple_size;
    uint8_t*  const tuple_count;
    uint8_t*  const local_depth;

    void redistribute(DistinctBindingHashBucket<T>& other, uint64_t mask, uint64_t other_suffix);
};
