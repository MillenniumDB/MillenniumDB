#include "strings_hash_bucket.h"

#include <bitset>
#include <cstring>
#include <memory>

#include "base/ids/object_id.h"
#include "storage/buffer_manager.h"
#include "storage/string_manager.h"

using namespace std;

StringsHashBucket::StringsHashBucket(FileId file_id, uint_fast32_t bucket_number) :
    page        (buffer_manager.get_page(file_id, bucket_number)),
    key_count   (reinterpret_cast<uint32_t*>(page.get_bytes())),
    local_depth (reinterpret_cast<uint32_t*>(page.get_bytes() + sizeof(uint32_t))),
    hashes      (reinterpret_cast<uint64_t*>(page.get_bytes() + 2*sizeof(uint32_t))),
    ids         (reinterpret_cast<uint64_t*>(page.get_bytes() + sizeof(uint64_t) + sizeof(uint64_t)*StringsHashBucket::MAX_KEYS)) { }


StringsHashBucket::~StringsHashBucket() {
    buffer_manager.unpin(page);
}


uint64_t StringsHashBucket::get_str_id(const string& str, uint64_t hash) const {
    for (size_t i = 0; i < *key_count; ++i) {
        if (hashes[i] == hash) {
            // check if object is
            auto id = ids[i];
            if (string_manager.str_eq(str, id)) {
                return id;
            }
        }
    }
    return ObjectId::OBJECT_ID_NOT_FOUND;
}


uint64_t StringsHashBucket::get_or_create_str_id(const string& str, uint64_t hash, bool* need_split) {
    for (size_t i = 0; i < *key_count; ++i) {
        if (hashes[i] == hash) {
            // check if object is
            auto id = ids[i];
            if (string_manager.str_eq(str, id)) {
                *need_split = false;
                return id;
            }
        }
    }
    if (*key_count == MAX_KEYS) {
        *need_split = true;
        return 0; // doesn't matter this returned value, ExtendibleHash needs to try to insert again
    }

    auto new_id = string_manager.create_new(str);

    hashes[*key_count] = hash;

    ids[*key_count] = new_id;
    ++(*key_count);
    page.make_dirty();

    *need_split = false;
    return new_id;
}


void StringsHashBucket::redistribute(StringsHashBucket& other, uint64_t mask, uint64_t other_suffix) {
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
