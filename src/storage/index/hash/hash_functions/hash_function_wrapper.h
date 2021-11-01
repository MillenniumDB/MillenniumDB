#ifndef STORAGE__HASH_FUNCTION_WRAPPER_H_
#define STORAGE__HASH_FUNCTION_WRAPPER_H_
#include "storage/index/hash/hash_functions/murmur3.h"
#include "storage/index/hash/hash_functions/clhash.h"
#include "storage/index/hash/hash_functions/google_cityhash.h"
#include "storage/index/hash/hash_functions/farmhash.h"

template <class T>
uint64_t hash_function_wrapper(const T* key, uint_fast32_t key_size) {
    // murmur:
    // uint64_t hash_[2];
    // MurmurHash3_x64_128(key, key_size * sizeof(T), 0, hash_);
    // return hash_[0];

    // clhash:
    // clhasher h(UINT64_C(0x23a23cf5033c3c81),UINT64_C(0xb3816f6a2c68e530));
    // return h(key, key_size);  // arrayhash
    // ??return h(key); //vechash
    // assert(vechash == arrayhash);

    // cityhash:
    // return CityHash64(reinterpret_cast<const char*>(key), key_size * sizeof(T));

    // farmhash
    return util::Hash64(reinterpret_cast<const char*>(key), key_size * sizeof(T));
}

template <class T>
uint64_t hash_function_wrapper_2(const T* key, uint_fast32_t key_size) {
    //murmur:
    uint64_t hash_[2];
    MurmurHash3_x64_128(key, key_size * sizeof(T), 0, hash_);
    return hash_[1];

    // clhash:
    //clhasher h(UINT64_C(0x23a23cf5033c3c81),UINT64_C(0xb3816f6a2c68e530));
    //return h(key, key_size);  // arrayhash
    //??return h(key); //vechash
    //assert(vechash == arrayhash);

    // cityhash:
    //return CityHash64(reinterpret_cast<const char*>(key), key_size * sizeof(T));

    //farmhash
    //
}
#endif // STORAGE__HASH_FUNCTION_WRAPPER_H_