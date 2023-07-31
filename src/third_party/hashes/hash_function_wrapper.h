#pragma once

#include <cstdint>

#ifdef __AVX2__
#include "third_party/xxhash/xxhash.h"
#else
#include "third_party/murmur3/murmur3.h"
#endif

inline uint64_t HashFunctionWrapper(const void* input, std::size_t length) {
#ifdef __AVX2__
    return XXH3_64bits(input, length);
#else
    uint64_t _hash[2];
    MurmurHash3_x64_128(input, length, 0, _hash);
    return _hash[0];
#endif
}
