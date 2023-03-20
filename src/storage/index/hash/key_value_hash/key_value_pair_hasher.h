#pragma once

#include <vector>

#include "base/ids/object_id.h"
#include "third_party/xxhash/xxhash.h"

template <class T>
uint64_t hash_function_wrapper(const T* key, uint_fast32_t key_size) {
    return XXH3_64bits(key, key_size * sizeof(T));
}

struct KeyValuePairHasher {
    uint64_t operator()(const std::vector<ObjectId>& key) const {
        return hash_function_wrapper(key.data(), key.size());
    }
};
