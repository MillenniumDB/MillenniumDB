#pragma once

#include <vector>

#include "graph_models/object_id.h"
#include "third_party/hashes/hash_function_wrapper.h"

struct KeyValuePairHasher {
    uint64_t operator()(const std::vector<ObjectId>& key) const {
        return HashFunctionWrapper(key.data(), key.size());
    }
};
