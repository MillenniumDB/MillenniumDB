#ifndef STORAGE__OBJECT_FILE_HASH_BUCKET_H_
#define STORAGE__OBJECT_FILE_HASH_BUCKET_H_

#include <cstdint>
#include <map>
#include <string>

#include "storage/file_id.h"
#include "storage/index/object_file/object_file.h"
#include "storage/page.h"

class ObjectFileHashBucket {
friend class ObjectFileHash;

// 2 bytes needed for key_count and local_depth, 2*8 bytes for the hash
// and 6 bytes for the id (it assumes the other 2 bytes of the id are 0x00)
// TODO: maybe 5 bytes is enough => ~1TB for object_file
static constexpr auto BYTES_FOR_ID = 6U;
static constexpr auto MAX_KEYS = (Page::MDB_PAGE_SIZE - 2*sizeof(uint8_t)) / (2*sizeof(uint64_t) + BYTES_FOR_ID);
static_assert(MAX_KEYS <= UINT8_MAX, "ObjectFileHashBucket KEY_COUNT(UINT8) CAN'T REACH MAX_KEYS");

public:
    ObjectFileHashBucket(FileId file_id, uint_fast32_t bucket_number, ObjectFile& objecy_file);
    ~ObjectFileHashBucket();

    uint64_t get_id(const std::string& str, const uint64_t hash1, const uint64_t hash2) const;

    uint64_t get_or_create_id(const std::string& str, const uint64_t hash1, const uint64_t hash2,
                              bool* const need_split, bool* const created);

private:
    Page& page;
    ObjectFile& object_file;

    uint64_t* const hashes; // each tuple is (hash1, hash2)
    uint8_t*  const key_count;
    uint8_t*  const local_depth;
    uint8_t*  const ids;

    void write_id(const uint64_t id, const uint_fast32_t index);
    uint64_t read_id(const uint_fast32_t index) const;

    void redistribute(ObjectFileHashBucket& other, const uint64_t mask, const uint64_t other_suffix);
};

#endif // STORAGE__OBJECT_FILE_HASH_BUCKET_H_
