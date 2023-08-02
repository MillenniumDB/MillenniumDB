#pragma once

#include <memory>
#include <vector>


#include "storage/index/hash/key_value_hash/key_value_pair_hasher.h"


namespace HashJoin { namespace Generic {

struct Key {
    Key(uint64_t* start, size_t size) :
        start (start),
        size (size) { }

    ~Key() = default;

    bool operator==(const Key other) const {
        for (size_t i = 0; i < size; i++) {
            if (start[i] != other.start[i]) {
                return false;
            }
        }
        return true;
    }

    uint64_t* start;
    size_t size;
};


struct Hasher {
    uint64_t operator()(const Key key) const {
        return HashFunctionWrapper(key.start, key.size * sizeof(uint64_t));
    }
};

}}
