#pragma once

#include <vector>

#include "base/ids/object_id.h"
#include "third_party/murmur3/murmur3.h"

template <class T>
uint64_t hash_function_wrapper(const T* key, uint_fast32_t key_size) {
    // murmur3:
    uint64_t hash_[2];
    MurmurHash3_x64_128(key, key_size * sizeof(T), 0, hash_);
    return hash_[0];
}

struct KeyValuePairHasher {
    uint64_t operator()(const std::vector<ObjectId>& key) const {
        return hash_function_wrapper(key.data(), key.size());
    }
};
