#include "bplus_tree_dir.h"

#include <cassert>
#include <iostream>
#include <utility>
#include <cstring>

#include "storage/buffer_manager.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/record.h"

template class BPlusTreeDir<1>;
template class BPlusTreeDir<2>;
template class BPlusTreeDir<3>;
template class BPlusTreeDir<4>;

using namespace std;

template <std::size_t N>
BPlusTreeDir<N>::BPlusTreeDir(FileId const leaf_file_id, Page& page) :
    dir_file_id  (page.page_id.file_id),
    leaf_file_id (leaf_file_id),
    page         (page),
    keys         (reinterpret_cast<uint64_t*>(page.get_bytes())),
    key_count    (reinterpret_cast<uint32_t*>(page.get_bytes()
                      + (sizeof(uint64_t) * BPlusTree<N>::dir_max_records * N))),
    children     (reinterpret_cast<int32_t*>(page.get_bytes()
                      + (sizeof(uint64_t) * BPlusTree<N>::dir_max_records * N)
                      + sizeof(uint32_t)))
    { }


template <std::size_t N>
BPlusTreeDir<N>::~BPlusTreeDir() {
    buffer_manager.unpin(page);
}


// requieres first insert manually
template <std::size_t N>
std::unique_ptr<BPlusTreeSplit<N>> BPlusTreeDir<N>::bulk_insert(BPlusTreeLeaf<N>& leaf) {
    int page_pointer = children[*key_count];
    std::unique_ptr<BPlusTreeSplit<N>> split;

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page(dir_file_id, page_pointer*-1);
        auto child =  BPlusTreeDir<N>(leaf_file_id, child_page);
        split = child.bulk_insert(leaf);
    }
    else { // positive number: pointer to leaf
        split = make_unique<BPlusTreeSplit<N>>(*leaf.get_record(0), leaf.page.get_page_number());
    }

    if (split != nullptr) {
        // Case 1: no need to split this node
        if (*key_count < BPlusTree<N>::dir_max_records) {
            update_key(*key_count, split->record);
            ++(*key_count);
            update_child(*key_count, split->encoded_page_number);
            page.make_dirty();
            return nullptr;
        }
        // Case 2: we need to split this node and this node is the root
        else if (page.get_page_number() == 0) {
            auto& new_left_page = buffer_manager.append_page(dir_file_id);
            auto& new_right_page = buffer_manager.append_page(dir_file_id);

            auto new_left_dir = BPlusTreeDir<N>(leaf_file_id, new_left_page);
            auto new_right_dir = BPlusTreeDir<N>(leaf_file_id, new_right_page);

            // write left keys from 0 to (key_count-1)
            std::memcpy(
                new_left_dir.keys,
                keys,
                (*key_count) * N * sizeof(uint64_t)
            );
            // write left children-pointers from 0 to key_count
            std::memcpy(
                new_left_dir.children,
                children,
                ((*key_count) + 1) * sizeof(int)
            );

            // write right dirs
            new_right_dir.children[0] = split->encoded_page_number;

            // update counts
            *new_left_dir.key_count = *this->key_count;
            *this->key_count = 1;
            *new_right_dir.key_count = 0;

            std::memcpy(
                keys,
                split->record.ids.data(),
                N * sizeof(uint64_t)
            );
            children[0] = new_left_dir.page.get_page_number() * -1;
            children[1] = new_right_dir.page.get_page_number() * -1;
            new_left_page.make_dirty();
            new_right_page.make_dirty();
            this->page.make_dirty();

            return nullptr;
        }
        // Case 3: no-root split
        else {
            auto& new_page = buffer_manager.append_page(dir_file_id);
            auto new_dir = BPlusTreeDir<N>(leaf_file_id, new_page);
            new_dir.children[0] = split->encoded_page_number;
            *new_dir.key_count = 0;
            // this->key_count does not change
            new_page.make_dirty();
            this->page.make_dirty();
            return std::make_unique<BPlusTreeSplit<N>>(split->record, new_page.get_page_number()*-1);
        }
    }
    return nullptr;
}


template <std::size_t N>
std::unique_ptr<BPlusTreeSplit<N>> BPlusTreeDir<N>::insert(const Record<N>& record) {
    int index = (*key_count > 0)
        ? search_child_index(0, *key_count, record)
        : 0;

    int page_pointer = children[index];
    std::unique_ptr<BPlusTreeSplit<N>> split = nullptr;

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page(dir_file_id, page_pointer*-1);
        auto child =  BPlusTreeDir<N>(leaf_file_id, child_page);
        split = child.insert(record);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page(leaf_file_id, page_pointer);
        auto child =  BPlusTreeLeaf<N>(child_page);
        split = child.insert(record);
    }

    if (split != nullptr) {
        uint_fast32_t splitted_index = search_child_index(0, *key_count, split->record);
        // Case 1: no need to split this node
        if (*key_count < BPlusTree<N>::dir_max_records) {
            shift_right_keys(splitted_index, (*key_count)-1);
            shift_right_children(splitted_index+1, *key_count);
            update_key(splitted_index, split->record);
            update_child(splitted_index+1, split->encoded_page_number);
            ++(*key_count);
            this->page.make_dirty();
            return nullptr;
        }
        // Case 2: we need to split this node and this node is the root
        else if (page.get_page_number() == 0) {
            // poner nuevo record/dir y guardar el ultimo (que no cabe)
            std::array<uint64_t, N> last_key;
            int last_dir;
            if (splitted_index == *key_count) { // splitted key is the last key
                std::memcpy(
                    last_key.data(),
                    split->record.ids.data(),
                    N * sizeof(uint64_t)
                );
                last_dir = split->encoded_page_number;
            }
            else {
                std::memcpy(
                    last_key.data(),
                    &keys[((*key_count)-1) * N],
                    N * sizeof(uint64_t)
                );
                last_dir = children[*key_count];
                shift_right_keys(splitted_index, (*key_count)-2);
                shift_right_children(splitted_index+1, (*key_count)-1);
                update_key(splitted_index, split->record);
                update_child(splitted_index+1, split->encoded_page_number);
            }
            int middle_index = ((*key_count)+1)/2;
            auto& new_left_page = buffer_manager.append_page(dir_file_id);
            auto& new_right_page = buffer_manager.append_page(dir_file_id);

            auto new_left_dir = BPlusTreeDir<N>(leaf_file_id, new_left_page);
            auto new_right_dir = BPlusTreeDir<N>(leaf_file_id, new_right_page);

            // write left keys from 0 to (middle_index-1)
            std::memcpy(
                new_left_dir.keys,
                keys,
                middle_index * N * sizeof(uint64_t)
            );
            // write right keys from (middle_index+1) to (*count-1) plus the last key saved before
            std::memcpy(
                new_right_dir.keys,
                &keys[(middle_index + 1) * N],
                (BPlusTree<N>::dir_max_records-(middle_index + 1)) * N * sizeof(uint64_t)
            );

            std::memcpy(
                &new_right_dir.keys[(BPlusTree<N>::dir_max_records - (middle_index + 1)) * N],
                last_key.data(),
                N * sizeof(uint64_t)
            );

            // write left children from 0 to middle_index
            std::memcpy(
                new_left_dir.children,
                children,
                (middle_index+1) * sizeof(int)
            );

            // write right dirs from middle_index + 1 to *count plus the last dir saved before
            std::memcpy(
                new_right_dir.children,
                &children[middle_index + 1],
                ((*key_count) - middle_index) * sizeof(int)
            );
            new_right_dir.children[(*key_count) - middle_index] = last_dir;
            // update counts
            (*key_count) = 1;
            *new_left_dir.key_count = middle_index;
            *new_right_dir.key_count = BPlusTree<N>::dir_max_records - middle_index;

            // record at middle_index becomes the first and only record of the root
            std::memcpy(
                keys,
                &keys[middle_index*N],
                N * sizeof(uint64_t)
            );
            children[0] = new_left_dir.page.get_page_number() * -1;
            children[1] = new_right_dir.page.get_page_number() * -1;
            new_left_page.make_dirty();
            new_right_page.make_dirty();
            this->page.make_dirty();
            return nullptr;
        }
        // Case 3: normal split (this node is not the root)
        else {
            // poner nuevo record/dir y guardar el ultimo (que no cabe)
            std::array<uint64_t, N> last_key;
            int last_dir;
            if (splitted_index == *key_count) { // splitted key is the last key
                std::memcpy(
                    last_key.data(),
                    split->record.ids.data(),
                    N * sizeof(uint64_t)
                );
                last_dir = split->encoded_page_number;
            }
            else {
                std::memcpy(
                    last_key.data(),
                    &keys[((*key_count)-1) * N],
                    N * sizeof(uint64_t)
                );
                last_dir = children[*key_count];
                shift_right_keys(splitted_index, (*key_count)-2);
                shift_right_children(splitted_index+1, (*key_count)-1);
                update_key(splitted_index, split->record);
                update_child(splitted_index+1, split->encoded_page_number);
            }
            int middle_index = ((*key_count)+1)/2;

            auto& new_page = buffer_manager.append_page(dir_file_id);
            auto new_dir = BPlusTreeDir<N>(leaf_file_id, new_page);

            // write records from (middle_index+1) to ((*key_count)-1) and the last record saved before
            std::memcpy(
                new_dir.keys,
                &keys[(middle_index+1)*N],
                (BPlusTree<N>::dir_max_records - (middle_index+1))*N * sizeof(uint64_t)
            );
            std::memcpy(
                &new_dir.keys[(BPlusTree<N>::dir_max_records - (middle_index+1))*N],
                last_key.data(),
                N * sizeof(uint64_t)
            );
            // write children from middle_index + 1 to key_count and the last dir saved before
            std::memcpy(
                new_dir.children,
                &children[middle_index + 1],
                ((*key_count) - middle_index) * sizeof(int)
            );
            new_dir.children[(*key_count) - middle_index] = last_dir;
            // update counts
            *key_count = middle_index;
            *new_dir.key_count = BPlusTree<N>::dir_max_records - middle_index;

            // key at middle_index is returned
            std::array<uint64_t, N> split_key;
            std::memcpy(
                split_key.data(),
                &keys[middle_index*N],
                N * sizeof(uint64_t)
            );
            new_page.make_dirty();
            this->page.make_dirty();
            return std::make_unique<BPlusTreeSplit<N>>(move(split_key), new_page.get_page_number()*-1);
        }
    }
    return nullptr;
}


template <std::size_t N>
void BPlusTreeDir<N>::update_key(int index, const Record<N>& record) {
    std::memcpy(
        &keys[index*N],
        record.ids.data(),
        N * sizeof(uint64_t)
    );
}


template <std::size_t N>
void BPlusTreeDir<N>::update_child(int index, int dir) {
    children[index] = dir;
}


template <std::size_t N>
void BPlusTreeDir<N>::shift_right_keys(int from, int to) {
    for (int i = to; i >= from; i--) {
        for (uint_fast32_t j = 0; j < N; j++) {
            keys[(i+1)*N + j] = keys[i*N + j];
        }
    }
}


template <std::size_t N>
void BPlusTreeDir<N>::shift_right_children(int from, int to) {
    for (int i = to; i >= from; i--) {
        children[i+1] = children[i];
    }
}


template <std::size_t N>
SearchLeafResult<N> BPlusTreeDir<N>::search_leaf(const Record<N>& min) const noexcept {
    int dir_index = search_child_index(0, *key_count, min);
    int page_pointer = children[dir_index];

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page(dir_file_id, page_pointer*-1);
        auto child = BPlusTreeDir<N>(leaf_file_id, child_page);
        return child.search_leaf(min);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page(leaf_file_id, page_pointer);
        auto child = make_unique<BPlusTreeLeaf<N>>(child_page);
        auto index = child->search_index(min);
        return SearchLeafResult(move(child), index);
    }
}


template <std::size_t N>
SearchLeafResult<N> BPlusTreeDir<N>::search_leaf(stack< unique_ptr<BPlusTreeDir<N>> >& stack,
                                                 const Record<N>& min) const noexcept
{
    int dir_index = search_child_index(0, *key_count, min);
    int page_pointer = children[dir_index];

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page(dir_file_id, page_pointer*-1);
        auto child = make_unique<BPlusTreeDir<N>>(leaf_file_id, child_page);
        stack.push( std::move(child) );
        return stack.top()->search_leaf(stack, min);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page(leaf_file_id, page_pointer);
        auto child = make_unique<BPlusTreeLeaf<N>>(child_page);
        auto index = child->search_index(min);
        return SearchLeafResult(move(child), index);
    }
}


template <std::size_t N>
int BPlusTreeDir<N>::search_child_index(int dir_from, int dir_to, const Record<N>& record) const {
search_child_index_begin:
    if (dir_from == dir_to) {
        return dir_from;
    }
    int middle_dir = (dir_from + dir_to + 1) / 2;
    int middle_record = middle_dir-1;

    for (uint_fast32_t i = 0; i < N; i++) {
        auto id = keys[middle_record*N + i];
        if (record.ids[i] < id) {
            dir_to = middle_record;
            goto search_child_index_begin;
        } else if (record.ids[i] > id) {
            dir_from = middle_record+1;
            goto search_child_index_begin;
        }
        // continue if they were equal
    }
    dir_from = middle_record+1;
    goto search_child_index_begin;
}


template <std::size_t N>
bool BPlusTreeDir<N>::check_range(const Record<N>& r) const {
    if (*key_count == 0) {
        return false;
    }
    std::array<uint64_t, N> min;
    std::array<uint64_t, N> max;
    for (uint_fast32_t i = 0; i < N; i++) {
        min[i] = keys[0*N + i];
        max[i] = keys[(*key_count-1)*N + i];
    }

    return min <= r.ids && r.ids <= max;
}


template <std::size_t N>
bool BPlusTreeDir<N>::check() const {
    if (*key_count > BPlusTree<N>::dir_max_records) {
        std::cerr << "  ERROR: key_count shouldn't be less than 0\n";
        return false;
    }
    if (*key_count == 0 && page.get_page_number() != 0) {
        std::cout << "  WARNING: key_count shouldn't be 0, except for one node (at most)"
                  << " at the right-most branch if bulk import was used\n";
    }

    if (*key_count > 1) {
        // check keys are ordered
        auto x = Record<N>(std::array<uint64_t, N>());
        auto y = Record<N>(std::array<uint64_t, N>());

        uint_fast32_t current_pos = 0;
        while (current_pos < N) {
            x.ids[current_pos] = keys[current_pos];
            current_pos++;
        }

        for (uint_fast32_t k = 1; k < *key_count; k++) {
            for (uint_fast32_t i = 0; i < N; i++) {
                y.ids[i] = keys[current_pos++];
            }
            if (y <= x) {
                std::cerr << "  bad key order at BPlusTreeDir:\n";
                std::cerr << "    " << x << "\n";
                std::cerr << "    " << y << "\n";
                return false;
            }
            x = y;
        }
    }

    // check children and keys are consistent
    int current_pos = 0;
    for (uint_fast32_t i = 0; i < *key_count; i++) {
        auto key = Record<N>(std::array<uint64_t, N>());
        auto greatest_left_key = Record<N>(std::array<uint64_t, N>());
        auto smallest_right_key = Record<N>(std::array<uint64_t, N>());

        for (uint_fast32_t j = 0; j < N; j++) {
            key.ids[j] = keys[current_pos++];
        }

        // Set greatest_left_key
        int left_pointer = children[i];
        if (left_pointer < 0) { // negative number: pointer to dir
            auto& left_page = buffer_manager.get_page(dir_file_id, left_pointer*-1);
            auto left_child =  BPlusTreeDir(leaf_file_id, left_page);
            for (uint_fast32_t j = 0; j < N; j++) {
                greatest_left_key.ids[j] = left_child.keys[((*left_child.key_count-1) * N) + j];
            }
        }
        else { // positive number: pointer to leaf
            auto& left_page = buffer_manager.get_page(leaf_file_id, left_pointer);
            auto left_child =  BPlusTreeLeaf<N>(left_page);
            for (uint_fast32_t j = 0; j < N; j++) {
                greatest_left_key.ids[j] = left_child.records[((*left_child.value_count-1) * N) + j];
            }
        }

        // Set smallest_right_key
        int right_pointer = children[i+1];
        bool right_empty = false; // for skipping empty dirs
        if (right_pointer < 0) { // negative number: pointer to dir
            Page& right_page = buffer_manager.get_page(dir_file_id, right_pointer*-1);
            auto right_child =  BPlusTreeDir(leaf_file_id, right_page);
            for (uint_fast32_t j = 0; j < N; j++) {
                smallest_right_key.ids[j] = right_child.keys[j];
            }
            if (*right_child.key_count == 0) {
                right_empty = true;
            }
        }
        else { // positive number: pointer to leaf
            Page& right_page = buffer_manager.get_page(leaf_file_id, right_pointer);
            auto right_child =  BPlusTreeLeaf<N>(right_page);
            for (uint_fast32_t j = 0; j < N; j++) {
                smallest_right_key.ids[j] = right_child.records[j];
            }
            if (*right_child.value_count == 0) {
                right_empty = true;
            }
        }

        if (!(greatest_left_key < key)) {
            std::cerr << "  ERROR: inconsistency between key and left-child key at BPlusTreeDir.\n";
            std::cerr << "    " << greatest_left_key << "\n";
            std::cerr << "    " << key << "\n";
            return false;
        }
        if (!right_empty && !(key <= smallest_right_key)) {
            std::cerr << "  ERROR: inconsistency between key and right-child key at BPlusTreeDir\n";
            std::cerr << key << "\n";
            std::cerr << smallest_right_key << "\n";
            return false;
        }
    }

    // propagate checking to children
    for (uint_fast32_t i = 0; i <= *key_count; i++) {
        int page_pointer = children[i];

        if (page_pointer < 0) { // negative number: pointer to dir
            auto& child_page = buffer_manager.get_page(dir_file_id, page_pointer*-1);
            auto child =  BPlusTreeDir<N>(leaf_file_id, child_page);
            if (!child.check())
                return false;
        }
        else { // positive number: pointer to leaf
            auto& child_page = buffer_manager.get_page(leaf_file_id, page_pointer);
            auto child =  BPlusTreeLeaf<N>(child_page);
            if (!child.check())
                return false;
        }
    }
    return true;
}
