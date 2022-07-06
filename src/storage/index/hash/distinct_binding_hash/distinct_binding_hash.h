#pragma once

#include <cstdint>
#include <vector>

#include "base/ids/var_id.h"
#include "storage/file_id.h"

template <class T>
class DistinctBindingHash {
public:
    static constexpr auto DEFAULT_GLOBAL_DEPTH = 10;

    DistinctBindingHash(std::size_t vars_size);
    ~DistinctBindingHash();

    bool is_in(const std::vector<T>& tuple);

    // returns true if tuple is present, insert it otherwise
    bool is_in_or_insert(const std::vector<T>& tuple);

private:
    uint_fast8_t global_depth = DEFAULT_GLOBAL_DEPTH;
    std::size_t tuple_size;

    const TmpFileId buckets_file_id;

    // array of size 2^global_depth
    uint_fast32_t* dir;

    void duplicate_dirs();
};
