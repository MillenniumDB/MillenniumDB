#include "leapfrog_bpt_iter.h"

#include <cassert>
#include <iostream>

#include "base/exceptions.h"
#include "storage/buffer_manager.h"

using namespace std;

template <size_t N>
LeapfrogBptIter<N>::LeapfrogBptIter(bool*                         interruption_requested,
                                    const BPlusTree<N>&           btree,
                                    vector<unique_ptr<ScanRange>> _initial_ranges,
                                    vector<VarId>                 _intersection_vars,
                                    vector<VarId>                 _enumeration_vars) :
    LeapfrogIter (interruption_requested,
                  move(_initial_ranges),
                  move(_intersection_vars),
                  move(_enumeration_vars))
{
    auto root = btree.get_root();
    directory_stack.push( move(root) );

    // there is a border case when nothing is done, but enumeration, so we must
    // position at the first record
    array<uint64_t, N> min;
    for (size_t i = 0; i < N; i++) {
        min[i] = 0;
    }

    auto leaf_and_pos = directory_stack.top()->search_leaf(directory_stack, min);
    current_leaf = move(leaf_and_pos.leaf);
    assert(current_leaf != nullptr);
    current_pos_in_leaf = leaf_and_pos.result_index;

    // check border case when B+tree is empty
    if (current_pos_in_leaf < current_leaf->get_value_count()) {
        current_tuple = current_leaf->get_record(current_pos_in_leaf);
    } else {
        array<uint64_t, N> max;
        for (size_t i = 0; i < N; i++) {
            min[i] = UINT64_MAX;
        }
        current_tuple = make_unique<Record<N>>(max);
    }
}


template <size_t N>
void LeapfrogBptIter<N>::down() {
    assert(current_tuple != nullptr);
    level++;

    array<uint64_t, N> min;
    array<uint64_t, N> max;

    // before the level min and max must be equal to the current_record
    for (int_fast32_t i = 0; i < level; i++) {
        min[i] = (*current_tuple)[i];
        max[i] = (*current_tuple)[i];
    }

    // from level until the end is an open range [0, MAX]
    for (uint_fast32_t i = level; i < N; i++) {
        min[i] = 0;
        max[i] = UINT64_MAX;
    }

    internal_search(Record<N>(min), Record<N>(max));
}


template <size_t N>
bool LeapfrogBptIter<N>::next() {
    assert(current_tuple != nullptr);
    array<uint64_t, N> min;
    array<uint64_t, N> max;

    // before level min and max are equal to the current_record
    for (int_fast32_t i = 0; i < level; i++) {
        min[i] = (*current_tuple)[i];
        max[i] = (*current_tuple)[i];
    }

    // at the same level min is 1 grater than the current record and max is unbound
    min[level] = (*current_tuple)[level] + 1;
    max[level] = UINT64_MAX;

    // after level min is 0 and max is unbound
    for (size_t i = level+1; i < N; i++) {
        min[i] = 0;
        max[i] = UINT64_MAX;
    }

    return internal_search(Record<N>(min), Record<N>(max));
}


template <size_t N>
bool LeapfrogBptIter<N>::seek(uint64_t key) {
    assert(current_tuple != nullptr);
    array<uint64_t, N> min;
    array<uint64_t, N> max;

    // before level min and max are equal to the current_record
    for (int_fast32_t i = 0; i < level; i++) {
        min[i] = (*current_tuple)[i];
        max[i] = (*current_tuple)[i];
    }

    min[level] = key;
    max[level] = UINT64_MAX;

    // after level min is 0 and max is unbound
    for (uint_fast32_t i = level+1; i < N; i++) {
        min[i] = 0;
        max[i] = UINT64_MAX;
    }
    return internal_search(Record<N>(min), Record<N>(max));
}


// updates current_leaf, current_tuple and current_pos_in_leaf only when returns true;
template <std::size_t N>
bool LeapfrogBptIter<N>::internal_search(const Record<N>& min, const Record<N>& max) {
    assert(current_leaf != nullptr);

    // if leaf.min <= min <= leaf.max, search inside the leaf and return
    if (current_leaf->check_range(min)) {
        auto new_current_pos_in_leaf = current_leaf->search_index(min);
        // check new_current_pos_in_leaf is a valid position
        if (new_current_pos_in_leaf < current_leaf->get_value_count()) {
            auto new_current_tuple = current_leaf->get_record(new_current_pos_in_leaf);
            if ((*new_current_tuple) <= max) {
                // current_leaf stays the same
                current_tuple       = move(new_current_tuple);
                current_pos_in_leaf = new_current_pos_in_leaf;
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        // else search in the stack for a dir where dir.min <= min <= dir.max
        // a dir may not have records (i.e when having one leaf as child), in that case it will return the a record with zeros
        // and conditions will be false, so its ok
        // if we don't find it we stay with the root (lowest item in the stack)
        while (directory_stack.size() > 1
               && !directory_stack.top()->check_range(min))
        {
            directory_stack.pop();
        }

        // then search until reaching the leaf_number and index of the record.
        auto leaf_and_pos = directory_stack.top()->search_leaf(directory_stack, min);
        auto new_current_leaf = move(leaf_and_pos.leaf);
        auto new_current_pos_in_leaf = leaf_and_pos.result_index;

        // check new_current_pos_in_leaf is a valid position
        // we may need to go to the first record of the next leaf
        if (new_current_pos_in_leaf >= new_current_leaf->get_value_count()) {
            if (new_current_leaf->has_next()) {
                new_current_leaf = new_current_leaf->get_next_leaf();
                new_current_pos_in_leaf = 0;
            } else {
                return false;
            }
        }

        auto new_current_tuple = new_current_leaf->get_record(new_current_pos_in_leaf);
        if ((*new_current_tuple) <= max) {
            current_tuple       = move(new_current_tuple);
            current_leaf        = move(new_current_leaf);
            current_pos_in_leaf = new_current_pos_in_leaf;
            return true;
        } else {
            return false;
        }
    }
}


template <size_t N>
void LeapfrogBptIter<N>::begin_enumeration() {
    array<uint64_t, N> max;

    for (int_fast32_t i = 0; i <= level; i++) {
        max[i] = (*current_tuple)[i];
    }
    for (size_t i = level+1; i < N; i++) {
        max[i] = UINT64_MAX;
    }

    enum_bpt_iter = make_unique<BptIter<N>>(interruption_requested,
                                            SearchLeafResult<N>(current_leaf->duplicate(), current_pos_in_leaf),
                                            Record<N>(max));
}


template <size_t N>
void LeapfrogBptIter<N>::reset_enumeration() {
    begin_enumeration();
}


template <size_t N>
bool LeapfrogBptIter<N>::next_enumeration(BindingId& binding) {
    auto record = enum_bpt_iter->next();
    if (record != nullptr) {
        // assign to binding
        for (uint_fast32_t i = 0; i < enumeration_vars.size(); i++) {
            binding.add(enumeration_vars[i],
                        ObjectId(record->ids[initial_ranges.size() + intersection_vars.size() + i]));
        }
        return true;
    } else {
        return false;
    }
}


template <size_t N>
bool LeapfrogBptIter<N>::open_terms(BindingId& input_binding) {
    assert(current_tuple != nullptr);
    array<uint64_t, N> min;
    array<uint64_t, N> max;

    // before level min and max are equal to the current_record
    size_t i = 0;
    for (; i < initial_ranges.size(); i++) {
        min[i] = initial_ranges[i]->get_min(input_binding);
        max[i] = initial_ranges[i]->get_max(input_binding);
    }

    for (; i < N; i++) {
        min[i] = 0;
        max[i] = UINT64_MAX;
    }

    level = initial_ranges.size() - 1;
    return internal_search(Record<N>(min), Record<N>(max));
}

template class LeapfrogBptIter<2>;
template class LeapfrogBptIter<3>;
template class LeapfrogBptIter<4>;
