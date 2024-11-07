#include "bplus_tree_dir.h"

#include <cassert>
#include <cstring>
#include <utility>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/record.h"
#include "system/buffer_manager.h"


template <std::size_t N>
BPlusTreeDir<N>::~BPlusTreeDir<N>() {
    buffer_manager.unpin(*page);
}


template <std::size_t N>
void BPlusTreeDir<N>::upgrade_to_editable() {
    if (buffer_manager.need_edit_version(*page)) {

        auto new_page = &buffer_manager.get_page_editable(dir_file_id, page->get_page_number());
        buffer_manager.unpin(*page);
        page = new_page;

        keys = reinterpret_cast<uint64_t*>(page->get_bytes());

        key_count = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(page->get_bytes()
                        + (sizeof(uint64_t) * BPlusTree<N>::dir_max_records * N)));

        children = reinterpret_cast<int32_t*>(page->get_bytes()
                        + (sizeof(uint64_t) * BPlusTree<N>::dir_max_records * N)
                        + sizeof(uint32_t));
    }
}


template <std::size_t N>
bool BPlusTreeDir<N>::delete_record(const Record<N>& record) {
    auto index = (*key_count > 0) ? search_child_index(record)
                                  : 0;

    auto page_pointer = children[index];

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page_readonly(dir_file_id, page_pointer*-1);
        BPlusTreeDir<N> child(leaf_file_id, &child_page);
        return child.delete_record(record);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page_readonly(leaf_file_id, page_pointer);
        BPlusTreeLeaf<N> child(&child_page);
        return child.delete_record(record);
    }
}


template <std::size_t N>
std::unique_ptr<BPlusTreeSplit<N>> BPlusTreeDir<N>::insert(const Record<N>& record, bool& error) {
    auto index = (*key_count > 0) ? search_child_index(record)
                                  : 0;

    auto page_pointer = children[index];
    std::unique_ptr<BPlusTreeSplit<N>> split = nullptr;

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page_readonly(dir_file_id, page_pointer*-1);
        BPlusTreeDir<N> child(leaf_file_id, &child_page);
        split = child.insert(record, error);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page_readonly(leaf_file_id, page_pointer);
        BPlusTreeLeaf<N> child(&child_page);
        split = child.insert(record, error);
    }

    if (split != nullptr) {
        uint_fast32_t splitted_index = search_child_index(split->record);
        uint_fast32_t splitted_index2 = search_child_index(split->record2);

        upgrade_to_editable();

        // Case 1: no need to split this node
        if (!split->double_split && *key_count < BPlusTree<N>::dir_max_records) {
            if (*key_count > 0) {
                shift_right_keys(splitted_index, (*key_count)-1);
            }
            shift_right_children(splitted_index+1, *key_count);
            update_key(splitted_index, split->record);
            update_child(splitted_index+1, split->encoded_page_number);
            ++(*key_count);

            return nullptr;
        }
        // Case 2: no need to split this node, 2 keys are inserted
        else if (split->double_split && *key_count + 1 < BPlusTree<N>::dir_max_records) {
            if (*key_count > 0) {
                shift_right_keys(splitted_index2, *key_count);
            }
            shift_right_children(splitted_index2 + 1, *key_count);
            update_key(splitted_index2, split->record2);
            update_child(splitted_index2 + 1, split->encoded_page_number2);
            ++*key_count;

            shift_right_keys(splitted_index, *key_count);
            shift_right_children(splitted_index + 1, *key_count + 1);
            update_key(splitted_index, split->record);
            update_child(splitted_index + 1, split->encoded_page_number);
            ++*key_count;

            return nullptr;
        }
        // Case 3: we need to split this node and this node is the root
        else if (page->get_page_number() == 0) {
            // put new record/dir and save the last (that does not fit)
            std::vector<std::array<uint64_t, N>> last_keys(2);
            std::vector<int_fast32_t> last_dirs(2);

            // splitted key is the last key
            if (splitted_index == *key_count) {
                last_keys[0] = record;
                last_dirs[0] = split->encoded_page_number;
            }
            else {
                std::memcpy(
                    last_keys[0].data(),
                    &keys[((*key_count)-1) * N],
                    N * sizeof(uint64_t)
                );

                last_dirs[0] = children[*key_count];
                shift_right_keys(splitted_index, (*key_count)-2);
                shift_right_children(splitted_index+1, (*key_count)-1);
                update_key(splitted_index, split->record);
                update_child(splitted_index+1, split->encoded_page_number);
            }

            if (split->double_split && splitted_index2 == *key_count) {
                last_keys[1] = split->record2;
                last_dirs[1] = split->encoded_page_number2;
            }
            else if (split->double_split) {
                last_keys[1] = last_keys[0];

                std::memcpy(
                    last_keys[0].data(),
                    &keys[((*key_count)-1) * N],
                    N * sizeof(uint64_t)
                );

                last_dirs[1] = last_dirs[0];

                last_dirs[0] = children[*key_count];
                shift_right_keys(splitted_index2, (*key_count)-2);
                shift_right_children(splitted_index2+1, (*key_count)-1);
                update_key(splitted_index2, split->record2);
                update_child(splitted_index2+1, split->encoded_page_number2);
            }

            int_fast32_t middle_index = ((*key_count)+1)/2;
            auto& new_lhs_page = buffer_manager.append_vpage(dir_file_id);
            auto& new_rhs_page = buffer_manager.append_vpage(dir_file_id);

            BPlusTreeDir<N> new_lhs_dir(leaf_file_id, &new_lhs_page);
            BPlusTreeDir<N> new_rhs_dir(leaf_file_id, &new_rhs_page);

            // write left keys from 0 to (middle_index-1)
            std::memcpy(
                new_lhs_dir.keys,
                keys,
                middle_index * N * sizeof(uint64_t)
            );
            // write right keys from (middle_index+1) to (*count-1) plus the last key saved before
            std::memcpy(
                new_rhs_dir.keys,
                &keys[(middle_index + 1) * N],
                (BPlusTree<N>::dir_max_records-(middle_index + 1)) * N * sizeof(uint64_t)
            );

            std::memcpy(
                &new_rhs_dir.keys[(BPlusTree<N>::dir_max_records - (middle_index + 1)) * N],
                last_keys[0].data(),
                N * sizeof(uint64_t)
            );

            // write left children from 0 to middle_index
            std::memcpy(
                new_lhs_dir.children,
                children,
                (middle_index+1) * sizeof(int32_t)
            );

            // write right dirs from middle_index + 1 to *count plus the last dir saved before
            std::memcpy(
                new_rhs_dir.children,
                &children[middle_index + 1],
                ((*key_count) - middle_index) * sizeof(int32_t)
            );

            new_rhs_dir.children[(*key_count) - middle_index] = last_dirs[0];

            // update counts
            (*key_count) = 1;
            *new_lhs_dir.key_count = middle_index;
            *new_rhs_dir.key_count = BPlusTree<N>::dir_max_records - middle_index;

            // write the last record if there was a double split before
            if (split->double_split) {
                std::memcpy(
                    &new_rhs_dir.keys[(BPlusTree<N>::dir_max_records - middle_index) * N],
                    last_keys[1].data(),
                    N * sizeof(uint64_t)
                );

                new_rhs_dir.children[(*key_count) - middle_index + 1] = last_dirs[1];

                ++*new_rhs_dir.key_count;
            }

            // record at middle_index becomes the first and only record of the root
            std::memcpy(
                keys,
                &keys[middle_index*N],
                N * sizeof(uint64_t)
            );
            children[0] = static_cast<int32_t>(new_lhs_dir.page->get_page_number()) * -1;
            children[1] = static_cast<int32_t>(new_rhs_dir.page->get_page_number()) * -1;

            return nullptr;
        }
        // Case 4: normal split (this node is not the root)
        else {
            // put new record/dir and save the last (that does not fit)
            std::vector<std::array<uint64_t, N>> last_keys(2);
            std::vector<int_fast32_t> last_dirs(2);

            // splitted key is the last key
            if (splitted_index == *key_count) {
                last_keys[0] = record;
                last_dirs[0] = split->encoded_page_number;
            }
            else {
                std::memcpy(
                    last_keys[0].data(),
                    &keys[((*key_count)-1) * N],
                    N * sizeof(uint64_t)
                );
                last_dirs[0] = children[*key_count];
                shift_right_keys(splitted_index, (*key_count)-2);
                shift_right_children(splitted_index+1, (*key_count)-1);
                update_key(splitted_index, split->record);
                update_child(splitted_index+1, split->encoded_page_number);
            }

            if (split->double_split && splitted_index2 == *key_count) {
                last_keys[1] = split->record2;
                last_dirs[1] = split->encoded_page_number2;
            }
            else if (split->double_split) {
                last_keys[1] = last_keys[0];

                std::memcpy(
                    last_keys[0].data(),
                    &keys[((*key_count)-1) * N],
                    N * sizeof(uint64_t)
                );

                last_dirs[1] = last_dirs[0];

                last_dirs[0] = children[*key_count];
                shift_right_keys(splitted_index2, (*key_count)-2);
                shift_right_children(splitted_index2+1, (*key_count)-1);
                update_key(splitted_index2, split->record2);
                update_child(splitted_index2+1, split->encoded_page_number2);
            }

            int_fast32_t middle_index = ((*key_count)+1)/2;

            auto& new_page = buffer_manager.append_vpage(dir_file_id);
            auto new_dir = BPlusTreeDir<N>(leaf_file_id, &new_page);

            // write records from (middle_index+1) to ((*key_count)-1) and the last record saved before
            std::memcpy(
                new_dir.keys,
                &keys[(middle_index+1)*N],
                (BPlusTree<N>::dir_max_records - (middle_index+1))*N * sizeof(uint64_t)
            );
            std::memcpy(
                &new_dir.keys[(BPlusTree<N>::dir_max_records - (middle_index+1))*N],
                last_keys[0].data(),
                N * sizeof(uint64_t)
            );
            // write children from middle_index + 1 to key_count and the last dir saved before
            std::memcpy(
                new_dir.children,
                &children[middle_index + 1],
                ((*key_count) - middle_index) * sizeof(int32_t)
            );
            new_dir.children[(*key_count) - middle_index] = last_dirs[0];

            // update counts
            *key_count = middle_index;
            *new_dir.key_count = BPlusTree<N>::dir_max_records - middle_index;

            // write the last record if there was a double split before
            if (split->double_split) {
                std::memcpy(
                    &new_dir.keys[(BPlusTree<N>::dir_max_records - middle_index) * N],
                    last_keys[1].data(),
                    N * sizeof(uint64_t)
                );

                new_dir.children[(*key_count) - middle_index + 1] = last_dirs[1];

                ++*new_dir.key_count;
            }

            // key at middle_index is returned
            std::array<uint64_t, N> split_key;
            std::memcpy(
                split_key.data(),
                &keys[middle_index*N],
                N * sizeof(uint64_t)
            );
            return std::make_unique<BPlusTreeSplit<N>>(
                std::move(split_key),
                new_page.get_page_number()*-1);
        }
    }
    return nullptr;
}


template <std::size_t N>
void BPlusTreeDir<N>::update_key(int_fast32_t index, const Record<N>& record) {
    std::memcpy(
        &keys[index*N],
        record.data(),
        N * sizeof(uint64_t)
    );
}


template <std::size_t N>
void BPlusTreeDir<N>::update_child(int_fast32_t index, int_fast32_t dir) {
    children[index] = dir;
}


template <std::size_t N>
void BPlusTreeDir<N>::shift_right_keys(int_fast32_t from, int_fast32_t to) {
    for (int_fast32_t i = to; i >= from; i--) {
        for (uint_fast32_t j = 0; j < N; j++) {
            keys[(i+1)*N + j] = keys[i*N + j];
        }
    }
}


template <std::size_t N>
void BPlusTreeDir<N>::shift_right_children(int_fast32_t from, int_fast32_t to) {
    for (int_fast32_t i = to; i >= from; i--) {
        children[i+1] = children[i];
    }
}


template <std::size_t N>
SearchLeafResult<N> BPlusTreeDir<N>::search_leaf(const Record<N>& min) const noexcept {
    auto dir_index = search_child_index(min);
    auto page_pointer = children[dir_index];

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page_readonly(dir_file_id, page_pointer*-1);
        auto child = BPlusTreeDir<N>(leaf_file_id, &child_page);
        return child.search_leaf(min);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page_readonly(leaf_file_id, page_pointer);
        BPlusTreeLeaf<N> child(&child_page);
        auto index = child.search_index(min);
        return SearchLeafResult(std::move(child), index);
    }
}


template <std::size_t N>
SearchLeafResult<N> BPlusTreeDir<N>::search_leaf(
    std::vector< std::unique_ptr<BPlusTreeDir<N>> >& stack,
    const Record<N>& min) const noexcept
{
    auto dir_index = search_child_index(min);
    auto page_pointer = children[dir_index];

    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page_readonly(dir_file_id, page_pointer*-1);
        auto child = std::make_unique<BPlusTreeDir<N>>(leaf_file_id, &child_page);
        stack.push_back( std::move(child) );
        return stack.back()->search_leaf(stack, min);
    }
    else { // positive number: pointer to leaf
        auto& child_page = buffer_manager.get_page_readonly(leaf_file_id, page_pointer);
        BPlusTreeLeaf<N> child(&child_page);
        auto index = child.search_index(min);
        return SearchLeafResult(std::move(child), index);
    }
}


template <std::size_t N>
size_t BPlusTreeDir<N>::search_child_index(const Record<N>& record) const noexcept {
    int_fast32_t dir_from = 0;
    int_fast32_t dir_to = *key_count;
search_child_index_begin:
    if (dir_from == dir_to) {
        return dir_from;
    }
    int_fast32_t middle_dir = (dir_from + dir_to + 1) / 2;
    int_fast32_t middle_record = middle_dir-1;

    for (uint_fast32_t i = 0; i < N; i++) {
        auto id = keys[middle_record*N + i];
        if (record[i] < id) {
            dir_to = middle_record;
            goto search_child_index_begin;
        } else if (record[i] > id) {
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

    return min <= r && r <= max;
}


template <std::size_t N>
bool BPlusTreeDir<N>::check(std::ostream& os) const {
    if (*key_count > BPlusTree<N>::dir_max_records) {
        os << "  ERROR: key_count shouldn't be less than 0\n";
        return false;
    }
    if (*key_count == 0 && page->get_page_number() != 0) {
        os << "  WARNING: key_count shouldn't be 0, except for one node (at most)"
           << " at the right-most branch if bulk import was used\n";
    }

    if (*key_count > 1) {
        // check keys are ordered
        auto x = Record<N>(std::array<uint64_t, N>());
        auto y = Record<N>(std::array<uint64_t, N>());

        uint_fast32_t current_pos = 0;
        while (current_pos < N) {
            x[current_pos] = keys[current_pos];
            current_pos++;
        }

        for (uint_fast32_t k = 1; k < *key_count; k++) {
            for (uint_fast32_t i = 0; i < N; i++) {
                y[i] = keys[current_pos++];
            }
            if (y <= x) {
                os << "  bad key order at BPlusTreeDir:\n";
                os << "    " << x << "\n";
                os << "    " << y << "\n";
                return false;
            }
            x = y;
        }
    }

    // check children and keys are consistent
    int_fast32_t current_pos = 0;
    for (uint_fast32_t i = 0; i < *key_count; i++) {
        Record<N> key;
        Record<N> greatest_left_key;
        Record<N> smallest_right_key;

        for (uint_fast32_t j = 0; j < N; j++) {
            key[j] = keys[current_pos++];
        }

        // Set greatest_left_key
        auto left_pointer = children[i];
        if (left_pointer < 0) { // negative number: pointer to dir
            auto& left_page = buffer_manager.get_page_readonly(dir_file_id, left_pointer*-1);
            BPlusTreeDir<N> left_child(leaf_file_id, &left_page);
            for (uint_fast32_t j = 0; j < N; j++) {
                greatest_left_key[j] = left_child.keys[((*left_child.key_count-1) * N) + j];
            }
        }
        else { // positive number: pointer to leaf
            auto& left_page = buffer_manager.get_page_readonly(leaf_file_id, left_pointer);
            BPlusTreeLeaf<N> left_child(&left_page);
            left_child.set_record(*left_child.value_count - 1, greatest_left_key);
        }

        // Set smallest_right_key
        auto right_pointer = children[i+1];
        bool right_empty = false; // for skipping empty dirs
        if (right_pointer < 0) { // negative number: pointer to dir
            auto& right_page = buffer_manager.get_page_readonly(dir_file_id, right_pointer*-1);
            BPlusTreeDir<N> right_child(leaf_file_id, &right_page);
            for (uint_fast32_t j = 0; j < N; j++) {
                smallest_right_key[j] = right_child.keys[j];
            }
            if (*right_child.key_count == 0) {
                right_empty = true;
            }
        }
        else { // positive number: pointer to leaf
            auto& right_page = buffer_manager.get_page_readonly(leaf_file_id, right_pointer);
            BPlusTreeLeaf<N> right_child(&right_page);
            right_child.set_record(0, smallest_right_key);
            if (*right_child.value_count == 0) {
                right_empty = true;
            }
        }

        if (!(greatest_left_key < key)) {
            os << "  ERROR: inconsistency between key and left-child key at BPlusTreeDir.\n";
            os << "    " << greatest_left_key << "\n";
            os << "    " << key << "\n";
            return false;
        }
        if (!right_empty && !(key <= smallest_right_key)) {
            os << "  ERROR: inconsistency between key and right-child key at BPlusTreeDir\n";
            os << key << "\n";
            os << smallest_right_key << "\n";
            return false;
        }
    }

    // propagate checking to children
    for (uint_fast32_t i = 0; i <= *key_count; i++) {
        auto page_pointer = children[i];

        if (page_pointer < 0) { // negative number: pointer to dir
            auto& child_page = buffer_manager.get_page_readonly(dir_file_id, page_pointer*-1);
            BPlusTreeDir<N> child(leaf_file_id, &child_page);
            if (!child.check(os))
                return false;
        }
        else { // positive number: pointer to leaf
            auto& child_page = buffer_manager.get_page_readonly(leaf_file_id, page_pointer);
            BPlusTreeLeaf<N> child(&child_page);
            if (!child.check(os))
                return false;
        }
    }
    return true;
}


template <std::size_t N>
void BPlusTreeDir<N>::get_branch_indexes(const Record<N>& r, std::vector<int>& idxs) const {
    auto dir_index = search_child_index(r);
    auto page_pointer = children[dir_index];

    idxs.push_back(dir_index);
    if (page_pointer < 0) { // negative number: pointer to dir
        auto& child_page = buffer_manager.get_page_readonly(dir_file_id, page_pointer*-1);
        BPlusTreeDir<N> child(leaf_file_id, &child_page);
        child.get_branch_indexes(r, idxs);
    }
}


template class BPlusTreeDir<1>;
template class BPlusTreeDir<2>;
template class BPlusTreeDir<3>;
template class BPlusTreeDir<4>;
