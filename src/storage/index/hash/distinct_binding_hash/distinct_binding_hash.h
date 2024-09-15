#pragma once

#include <cstdint>
#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "storage/file_id.h"

class DistinctBindingHash {
public:
    static constexpr auto DEFAULT_GLOBAL_DEPTH = 8;

    DistinctBindingHash(uint_fast32_t vars_size);
    ~DistinctBindingHash();

    // Clears all stored tuples
    void reset();

    bool is_in(const std::vector<ObjectId>& tuple);

    // returns true if tuple is present, insert it otherwise
    bool is_in_or_insert(const std::vector<ObjectId>& tuple);

private:
    uint_fast8_t global_depth = DEFAULT_GLOBAL_DEPTH;
    uint_fast32_t tuple_size;

    const TmpFileId buckets_file_id;

    // array of size 2^global_depth
    uint_fast32_t* dir;

    void duplicate_dirs();
};
