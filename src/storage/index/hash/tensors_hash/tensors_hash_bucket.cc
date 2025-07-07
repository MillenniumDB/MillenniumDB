#include "tensors_hash_bucket.h"

#include <cstring>

#include "graph_models/object_id.h"
#include "storage/index/hash/strings_hash/strings_hash.h"
#include "system/buffer_manager.h"
#include "system/tensor_manager.h"

TensorsHashBucket::TensorsHashBucket(UPage& page) :
    page { page },
    key_count { reinterpret_cast<uint32_t*>(page.get_bytes()) },
    local_depth { reinterpret_cast<uint32_t*>(page.get_bytes() + sizeof(uint32_t)) },
    arr1 { reinterpret_cast<uint64_t*>(page.get_bytes() + 2 * sizeof(uint32_t)) },
    arr2 { reinterpret_cast<uint32_t*>(
        page.get_bytes() + 2 * sizeof(uint32_t) + sizeof(uint64_t) * TensorsHashBucket::MAX_KEYS
    ) }
{ }

TensorsHashBucket::~TensorsHashBucket()
{
    buffer_manager.unpin(page);
}

uint64_t TensorsHashBucket::get_id(const char* bytes, uint64_t size, uint64_t hash) const
{
    const uint64_t hash_ = (hash >> StringsHash::MIN_GLOBAL_DEPTH) & HASH_MASK;
    for (size_t i = 0; i < *key_count; ++i) {
        if ((arr1[i] & HASH_MASK) == hash_) {
            // check object
            const uint64_t arr_1_bits = arr1[i] >> 52ULL;
            const uint64_t id = (arr_1_bits << 32ULL) | static_cast<uint64_t>(arr2[i]);
            if (tensor_manager.bytes_eq(bytes, size, id)) {
                return id;
            }
        }
    }
    return ObjectId::MASK_NOT_FOUND;
}

void TensorsHashBucket::create_id(uint64_t new_id, uint64_t hash)
{
    assert(*key_count < MAX_KEYS);
    const uint64_t hash_ = (hash >> StringsHash::MIN_GLOBAL_DEPTH) & HASH_MASK;

    uint64_t new_arr1_val = (new_id & 0x00'00'0F'FF'00'00'00'00ULL) << 20ULL;
    new_arr1_val |= hash_;

    arr1[*key_count] = new_arr1_val;
    arr2[*key_count] = static_cast<uint32_t>(new_id & 0xFF'FF'FF'FFUL);

    ++(*key_count);
    page.make_dirty();
}

void TensorsHashBucket::redistribute(TensorsHashBucket& other, uint64_t mask, uint64_t other_suffix)
{
    assert(*key_count == MAX_KEYS);

    uint32_t other_pos = 0;
    uint32_t this_pos = 0;

    const uint64_t mask_ = mask >> StringsHash::MIN_GLOBAL_DEPTH;
    uint64_t other_suffix_ = other_suffix >> StringsHash::MIN_GLOBAL_DEPTH;

    for (std::size_t i = 0; i < *key_count; ++i) {
        if ((mask_ & arr1[i]) == other_suffix_) {
            other.arr1[other_pos] = arr1[i];
            other.arr2[other_pos] = arr2[i];

            ++other_pos;
        } else {
            arr1[this_pos] = arr1[i];
            arr2[this_pos] = arr2[i];

            ++this_pos;
        }
    }
    *this->key_count = this_pos;
    *other.key_count = other_pos;

    page.make_dirty();
    // other.page should be already dirty
}
