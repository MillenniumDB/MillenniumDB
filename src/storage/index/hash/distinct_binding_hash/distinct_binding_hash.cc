#include "distinct_binding_hash.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>

#include "base/ids/object_id.h"
#include "base/graph_object/graph_object.h"
#include "storage/file_manager.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash_bucket.h"
#include "third_party/xxhash/xxhash.h"

template <class T>
DistinctBindingHash<T>::DistinctBindingHash(std::size_t tuple_size) :
    tuple_size      (tuple_size),
    buckets_file_id (file_manager.get_tmp_file_id())
{
    // create directory with 2^global_depth empty buckets
    uint_fast32_t dir_size = 1 << global_depth;
    dir = new uint_fast32_t[dir_size];
    for (uint_fast32_t i = 0; i < dir_size; ++i) {
        DistinctBindingHashBucket<T> bucket(buckets_file_id, i, tuple_size);
        *bucket.tuple_count = 0;
        *bucket.local_depth = DEFAULT_GLOBAL_DEPTH;
        dir[i] = i;
        bucket.page.make_dirty();
    }
}


template <class T>
DistinctBindingHash<T>::~DistinctBindingHash() {
    delete[] dir;
    file_manager.remove_tmp(buckets_file_id);
}


template <class T>
bool DistinctBindingHash<T>::is_in(const std::vector<T>& tuple) {
    assert(tuple.size() == tuple_size);

    uint64_t hash = XXH3_64bits(tuple.data(), tuple.size() * sizeof(T));

    // global_depth must be <= 64
    auto mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
    auto suffix = hash & mask;
    auto bucket_number = dir[suffix];
    auto bucket = DistinctBindingHashBucket<T>(buckets_file_id, bucket_number, tuple_size);

    return bucket.is_in(tuple, hash);
}


template <class T>
bool DistinctBindingHash<T>::is_in_or_insert(const std::vector<T>& tuple) {
    assert(tuple.size() == tuple_size);

    uint64_t hash = XXH3_64bits(tuple.data(), tuple.size() * sizeof(T));

    // After a bucket split, need to try insert again.
    while (true) {
        // global_depth must be <= 64
        auto mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
        auto suffix = hash & mask;
        auto bucket_number = dir[suffix];
        auto bucket = DistinctBindingHashBucket<T>(buckets_file_id, bucket_number, tuple_size);

        bool need_split;
        bool is_in = bucket.is_in_or_insert(tuple, hash, &need_split);
        if (need_split) {
            if (*bucket.local_depth < global_depth) {
                // new_bucket_number = 2^local_depth + bucket_number
                const auto new_bucket_number = bucket_number | (1 << (*bucket.local_depth));
                ++(*bucket.local_depth);
                const auto new_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - (*bucket.local_depth));
                DistinctBindingHashBucket<T> new_bucket(buckets_file_id, new_bucket_number, tuple_size);
                *new_bucket.tuple_count = 0;
                (*new_bucket.local_depth) = (*bucket.local_depth);

                // redistribute keys between buckets `0|bucket_number` and `1|bucket_number`
                bucket.redistribute(new_bucket, new_mask, new_bucket_number);

                // update dirs having `new_bucket_number` suffix and point to the new_bucket
                const auto update_dir_count = 1 << (global_depth - (*bucket.local_depth));
                for (auto i = 0; i < update_dir_count; ++i) {
                    dir[(i << (*bucket.local_depth)) | new_bucket_number] = new_bucket_number;
                }

            } else {
                assert(suffix == bucket_number && "EXTENDIBLE HASH INCONSISTENCY: suffix != bucket_number");
                assert(*bucket.local_depth == global_depth && "EXTENDIBLE HASH INCONSISTENCY: *bucket.local_depth != global_depth");
                ++(*bucket.local_depth);

                // new_bucket_number = 2^local_depth + bucket_number
                const auto new_bucket_number = bucket_number | (1 << global_depth);
                DistinctBindingHashBucket<T> new_bucket(buckets_file_id, new_bucket_number, tuple_size);
                *new_bucket.tuple_count = 0;
                *new_bucket.local_depth = *bucket.local_depth;

                duplicate_dirs();

                // redistribute keys between buckets `0|bucket_number` and `1|bucket_number`
                const auto new_mask = 0xFFFF'FFFF'FFFF'FFFF >> (64 - global_depth);
                bucket.redistribute(new_bucket, new_mask, new_bucket_number);

                // update dir for `1|bucket_number`
                dir[new_bucket_number] = new_bucket_number;
            }
        }
        else {
            return is_in;
        }
    }
}


template <class T>
void DistinctBindingHash<T>::duplicate_dirs() {
    uint64_t old_dir_size = 1UL << global_depth;
    ++global_depth;
    auto new_dir_size = 1UL << global_depth;
    auto new_dir = new uint_fast32_t[new_dir_size];

    std::memcpy(
        new_dir,
        dir,
        old_dir_size * sizeof(uint_fast32_t)
    );
    // new pointers should still point to old buckets (no need to create all buckets at once when duplicating)
    std::memcpy(
        &new_dir[old_dir_size],
        dir,
        old_dir_size * sizeof(uint_fast32_t)
    );

    delete[](dir);
    dir = new_dir;
}

template class DistinctBindingHash<GraphObject>;
template class DistinctBindingHash<ObjectId>;
