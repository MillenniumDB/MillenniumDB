#ifndef STORAGE__KEY_VALUE_HASH__H_
#define STORAGE__KEY_VALUE_HASH__H_

#include <cstdint>
#include <queue>
#include <map>
#include <utility>
#include <vector>

#include "base/ids/var_id.h"
#include "storage/file_id.h"
#include "storage/index/object_file/object_file.h"
#include "storage/index/hash/key_value_hash/key_value_hash_bucket.h"

// KeyValueHash is used to save two vectors, key is a vector of type K and value a vector of type V, keys and values
// must have a constant size. 
// Sorting buckets is optional, but needed if you want to use find_first method, otherwise you must search in all the 
// bucket to check if a key is present (and could be more than once)

template <class K, class V>
class KeyValueHash {
public:
    static constexpr uint_fast32_t DEFAULT_INITIAL_DEPTH = 8;

    KeyValueHash(std::size_t key_size, std::size_t value_size);
    ~KeyValueHash();

    void begin(uint_fast32_t initial_depth = DEFAULT_INITIAL_DEPTH);
    void reset(uint_fast32_t new_depth = DEFAULT_INITIAL_DEPTH);
    void insert(const std::vector<K>& key, const std::vector<V>& value);
    void sort_buckets();
    bool find_first(const std::vector<K>& current_key, uint_fast32_t current_bucket, uint_fast32_t* current_bucket_pos);

    inline uint_fast32_t get_depth() const noexcept { return depth; }

    inline uint_fast32_t get_bucket_size(uint_fast32_t bucket_number) const {
        return buckets_sizes[bucket_number];
    }

    std::pair<std::vector<K>, std::vector<V>> get_pair(uint_fast32_t current_bucket, uint_fast32_t current_pos);
    K* get_key(uint_fast32_t current_bucket, uint_fast32_t current_pos);
    V* get_value(uint_fast32_t current_bucket, uint_fast32_t current_pos);

    // split directory and redistribute every bucket
    void split();

    uint_fast32_t get_bucket(const std::vector<K>& key) const;
    void check_order();
    void sort_bucket(uint_fast32_t bucket_number);

private:
    const std::size_t key_size;
    const std::size_t value_size;
    const uint32_t    max_tuples;

    uint_fast32_t     last_page_number;
    const TmpFileId   buckets_file;

    uint64_t      tuples_count;
    uint_fast32_t depth;

    std::vector<std::vector<uint_fast32_t>> buckets_page_numbers;
    std::vector<uint_fast32_t>              buckets_sizes;

    std::queue<uint_fast32_t> available_pages;

    inline uint_fast32_t get_split_treshold() const noexcept { return max_tuples * (1 << depth); }
    uint_fast32_t get_new_page_number();
};

#endif // STORAGE__KEY_VALUE_HASH__H_
