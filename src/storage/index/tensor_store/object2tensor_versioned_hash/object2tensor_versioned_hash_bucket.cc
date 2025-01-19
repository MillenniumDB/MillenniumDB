#include "object2tensor_versioned_hash_bucket.h"

#include "system/buffer_manager.h"

Object2TensorVersionedHashBucket::Object2TensorVersionedHashBucket(VPage& page) :
    page { page },
    header { reinterpret_cast<Header*>(page.get_bytes()) },
    hashes { reinterpret_cast<uint64_t*>(page.get_bytes() + sizeof(Header)) },
    values { reinterpret_cast<uint32_t*>(page.get_bytes() + sizeof(Header) + sizeof(uint64_t) * MAX_ENTRIES) }
{ }

Object2TensorVersionedHashBucket::~Object2TensorVersionedHashBucket()
{
    buffer_manager.unpin(page);
}

bool Object2TensorVersionedHashBucket::contains(uint64_t hash) const
{
    for (uint32_t i = 0; i < header->num_entries; ++i) {
        if (hashes[i] == hash) {
            return true;
        }
    }

    return false;
}

bool Object2TensorVersionedHashBucket::get(uint64_t hash, uint32_t* tensor_index) const
{
    for (uint32_t i = 0; i < header->num_entries; ++i) {
        if (hashes[i] == hash) {
            *tensor_index = values[i];
            return true;
        }
    }

    return false;
}

void Object2TensorVersionedHashBucket::insert(uint64_t hash, uint32_t value)
{
    assert(buffer_manager.is_editable(page) && "Page is not editable");
    assert(header->num_entries < MAX_ENTRIES && "Bucket is full");

    hashes[header->num_entries] = hash;
    values[header->num_entries] = value;

    ++(header->num_entries);
}

bool Object2TensorVersionedHashBucket::remove(uint64_t hash)
{
    for (uint32_t i = 0; i < header->num_entries; ++i) {
        if (hashes[i] == hash) {
            --(header->num_entries);
            hashes[i] = hashes[header->num_entries];
            values[i] = values[header->num_entries];
            return true;
        }
    }

    return false;
}

void Object2TensorVersionedHashBucket::redistribute(
    Object2TensorVersionedHashBucket& new_bucket,
    uint64_t split_mask,
    uint64_t split_suffix
)
{
    assert(header->num_entries == MAX_ENTRIES && "Bucket is not full");
    assert(buffer_manager.is_editable(page) && "this->page is not editable");
    assert(buffer_manager.is_editable(new_bucket.page) && "new_bucket.page is not editable");

    uint32_t this_pos = 0;
    uint32_t new_pos = 0;

    for (uint32_t i = 0; i < header->num_entries; ++i) {
        if ((hashes[i] & split_mask) == split_suffix) {
            new_bucket.hashes[new_pos] = hashes[i];
            new_bucket.values[new_pos] = values[i];
            ++new_pos;
        } else {
            hashes[this_pos] = hashes[i];
            values[this_pos] = values[i];
            ++this_pos;
        }
    }

    this->header->num_entries = this_pos;
    new_bucket.header->num_entries = new_pos;
}