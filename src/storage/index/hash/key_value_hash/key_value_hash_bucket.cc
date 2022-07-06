#include "key_value_hash_bucket.h"

#include <cstring>
#include <iostream>

#include "storage/buffer_manager.h"
#include "base/ids/object_id.h"

using namespace std;

template <class K, class V>
KeyValueHashBucket<K, V>::KeyValueHashBucket(TmpFileId     file_id,
                                             uint_fast32_t bucket_number,
                                             std::size_t   _key_size,
                                             std::size_t   _value_size,
                                             uint32_t      _max_tuples) :

    page        (buffer_manager.get_tmp_page(file_id, bucket_number)),
    key_size    (_key_size),
    value_size  (_value_size),
    max_tuples  (_max_tuples),
    tuple_count ( reinterpret_cast<uint64_t*>(page.get_bytes() ) ),
    keys        ( reinterpret_cast<K*>(page.get_bytes() + sizeof(uint64_t)) ),
    values      ( reinterpret_cast<V*>(page.get_bytes() + sizeof(uint64_t) + (sizeof(K) * key_size * max_tuples) ))
    {}


template <class K, class V>
KeyValueHashBucket<K, V>::~KeyValueHashBucket() {
    //page.make_dirty();
    buffer_manager.unpin(page);
}


template <class K, class V>
void KeyValueHashBucket<K, V>::insert(const vector<K>& key, const vector<V>& value) {
    for (uint_fast16_t i = 0; i < key_size; i++) {
        keys[(key_size * (*tuple_count)) + i] = key[i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        values[(value_size * (*tuple_count)) + i] = value[i];
    }
    ++(*tuple_count);
    page.make_dirty();
}


template <class K, class V>
void KeyValueHashBucket<K, V>::insert_in_pos(const K* key, const V* value, uint_fast32_t pos) {
    for (uint_fast16_t i = 0; i < key_size; i++) {
        keys[(key_size * pos) + i] = key[i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        values[(value_size * pos) + i] = value[i];
    }
    page.make_dirty();
}


template <class K, class V>
void KeyValueHashBucket<K, V>::insert_with_pointers(const K* key, const V* value) {
    for (uint_fast16_t i = 0; i < key_size; i++) {
        keys[(key_size * (*tuple_count)) + i] = key[i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        values[(value_size * (*tuple_count)) + i] = value[i];
    }
    ++(*tuple_count);
    page.make_dirty();
}


template <class K, class V>
pair<vector<K>, vector<V>> KeyValueHashBucket<K, V>::get_pair(std::uint_fast32_t pos) const {
    vector<K> key;
    vector<V> value;
    key.resize(key_size);
    value.resize(value_size);
    for (uint_fast16_t i = 0; i < key_size; i++) {
        key[i] = keys[(key_size * pos) + i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        value[i] = values[(value_size * pos) + i];
    }
    return make_pair(move(key), move(value));
}


template <class K, class V>
void KeyValueHashBucket<K, V>::sort() {
    if (*tuple_count > 1) {
        partition(0, (*tuple_count) - 1);
    }
}


template <class K, class V>
void KeyValueHashBucket<K, V>::partition(uint_fast32_t min_pos, uint_fast32_t max_pos) {
    if (max_pos <= min_pos) {
        return;
    }
    vector<K> pivot_key;
    vector<V> pivot_value;
    pivot_key.resize(key_size);
    pivot_value.resize(value_size);
    for (uint_fast16_t i = 0; i < key_size; i++) {
        pivot_key[i] = keys[(key_size * max_pos) + i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        pivot_value[i] = values[(value_size * max_pos) + i];
    }

    vector<K> aux_key;
    vector<V> aux_value;
    aux_key.resize(key_size);
    aux_value.resize(value_size);

    uint_fast32_t insert_min_pos = min_pos;
    uint_fast32_t insert_max_pos =  max_pos - 1;

    while (insert_min_pos != insert_max_pos) {
        bool smaller = false;
        for (uint_fast16_t i = 0; i < key_size; i++) {
            if (keys[(key_size * insert_max_pos) + i] < pivot_key[i]) {  // 212 //121
                smaller = true;
                break;
            }
            else if (keys[(key_size * insert_max_pos) + i] > pivot_key[i]) {
                break;
            }
        }
        for (uint_fast16_t i = 0; i < key_size; i++) {
            aux_key[i] = keys[(key_size * insert_max_pos) + i];
        }
        for (uint_fast16_t i = 0; i < value_size; i++) {
            aux_value[i] = values[(value_size * insert_max_pos) + i];
        }
        if (smaller) { // swap curr_pos (max) with insert_min_pos
            for (uint_fast16_t i = 0; i < key_size; i++) {
                keys[(key_size * insert_max_pos) + i] = keys[(key_size * insert_min_pos) + i];
            }
            for (uint_fast16_t i = 0; i < value_size; i++) {
                values[(value_size * insert_max_pos) + i] = values[(value_size * insert_min_pos) + i];
            }
            for (uint_fast16_t i = 0; i < key_size; i++) {
                keys[(key_size * insert_min_pos) + i] = aux_key[i];
            }
            for (uint_fast16_t i = 0; i < value_size; i++) {
                values[(value_size * insert_min_pos) + i] = aux_value[i];
            }
            insert_min_pos += 1;
        }
        else {  // insert_max_pos is equal or bigger
            insert_max_pos -= 1;
        }
    }
    // check last, compare with pivot this time
    bool smaller = false;
    for (uint_fast16_t i = 0; i < key_size; i++) {
        if (keys[(key_size * insert_max_pos) + i] < pivot_key[i]) {
            smaller = true;
        }
    }
    if (smaller) { // change next with pivot
        insert_min_pos += 1;
    }
    // insert_min_pos is where the pivot is inserted now
    for (uint_fast16_t i = 0; i < key_size; i++) {
        keys[(key_size * max_pos) + i] = keys[(key_size * insert_min_pos) + i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        values[(value_size * max_pos) + i] = values[(value_size * insert_min_pos) + i];
    }
    for (uint_fast16_t i = 0; i < key_size; i++) {
        keys[(key_size * insert_min_pos) + i] = pivot_key[i];
    }
    for (uint_fast16_t i = 0; i < value_size; i++) {
        values[(value_size * insert_min_pos) + i] = pivot_value[i];
    }
    if (insert_min_pos != 0) {
        partition(min_pos, insert_min_pos - 1);
    }
    if (insert_min_pos < max_pos) {
        partition(insert_min_pos + 1, max_pos);
    }
}

// alignment works well for <ObjectId, ObjectId> but need to analyze for other cases
template class KeyValueHashBucket<ObjectId, ObjectId>;
