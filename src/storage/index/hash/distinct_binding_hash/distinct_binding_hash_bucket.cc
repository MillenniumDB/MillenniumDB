#include "distinct_binding_hash_bucket.h"

#include <cstring>

#include "graph_models/object_id.h"
#include "system/buffer_manager.h"


DistinctBindingHashBucket::DistinctBindingHashBucket(
    TmpFileId file_id,
    uint_fast32_t bucket_number,
    uint_fast32_t tuple_size
) :
    page        (buffer_manager.get_ppage(file_id, bucket_number)),
    tuple_size  (tuple_size),
    max_tuples  ( (PPage::SIZE - sizeof(*tuple_count) - sizeof(local_depth))
                  / (sizeof(*hashes) + tuple_size*sizeof(ObjectId) ) ),
    tuples      (reinterpret_cast<ObjectId*>(page.get_bytes())),
    hashes      (reinterpret_cast<uint64_t*>(page.get_bytes() + tuple_size*max_tuples*sizeof(ObjectId))),
    tuple_count (reinterpret_cast<uint8_t*> (reinterpret_cast<uint8_t*>(hashes) + max_tuples*sizeof(*hashes))),
    local_depth (reinterpret_cast<uint8_t*> (reinterpret_cast<uint8_t*>(tuple_count) + sizeof(*tuple_count))) { }


DistinctBindingHashBucket::~DistinctBindingHashBucket() {
    buffer_manager.unpin(page);
}


bool DistinctBindingHashBucket::is_in(const std::vector<ObjectId>& tuple, uint64_t hash)
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



bool DistinctBindingHashBucket::is_in_or_insert(const std::vector<ObjectId>& tuple,
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



void DistinctBindingHashBucket::redistribute(DistinctBindingHashBucket& other,
                                             uint64_t mask,
                                             uint64_t other_suffix)
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
                tuple_size * sizeof(ObjectId)
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
                    tuple_size * sizeof(ObjectId)
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
