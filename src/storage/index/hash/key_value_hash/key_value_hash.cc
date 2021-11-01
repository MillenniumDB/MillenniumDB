#include "key_value_hash.h"

#include <cassert>
#include <iostream>

#include "base/ids/object_id.h"
#include "storage/file_manager.h"
#include "storage/buffer_manager.h"
#include "storage/index/hash/hash_functions/hash_function_wrapper.h"
#include "storage/index/hash/key_value_hash/key_value_hash_bucket.h"

using namespace std;

template class KeyValueHash<ObjectId, ObjectId>;

template <class K, class V>
KeyValueHash<K, V>::KeyValueHash(std::size_t key_size, std::size_t value_size) :
    key_size        (key_size),
    value_size      (value_size),
    max_tuples      ( (Page::MDB_PAGE_SIZE - sizeof(uint64_t)) / (key_size*sizeof(K) + value_size*sizeof(V)) ),
    buckets_file    ( file_manager.get_tmp_file_id())
    { }


template <class K, class V>
KeyValueHash<K, V>::~KeyValueHash() {
    //current_buckets_pages.clear();
    file_manager.remove_tmp(buckets_file);
}


template <class K, class V>
void KeyValueHash<K, V>::begin(uint_fast32_t initial_depth) {
    depth = initial_depth;
    const uint_fast32_t number_of_buckets = 1 << depth; // 2^depth

    buckets_sizes.reserve(number_of_buckets);
    buckets_page_numbers.reserve(number_of_buckets);

    last_page_number = -1;  // overflow to start with 0 in loop

    for (uint_fast32_t i = 0; i < number_of_buckets; ++i) {
        buckets_sizes.push_back(0);
        std::vector<uint_fast32_t> vec;
        buckets_page_numbers.push_back(vec);
    }
    tuples_count = 0;
}


template <class K, class V>
void KeyValueHash<K, V>::reset(uint_fast32_t new_depth) {
    depth = new_depth;
    const uint_fast32_t number_of_buckets = 1 << depth; // 2^depth
    last_page_number = -1;

    buckets_sizes.resize(number_of_buckets);
    buckets_page_numbers.resize(number_of_buckets);

    for (uint_fast32_t i = 0; i < number_of_buckets; ++i) {
        buckets_sizes[i] = 0;
        buckets_page_numbers[i].clear();
    }
    while (!available_pages.empty()) {
        available_pages.pop();
    }
    tuples_count = 0;
}


template <class K, class V>
uint_fast32_t KeyValueHash<K, V>::get_new_page_number() {
    if (available_pages.size() > 0) {
        auto res = available_pages.front();
        available_pages.pop();
        return res;
    } else {
        return ++last_page_number;
    }
}


template <class K, class V>
void KeyValueHash<K, V>::insert(const std::vector<K>& key, const std::vector<V>& value) {
    assert(key.size() == key_size);

    uint64_t hash_ = hash_function_wrapper(key.data(), key_size);
    // assuming enough memory in each bucket
    uint64_t mask = (1 << depth) - 1;  // last depth bits
    uint64_t bucket_number = hash_ & mask;  // suffix = bucket_number in this case

    unique_ptr<KeyValueHashBucket<K, V>> current_bucket_page = nullptr;
    uint_fast32_t page_number;
    if (buckets_page_numbers[bucket_number].size() > 0) {
        page_number = buckets_page_numbers[bucket_number].back();
        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                         page_number,
                                                                         key_size,
                                                                         value_size,
                                                                         max_tuples);
    }
    else {
        page_number = get_new_page_number();
        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                         page_number,
                                                                         key_size,
                                                                         value_size,
                                                                         max_tuples);
        *(current_bucket_page->tuple_count) = 0;
        current_bucket_page->page.make_dirty();
        buckets_page_numbers[bucket_number].push_back(page_number);
    }

    while (current_bucket_page->get_tuple_count() >= max_tuples) {
        if (tuples_count >= get_split_treshold()) {  // split treshold should depend on depth or could be infinite
            split();
            mask = (1 << depth) - 1;
            bucket_number = hash_ & mask;
            page_number = buckets_page_numbers[bucket_number].back();
            current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                             page_number,
                                                                             key_size,
                                                                             value_size,
                                                                             max_tuples);
            continue;
        }
        // get page number for new bucket page
        uint_fast32_t new_page_number = get_new_page_number();
        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                         new_page_number,
                                                                         key_size,
                                                                         value_size,
                                                                         max_tuples);
        *(current_bucket_page->tuple_count) = 0;
        current_bucket_page->page.make_dirty();
        buckets_page_numbers[bucket_number].push_back(new_page_number);
    }
    current_bucket_page->insert(key, value);
    buckets_sizes[bucket_number]++;
    tuples_count++;
}


template <class K, class V>
pair<vector<K>, vector<V>> KeyValueHash<K, V>::get_pair(uint_fast32_t bucket_number, uint_fast32_t current_pos) {
    assert(current_pos <= buckets_sizes[bucket_number]);
    uint32_t bucket_page_number = current_pos / max_tuples;
    uint32_t real_page_number = buckets_page_numbers[bucket_number][bucket_page_number];

    auto current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                          real_page_number,
                                                                          key_size,
                                                                          value_size,
                                                                          max_tuples);
    uint32_t page_pos = current_pos % max_tuples;
    return current_bucket_page->get_pair(page_pos);
}


template <class K, class V>
K* KeyValueHash<K, V>::get_key(uint_fast32_t bucket_number,uint_fast32_t current_pos) {
    assert(current_pos <= buckets_sizes[bucket_number]);
    uint32_t bucket_page_number = current_pos / max_tuples;
    uint32_t real_page_number = buckets_page_numbers[bucket_number][bucket_page_number];

    auto current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                          real_page_number,
                                                                          key_size,
                                                                          value_size,
                                                                          max_tuples);
    uint32_t page_pos = current_pos % max_tuples;
    return current_bucket_page->get_key(page_pos);
}


template <class K, class V>
V* KeyValueHash<K, V>::get_value(uint_fast32_t bucket_number,uint_fast32_t current_pos) {
    assert(current_pos <= buckets_sizes[bucket_number]);
    uint32_t bucket_page_number = current_pos / max_tuples;
    uint32_t real_page_number = buckets_page_numbers[bucket_number][bucket_page_number];

    auto current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                          real_page_number,
                                                                          key_size,
                                                                          value_size,
                                                                          max_tuples);
    uint32_t page_pos = current_pos % max_tuples;
    return current_bucket_page->get_value(page_pos);
}


template <class K, class V>
void KeyValueHash<K, V>::split() {
    const uint_fast32_t previous_number_of_buckets = 1 << depth++;
    const uint_fast32_t number_of_buckets = 1 << depth; // 2^new_depth
    buckets_sizes.reserve(number_of_buckets);
    buckets_page_numbers.reserve(number_of_buckets);

    for (uint_fast32_t bucket_number = 0; bucket_number < previous_number_of_buckets; bucket_number++) {
        //uint_fast32_t new_bucket_number = previous_number_of_buckets + bucket_number;
        buckets_page_numbers.push_back(std::vector<uint_fast32_t>());
        buckets_sizes.push_back(0);

        // skip if bucket is empty
        if (buckets_page_numbers[bucket_number].empty()) {
            continue;
        }

        // initialize new bucket
        uint_fast32_t new_page_number = get_new_page_number();
        buckets_page_numbers.back().push_back(new_page_number);
        auto new_bucket_page = make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                     new_page_number,
                                                                     key_size,
                                                                     value_size,
                                                                     max_tuples);
        *(new_bucket_page->tuple_count) = 0;
        new_bucket_page->page.make_dirty();

        // we need an auxiliar bucket for writing tuples, because we are reading with current_buckets_pages
        auto writing_old_bucket_page = make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                             buckets_page_numbers[bucket_number][0],
                                                                             key_size,
                                                                             value_size,
                                                                             max_tuples);

        uint_fast32_t writing_old_pos = 0;  // position in buckets_page_numbers[bucket_number]
        uint_fast32_t writing_old_tuple_count = 0;
        for (auto reading_page_number : buckets_page_numbers[bucket_number]) {
            auto reading_old_bucket_page = make_unique<KeyValueHashBucket<K, V>>(buckets_file,
                                                                                 reading_page_number,
                                                                                 key_size,
                                                                                 value_size,
                                                                                 max_tuples);

            const uint_fast32_t bucket_page_tuples = reading_old_bucket_page->get_tuple_count();

            for (uint_fast32_t tuple_number = 0; tuple_number < bucket_page_tuples; tuple_number++) {
                // auto pair = current_buckets_pages[bucket_number]->get_pair(tuple_number);
                auto tuple_key = reading_old_bucket_page->get_key(tuple_number);
                auto tuple_value = reading_old_bucket_page->get_value(tuple_number);
                uint64_t hash_ = hash_function_wrapper(tuple_key, key_size);
                uint64_t mask = number_of_buckets - 1;  // last (depth) bits
                uint64_t insert_bucket_number = hash_ & mask;  // suffix = bucket_number
                assert(insert_bucket_number == bucket_number || insert_bucket_number == previous_number_of_buckets + bucket_number);

                if (insert_bucket_number == bucket_number) { // insert to writing_old_bucket_page
                    if (writing_old_tuple_count >= max_tuples) {
                        // save values of writing page
                        writing_old_bucket_page->set_tuple_count(writing_old_tuple_count);
                        writing_old_bucket_page->page.make_dirty();
                        // set next writing page
                        writing_old_bucket_page = make_unique<KeyValueHashBucket<K, V>>(
                                                      buckets_file,
                                                      buckets_page_numbers[bucket_number][++writing_old_pos],
                                                      key_size,
                                                      value_size,
                                                      max_tuples);
                        writing_old_tuple_count = 0;
                        assert(writing_old_pos <= reading_page_number);
                    }
                    writing_old_bucket_page->insert_in_pos(tuple_key, tuple_value, writing_old_tuple_count);
                    writing_old_tuple_count++;
                }
                else { // insert to new_bucket_page
                    buckets_sizes[insert_bucket_number]++;
                    buckets_sizes[bucket_number]--;
                    if (new_bucket_page->get_tuple_count() >= max_tuples) {
                        new_page_number = get_new_page_number();
                        buckets_page_numbers[insert_bucket_number].push_back(new_page_number);

                        new_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, new_page_number, key_size, value_size, max_tuples
                        );
                        *(new_bucket_page->tuple_count) = 0;
                        new_bucket_page->page.make_dirty();
                    }
                    new_bucket_page->insert_with_pointers(tuple_key, tuple_value);
                }
            }
        }
        // save current aux
        writing_old_bucket_page->set_tuple_count(writing_old_tuple_count);
        writing_old_bucket_page->page.make_dirty();
        // set available pages
        writing_old_pos++;
        auto n = 0;
        while (writing_old_pos < buckets_page_numbers[bucket_number].size()) {
            auto real_page_number = buckets_page_numbers[bucket_number][writing_old_pos++];
            available_pages.push(real_page_number);
             n += 1;
        }
        buckets_page_numbers[bucket_number].resize(buckets_page_numbers[bucket_number].size()-n);
    }
}


template <class K, class V>
uint_fast32_t KeyValueHash<K, V>::get_bucket(const std::vector<K>& key) const {
    assert(key.size() == key_size);

    uint64_t hash_ = hash_function_wrapper(key.data(), key_size);
    uint64_t mask = (1 << depth) - 1;
    uint_fast32_t bucket_number = hash_ & mask;  // suffix = bucket_number in this case
    return bucket_number;
}


template <class K, class V>
void KeyValueHash<K, V>::sort_buckets(){
    const uint_fast32_t number_of_buckets = 1 << depth; // 2^depth
    for (uint_fast32_t bucket_number=0; bucket_number<number_of_buckets; bucket_number++) {
        sort_bucket(bucket_number);
    }
}


template <class K, class V>
void KeyValueHash<K, V>::check_order() {
    const uint_fast32_t number_of_buckets = 1 << depth; // 2^depth
    std::unique_ptr<KeyValueHashBucket<K, V>> current_bucket_page;
    K* last_key;
    K* current_key;
    for (uint_fast32_t bucket_number=0; bucket_number<number_of_buckets; bucket_number++) {
        uint32_t current_pos = 0;
        if (buckets_sizes[bucket_number] > 0) {
            last_key = get_key(bucket_number, current_pos);
        }
        current_pos++;
        while (current_pos < buckets_sizes[bucket_number]) {
            current_key = get_key(bucket_number, current_pos);
            bool smaller = false;
            for (uint_fast16_t i = 0; i < key_size; i++) {
                if (current_key[i] < last_key[i]) {
                    smaller = true;
                    break;
                }
                else if (current_key[i] > last_key[i]) {
                    break;
                }
            }
            if (smaller) {
                cout << "bucket: " << bucket_number << ", pos: " << current_pos << "not ordered\n";
            }
            last_key = current_key;
            current_pos++;
        }
    }
}


template <class K, class V>
bool KeyValueHash<K, V>::find_first(const std::vector<K>& current_key, uint_fast32_t bucket_number,
                                    uint_fast32_t* current_bucket_pos) {
    if (buckets_sizes[bucket_number] == 0) {
        return false;
    }
    // initial
    uint_fast32_t curr_max = buckets_sizes[bucket_number] - 1;
    uint_fast32_t curr_min = 0;
    uint_fast32_t curr_pos = (curr_min + curr_max)/2;

    // set key
    K* key_ptr = get_key(bucket_number, curr_pos);

    while (curr_min <= curr_max) {
        auto key_ptr_smaller = false;
        for (uint_fast32_t i=0; i<key_size; i++) {
            if (key_ptr[i] < current_key[i]) {
                key_ptr_smaller = true;
                break;
            }
            else if (key_ptr[i] > current_key[i]) {
                break;
            }
        }
        if (key_ptr_smaller) {
            curr_min = curr_pos + 1;
        }
        else { //equal or bigger
            curr_max = curr_pos;
        }
        // set new curr_pos
        curr_pos = (curr_min + curr_max)/2;
        key_ptr = get_key(bucket_number, curr_pos);
        if (curr_pos == curr_min && curr_pos == curr_max) {
            for (uint_fast32_t i=0; i<key_size; i++) {
                if (key_ptr[i] != current_key[i]) {
                    return false;
                }
            }
            *current_bucket_pos = curr_pos;
            return true;
        }
    }
    return false;
}


template <class K, class V>
void KeyValueHash<K, V>::sort_bucket(uint_fast32_t bucket_number){
    std::unique_ptr<KeyValueHashBucket<K, V>> current_bucket_page; // aux bucket
    std::unique_ptr<KeyValueHashBucket<K, V>> current_left_page;
    std::unique_ptr<KeyValueHashBucket<K, V>> current_right_page;
    K* left_key;
    K* right_key;
    V* left_value;
    V* right_value;
    // sort each page
    for (auto real_page_number: buckets_page_numbers[bucket_number]) {
        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(
            buckets_file, real_page_number, key_size, value_size, max_tuples
        );
        current_bucket_page->sort();
    }
    // merge
    uint_fast32_t merge_size = 1;
    while (buckets_page_numbers[bucket_number].size() > merge_size) {
        // set aux buckets
        vector<uint_fast32_t> new_page_numbers;
        new_page_numbers.reserve(buckets_page_numbers[bucket_number].size());
        uint_fast32_t new_page_number;
        if (available_pages.size() > 0) {
            new_page_number = available_pages.front();
            available_pages.pop();
        } else {
            new_page_number = ++last_page_number;
        }
        new_page_numbers.push_back(new_page_number);
        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(
            buckets_file, new_page_number,
            key_size, value_size, max_tuples
        );
        *(current_bucket_page->tuple_count) = 0;
        current_bucket_page->page.make_dirty();
        for (uint_fast32_t step = 0; step < buckets_page_numbers[bucket_number].size(); step+=(2* merge_size)) {
            uint_fast32_t left_page_number = step;
            uint_fast32_t right_page_number = step + merge_size;
            if (right_page_number >= buckets_page_numbers[bucket_number].size()) {
                while (left_page_number < buckets_page_numbers[bucket_number].size()) {
                    new_page_numbers.push_back(buckets_page_numbers[bucket_number][left_page_number]);
                    left_page_number++;
                }
                break;
            }
            current_left_page = std::make_unique<KeyValueHashBucket<K, V>>(
                buckets_file, buckets_page_numbers[bucket_number][left_page_number],
                key_size, value_size, max_tuples
            );
            current_right_page = std::make_unique<KeyValueHashBucket<K, V>>(
                buckets_file, buckets_page_numbers[bucket_number][right_page_number],
                key_size, value_size, max_tuples
            );
            uint_fast32_t left_pos = 0;
            uint_fast32_t right_pos = 0;
            left_key = current_left_page->get_key(left_pos);
            left_value = current_left_page->get_value(left_pos);
            right_key = current_right_page->get_key(right_pos);
            right_value = current_right_page->get_value(right_pos);
            while (left_page_number < step + merge_size &&
                    right_page_number < step + (2*merge_size) &&
                    right_page_number < buckets_page_numbers[bucket_number].size()) {
                bool smaller = false;
                for (uint_fast16_t i = 0; i < key_size; i++) {
                    if (left_key[i] < right_key[i]) {
                        smaller = true;
                        break;
                    }
                    else if (left_key[i] > right_key[i]) {
                        break;
                    }
                }
                // check if aux is full before insert
                if (current_bucket_page->get_tuple_count() >= max_tuples) {
                    if (available_pages.size() > 0) {
                        new_page_number = available_pages.front();
                        available_pages.pop();
                    } else {
                        new_page_number = ++last_page_number;
                    }
                    new_page_numbers.push_back(new_page_number);
                    current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(
                        buckets_file, new_page_number,
                        key_size, value_size, max_tuples
                    );
                    *(current_bucket_page->tuple_count) = 0;
                    current_bucket_page->page.make_dirty();
                }
                // add to aux
                if (smaller) {
                    current_bucket_page->insert_with_pointers(left_key, left_value);
                    left_pos += 1;
                    if (left_pos >= current_left_page->get_tuple_count()) {
                        available_pages.push(buckets_page_numbers[bucket_number][left_page_number]);
                        left_page_number += 1;
                        if (left_page_number >= step + merge_size) {
                            break;
                        }
                        current_left_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, buckets_page_numbers[bucket_number][left_page_number],
                            key_size, value_size, max_tuples
                        );
                        left_pos = 0;
                    }
                    left_key = current_left_page->get_key(left_pos);
                    left_value = current_left_page->get_value(left_pos);
                }
                else {
                    current_bucket_page->insert_with_pointers(right_key, right_value);
                    right_pos += 1;
                    if (right_pos >= current_right_page->get_tuple_count()) {
                        available_pages.push(buckets_page_numbers[bucket_number][right_page_number]);
                        right_page_number += 1;
                        if (right_page_number >= step + (2*merge_size) ||
                            right_page_number >= buckets_page_numbers[bucket_number].size()) {
                            break;
                        }
                        current_right_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, buckets_page_numbers[bucket_number][right_page_number],
                            key_size, value_size, max_tuples
                        );
                        right_pos = 0;
                    }
                    right_key = current_right_page->get_key(right_pos);
                    right_value = current_right_page->get_value(right_pos);
                }
            }
            // insert the rest
            if (left_page_number < step + merge_size) {  // left have more tuples to insert
                while (left_page_number < step + merge_size) {
                    // check if aux is full before insert
                    if (current_bucket_page->get_tuple_count() >= max_tuples) {
                        if (available_pages.size() > 0) {
                            new_page_number = available_pages.front();
                            available_pages.pop();
                        } else {
                            new_page_number = ++last_page_number;
                        }
                        new_page_numbers.push_back(new_page_number);
                        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, new_page_number,
                            key_size, value_size, max_tuples
                        );
                        *(current_bucket_page->tuple_count) = 0;
                        current_bucket_page->page.make_dirty();
                    }
                    current_bucket_page->insert_with_pointers(left_key, left_value);
                    left_pos += 1;
                    if (left_pos >= current_left_page->get_tuple_count()) {
                        available_pages.push(buckets_page_numbers[bucket_number][left_page_number]);
                        left_page_number += 1;
                        if (left_page_number >= step + merge_size) {
                            break;
                        }
                        current_left_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, buckets_page_numbers[bucket_number][left_page_number],
                            key_size, value_size, max_tuples
                        );
                        left_pos = 0;
                    }
                    left_key = current_left_page->get_key(left_pos);
                    left_value = current_left_page->get_value(left_pos);
                }
            }
            else {               // right have more tuples to insert
                while (right_page_number < step + (2*merge_size) &&
                        right_page_number < buckets_page_numbers[bucket_number].size()) {
                    // check if aux is full before insert
                    if (current_bucket_page->get_tuple_count() >= max_tuples) {
                        if (available_pages.size() > 0) {
                            new_page_number = available_pages.front();
                            available_pages.pop();
                        } else {
                            new_page_number = ++last_page_number;
                        }
                        new_page_numbers.push_back(new_page_number);
                        current_bucket_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, new_page_number,
                            key_size, value_size, max_tuples
                        );
                        *(current_bucket_page->tuple_count) = 0;
                        current_bucket_page->page.make_dirty();
                    }
                    current_bucket_page->insert_with_pointers(right_key, right_value);
                    right_pos += 1;
                    if (right_pos >= current_right_page->get_tuple_count()) {
                        available_pages.push(buckets_page_numbers[bucket_number][right_page_number]);
                        right_page_number += 1;
                        if (right_page_number >= step + (2*merge_size) ||
                            right_page_number >= buckets_page_numbers[bucket_number].size()) {
                            break;
                        }
                        current_right_page = std::make_unique<KeyValueHashBucket<K, V>>(
                            buckets_file, buckets_page_numbers[bucket_number][right_page_number],
                            key_size, value_size, max_tuples
                        );
                        right_pos = 0;
                    }
                    right_key = current_right_page->get_key(right_pos);
                    right_value = current_right_page->get_value(right_pos);
                }
            }
        }
        buckets_page_numbers[bucket_number] = move(new_page_numbers);
        merge_size *= 2;
    }
}
