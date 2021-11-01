#include "object_file_hash_bucket.h"

#include <bitset>
#include <cstring>
#include <iostream>
#include <memory>

#include "storage/buffer_manager.h"

using namespace std;

ObjectFileHashBucket::ObjectFileHashBucket(FileId file_id, uint_fast32_t bucket_number, ObjectFile& object_file) :
    page        (buffer_manager.get_page(file_id, bucket_number)),
    object_file (object_file),
    hashes      (reinterpret_cast<uint64_t*>(page.get_bytes())),
    key_count   (reinterpret_cast<uint8_t*>(page.get_bytes() + 2*sizeof(uint64_t)*MAX_KEYS)),
    local_depth (reinterpret_cast<uint8_t*>(page.get_bytes() + 2*sizeof(uint64_t)*MAX_KEYS + sizeof(uint8_t))),
    ids         (reinterpret_cast<uint8_t*>(page.get_bytes() + 2*sizeof(uint64_t)*MAX_KEYS + 2*sizeof(uint8_t)))
{ }


ObjectFileHashBucket::~ObjectFileHashBucket() {
    buffer_manager.unpin(page);
}


uint64_t ObjectFileHashBucket::get_id(const string& str, const uint64_t hash1, const uint64_t hash2) const {
    for (uint8_t i = 0; i < *key_count; ++i) {
        if (hashes[2*i] == hash1 && hashes[2*i + 1] == hash2) {
            // check if object is
            auto id = read_id(i);
            auto c_str = object_file.read(id);
            if (str == c_str) {
                return id;
            }
        }
    }
    return ObjectId::OBJECT_ID_NOT_FOUND;
}


uint64_t ObjectFileHashBucket::get_or_create_id(const string& str,
                                                const uint64_t hash1,
                                                const uint64_t hash2,
                                                bool* const need_split,
                                                bool* const created)
{
    for (uint8_t i = 0; i < *key_count; ++i) {
        if (hashes[2*i] == hash1 && hashes[2*i + 1] == hash2) {
            // check if object is
            auto id = read_id(i);
            auto c_str = object_file.read(id);
            if (str == c_str) {
                *created = false;
                *need_split = false;
                return id;
            }
        }
    }
    if (*key_count == MAX_KEYS) {
        *need_split = true;
        return 0; // doesn't matter this returned value, ExtendibleHash needs to try to insert again
    }

    auto bytes = make_unique<vector<unsigned char>>(str.length());
    copy(str.begin(), str.end(), bytes->begin());
    auto new_id = object_file.write(*bytes);

    hashes[2 * (*key_count)]     = hash1;
    hashes[2 * (*key_count) + 1] = hash2;

    write_id(new_id, *key_count);
    ++(*key_count);
    page.make_dirty();

    *created = true;
    *need_split = false;
    return new_id;
}

void ObjectFileHashBucket::write_id(const uint64_t id, const uint_fast32_t i) {
    const auto offset = BYTES_FOR_ID*i;

    for (uint_fast8_t b = 0; b < BYTES_FOR_ID; b++) {
        ids[offset + b] = static_cast<uint8_t>( (id >> (8UL*b)) & 0xFF );
    }
}


uint64_t ObjectFileHashBucket::read_id(const uint_fast32_t i) const {
    const auto offset = BYTES_FOR_ID*i;

    uint64_t res = 0;
    for (uint_fast8_t b = 0; b < BYTES_FOR_ID; b++) {
        res += static_cast<uint64_t>(ids[offset + b]) <<  8UL*b;
    }
    return res;
}


void ObjectFileHashBucket::redistribute(ObjectFileHashBucket& other, const uint64_t mask, const uint64_t other_suffix) {
    uint8_t other_pos = 0;
    uint8_t this_pos = 0;

    for (uint8_t i = 0; i < *key_count; i++) {
        auto suffix = mask & hashes[2 * i];

        if (suffix == other_suffix) {
            // copy hash to other bucket
            std::memcpy(
                &other.hashes[2*other_pos],
                &hashes[2*i],
                2 * sizeof(uint64_t)
            );

            // copy id to ohter bucket
            std::memcpy(
                &other.ids[BYTES_FOR_ID*other_pos],
                &ids[BYTES_FOR_ID*i],
                BYTES_FOR_ID * sizeof(uint8_t)
            );
            ++other_pos;
        } else {
            if (i != this_pos) { // avoid redundant copy
                // copy hash in this bucket
                std::memcpy(
                    &hashes[2*this_pos],
                    &hashes[2*i],
                    2 * sizeof(uint64_t)
                );

                // copy id in this bucket
                std::memcpy(
                    &ids[BYTES_FOR_ID*this_pos],
                    &ids[BYTES_FOR_ID*i],
                    BYTES_FOR_ID * sizeof(uint8_t)
                );
            }
            ++this_pos;
        }
    }
    *this->key_count = this_pos;
    *other.key_count = other_pos;
    this->page.make_dirty();
    other.page.make_dirty();
}
