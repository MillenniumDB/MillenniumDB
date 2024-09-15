#pragma once

#include <cstdint>
#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "storage/file_id.h"
#include "storage/page/private_page.h"

class DistinctBindingHashBucket {

friend class DistinctBindingHash;

public:
    DistinctBindingHashBucket(TmpFileId file_id, uint_fast32_t bucket_number, uint_fast32_t tuple_size);
    ~DistinctBindingHashBucket();

    bool is_in(const std::vector<ObjectId>& tuple, uint64_t hash);
    bool is_in_or_insert(const std::vector<ObjectId>& tuple, uint64_t hash, bool* const need_split);

private:
    PPage& page;

    const uint_fast32_t tuple_size;
    const uint_fast32_t max_tuples;

    ObjectId* const tuples;
    uint64_t* const hashes; // each tuple is (hash1, hash2)
    uint8_t*  const tuple_count;
    uint8_t*  const local_depth;

    void redistribute(DistinctBindingHashBucket& other, uint64_t mask, uint64_t other_suffix);
};
