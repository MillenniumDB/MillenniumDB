#pragma once

#include <array>
#include <memory>
#include <vector>

#include "storage/index/hash/key_value_hash/key_value_pair_hasher.h"

namespace HashJoin { namespace BGP {

template<size_t N>
struct Key {
    Key(uint64_t* start) :
        start (start) { }

    ~Key() = default;

    bool operator==(const Key<N> other) const {
        for (size_t i = 0; i < N; i++) {
            if (start[i] != other.start[i]) {
                return false;
            }
        }
        return true;
    }

    uint64_t* start;
};


template<size_t N>
struct Hasher {
    uint64_t operator()(const Key<N> key) const {
        return HashFunctionWrapper(key.start, N * sizeof(uint64_t));
    }
};

template<>
struct Hasher<1> {
    uint64_t operator()(const Key<1> key) const {
        return *key.start;
    }
};


struct ObjectIdHasher {
    uint64_t operator()(const ObjectId key) const {
        return key.id;
    }
};

}}
