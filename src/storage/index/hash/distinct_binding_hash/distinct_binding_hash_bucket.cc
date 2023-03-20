#include "distinct_binding_hash_bucket.h"

#include <cstring>

#include "storage/buffer_manager.h"
#include "base/ids/object_id.h"
#include "base/graph_object/graph_object.h"

using namespace std;

template <class T>
DistinctBindingHashBucket<T>::DistinctBindingHashBucket(const TmpFileId file_id,
                                                        const uint_fast32_t bucket_number,
                                                        std::size_t tuple_size) :
    page        (buffer_manager.get_tmp_page(file_id, bucket_number)),
    tuple_size  (tuple_size),
    max_tuples  ( (Page::MDB_PAGE_SIZE - sizeof(*tuple_count) - sizeof(local_depth))
                  / (sizeof(*hashes) + tuple_size*sizeof(T) ) ),
    tuples      (reinterpret_cast<T*>(page.get_bytes())),
    hashes      (reinterpret_cast<uint64_t*>(page.get_bytes() + tuple_size*max_tuples*sizeof(T))),
    tuple_count (reinterpret_cast<uint8_t*> (reinterpret_cast<uint8_t*>(hashes) + max_tuples*sizeof(*hashes))),
    local_depth (reinterpret_cast<uint8_t*> (reinterpret_cast<uint8_t*>(tuple_count) + sizeof(*tuple_count))) { }


template <class T>
DistinctBindingHashBucket<T>::~DistinctBindingHashBucket() {
    buffer_manager.unpin(page);
}


template <class T>
bool DistinctBindingHashBucket<T>::is_in(const std::vector<T>& tuple, uint64_t hash)
{
    for (uint8_t i = 0; i < *tuple_count; ++i) {
        if (hashes[i] == hash) {
            bool tuple_found = true;
            // compare objects (in case we have a collision)
            for (uint_fast16_t j = 0; j < tuple_size; j++) {
                if (tuple[j] != tuples[(tuple_size * i) + j]) {
                    tuple_found = false;
                    break;
                }
            }
            if (tuple_found) {
                return true;
            }
        }
    }
    return false;
}


template <class T>
bool DistinctBindingHashBucket<T>::is_in_or_insert(const std::vector<T>& tuple,
                                                   uint64_t hash,
                                                   bool* const need_split)
{
    for (uint8_t i = 0; i < *tuple_count; ++i) {
        if (hashes[i] == hash) {
            bool tuple_found = true;
            // compare objects (in case we have a collision)
            for (uint_fast16_t j = 0; j < tuple_size; j++) {
                if (tuple[j] != tuples[(tuple_size * i) + j]) {
                    tuple_found = false;
                    break;
                }
            }
            if (tuple_found) {
                *need_split = false;
                return true;
            }
        }
    }
    if (*tuple_count == max_tuples) {
        *need_split = true;
        return false; // needs to try to insert again
    }

    hashes[(*tuple_count)] = hash;

    for (uint_fast16_t i = 0; i < tuple_size; i++) {
        tuples[(tuple_size * (*tuple_count)) + i] = tuple[i];
    }
    ++(*tuple_count);
    page.make_dirty();

    *need_split = false;
    return false;
}


template <class T>
void DistinctBindingHashBucket<T>::redistribute(DistinctBindingHashBucket<T>& other,
                                                const uint64_t mask,
                                                const uint64_t other_suffix)
{
    uint8_t other_pos = 0;
    uint8_t this_pos = 0;

    for (uint8_t i = 0; i < *tuple_count; i++) {
        auto suffix = mask & hashes[i];

        if (suffix == other_suffix) {
            // copy hash to other bucket
            other.hashes[other_pos] = hashes[i];
            // copy objects to other bucket
            std::memcpy(
                &other.tuples[tuple_size * other_pos],
                &tuples[tuple_size*i],
                tuple_size * sizeof(T)
            );
            ++other_pos;
        } else {
            if (i != this_pos) { // avoid redundant copy
                // copy hash in this bucket
                hashes[this_pos] = hashes[i];
                // copy objects in this bucket
                std::memcpy(
                    &tuples[tuple_size * this_pos],
                    &tuples[tuple_size *i],
                    tuple_size * sizeof(T)
                );
            }
            ++this_pos;
        }
    }
    // we are making the supposition that at least one suffix is different (otherwise it will split forever)
    *this->tuple_count = this_pos;
    *other.tuple_count = other_pos;
    this->page.make_dirty();
    other.page.make_dirty();
}

template class DistinctBindingHashBucket<GraphObject>;
template class DistinctBindingHashBucket<ObjectId>;
