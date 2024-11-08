#pragma once

#include <cstdint>
#include <cstddef>

#ifdef __AVX2__
#include "third_party/hashes/xxhash/xxhash.h"
#else
#include "third_party/hashes/murmur3/murmur3.h"
#endif

inline uint64_t HashFunctionWrapper(const void* input, size_t length) {
#ifdef __AVX2__
    return XXH3_64bits(input, length);
#else
    uint64_t _hash[2];
    MurmurHash3_x64_128(input, length, 0, _hash);
    return _hash[0];
#endif
}
