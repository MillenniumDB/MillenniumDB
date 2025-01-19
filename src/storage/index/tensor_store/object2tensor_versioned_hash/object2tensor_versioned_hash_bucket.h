#pragma once

#include <cstdint>

#include "storage/page/versioned_page.h"

/**
 * Buckets for ObjectIdVersionedHash. Hashes generated MUST be collision free or this structure
 * will not work as expected. One solution due sizeof(ObjectIds) == sizeof(Hash) is bit shuffling.
 *
 * Page structure:
 *   Header   header
 *   uint64_t hashes[MAX_ENTRIES]
 *   uint32_t tensor_index[MAX_ENTRIES]
 */
class Object2TensorVersionedHashBucket {
    friend class Object2TensorVersionedHash;

public:
    struct Header {
        uint32_t num_entries;
        uint32_t local_depth;
    };

    static constexpr auto MAX_ENTRIES = (VPage::SIZE - sizeof(Header))
                                      / (sizeof(uint64_t) + sizeof(uint32_t));

    explicit Object2TensorVersionedHashBucket(VPage& page);

    ~Object2TensorVersionedHashBucket();

    bool contains(uint64_t hash) const;

    // Write into value if there is an entry with the same hash. Returns false if there is no such entry
    bool get(uint64_t hash, uint32_t* tensor_index) const;

    // Insert a new entry into the bucket
    void insert(uint64_t hash, uint32_t tensor_index);

    // Removes the entry with the same hash. Returns false if there is no such entry
    bool remove(uint64_t hash);

private:
    VPage& page;

    Header* const header;
    uint64_t* const hashes;
    uint32_t* const values;

    void redistribute(
        Object2TensorVersionedHashBucket& new_bucket,
        uint64_t split_mask,
        uint64_t split_suffix
    );
};
