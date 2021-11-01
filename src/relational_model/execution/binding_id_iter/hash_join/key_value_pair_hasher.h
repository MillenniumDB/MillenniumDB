#ifndef RELATIONAL_MODEL__KEY_VALUE_PAIR_HASHER_H_
#define RELATIONAL_MODEL__KEY_VALUE_PAIR_HASHER_H_

#include <unordered_map>

#include "storage/index/hash/key_value_hash/key_value_hash.h"
#include "storage/index/hash/hash_functions/hash_function_wrapper.h"

struct KeyValuePairHasher {
    uint64_t operator()(const std::vector<ObjectId>& key) const {
        return hash_function_wrapper(key.data(), key.size());
    }
};

struct KeyValuePairHasher2 {
    uint64_t operator()(const std::vector<ObjectId>& key) const {
        return hash_function_wrapper_2(key.data(), key.size());
    }
};

#endif // RELATIONAL_MODEL__KEY_VALUE_PAIR_HASHER_H_
