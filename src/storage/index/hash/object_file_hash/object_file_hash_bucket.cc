#include "object_file_hash_bucket.h"

#include <bitset>
#include <cstring>
#include <memory>

#include "storage/buffer_manager.h"

using namespace std;

ObjectFileHashBucket::ObjectFileHashBucket(FileId file_id, uint_fast32_t bucket_number, ObjectFile& object_file) :
    page        (buffer_manager.get_page(file_id, bucket_number)),
    object_file (object_file),
    key_count   (reinterpret_cast<uint32_t*>(page.get_bytes())),
    local_depth (reinterpret_cast<uint32_t*>(page.get_bytes() + sizeof(uint32_t))),
    hashes      (reinterpret_cast<uint64_t*>(page.get_bytes() + 2*sizeof(uint32_t))),
    ids         (reinterpret_cast<uint64_t*>(page.get_bytes() + sizeof(uint64_t) + sizeof(uint64_t)*ObjectFileHashBucket::MAX_KEYS)) { }


ObjectFileHashBucket::~ObjectFileHashBucket() {
    buffer_manager.unpin(page);
}


uint64_t ObjectFileHashBucket::get_id(const string& str, const uint64_t hash) const {
    for (size_t i = 0; i < *key_count; ++i) {
        if (hashes[i] == hash) {
            // check if object is
            auto id = ids[i];
            auto str2 = object_file.get_string(id);
            if (str == str2) {
                return id;
            }
        }
    }
    return ObjectId::OBJECT_ID_NOT_FOUND;
}


uint64_t ObjectFileHashBucket::get_or_create_id(const string& str,
                                                const uint64_t hash,
                                                bool* const need_split,
                                                bool* const created)
{
    for (size_t i = 0; i < *key_count; ++i) {
        if (hashes[i] == hash) {
            // check if object is
            auto id = ids[i];
            auto str2 = object_file.get_string(id);
            if (str == str2) {
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

    auto new_id = object_file.write(str);

    hashes[*key_count] = hash;

    ids[*key_count] = new_id;
    ++(*key_count);
    page.make_dirty();

    *created = true;
    *need_split = false;
    return new_id;
}


void ObjectFileHashBucket::redistribute(ObjectFileHashBucket& other, const uint64_t mask, const uint64_t other_suffix) {
    uint32_t other_pos = 0;
    uint32_t this_pos = 0;

    for (size_t i = 0; i < *key_count; i++) {
        auto suffix = mask & hashes[i];

        if (suffix == other_suffix) {
            other.hashes[other_pos] = hashes[i];
            other.ids[other_pos] = ids[i];

            ++other_pos;
        } else {
            hashes[this_pos] = hashes[i];
            ids[this_pos] = ids[i];

            ++this_pos;
        }
    }
    *this->key_count = this_pos;
    *other.key_count = other_pos;
}
