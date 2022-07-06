#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "storage/file_id.h"
#include "storage/page.h"

template<class K, class V> class KeyValueHash;

template <class K, class V>
class KeyValueHashBucket {

friend class KeyValueHash<K, V>;

public:
    KeyValueHashBucket(TmpFileId     file_id,
                       uint_fast32_t bucket_number,
                       std::size_t   key_size,
                       std::size_t   value_size,
                       uint32_t      max_tuples);

    ~KeyValueHashBucket();

    void insert(const std::vector<K>& key, const std::vector<V>& value);
    void insert_in_pos(const K* key, const V* value, uint_fast32_t pos);  // for split
    void insert_with_pointers(const K* key, const V* value);  // for merge
    std::pair<std::vector<K>, std::vector<V>> get_pair(uint_fast32_t current_pos) const;
    inline K* get_key(uint_fast32_t current_pos) const { return &keys[current_pos*key_size]; }
    inline V* get_value(uint_fast32_t current_pos) const { return &values[current_pos*value_size]; }
    void sort();
    // quicksort in place
    void partition(uint_fast32_t min_pos, uint_fast32_t max_pos);

    inline uint_fast32_t get_tuple_count() const noexcept { return *tuple_count; }
    inline void set_tuple_count(uint_fast32_t value) const noexcept { *tuple_count = value; }

private:
    Page& page;

    uint32_t   const key_size;
    uint32_t   const value_size;
    uint32_t   const max_tuples;
    uint64_t*  const tuple_count;  // we use 64 bits for alignment
    K*         const keys;
    V*         const values;
};
