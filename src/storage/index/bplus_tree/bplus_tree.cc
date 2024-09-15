#include "bplus_tree.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "storage/index/record.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"

template <std::size_t N>
BPlusTree<N>::BPlusTree(const std::string& name) :
    dir_file_id  (file_manager.get_file_id(name + ".dir")),
    leaf_file_id (file_manager.get_file_id(name + ".leaf")) { }


template <std::size_t N>
std::unique_ptr<BPlusTreeDir<N>> BPlusTree<N>::get_root() const noexcept {
    return std::make_unique<BPlusTreeDir<N>>(
        leaf_file_id,
        &buffer_manager.get_page_readonly(dir_file_id, 0)
    );
}


template <std::size_t N>
BptIter<N> BPlusTree<N>::get_range(bool* interruption_requested,
                                   const Record<N>& min,
                                   const Record<N>& max) const noexcept {
    BPlusTreeDir<N> root(
        leaf_file_id,
        &buffer_manager.get_page_readonly(dir_file_id, 0)
    );
    auto leaf_and_pos = root.search_leaf(min);
    return BptIter<N>(interruption_requested, std::move(leaf_and_pos), max);
}


template <std::size_t N>
bool BPlusTree<N>::insert(const Record<N>& record) {
    // although root can be modified in insert, we start as readonly, and
    // it will create a new version in the insert method if needed
    BPlusTreeDir<N> root(
        leaf_file_id,
        &buffer_manager.get_page_readonly(dir_file_id, 0)
    );
    bool error;
    root.insert(record, error);
    return !error;
}


template <std::size_t N>
bool BPlusTree<N>::delete_record(const Record<N>& record) {
    // although root can be modified in insert, we start as readonly, and
    // it will create a new version in the insert method if needed
    BPlusTreeDir<N> root(
        leaf_file_id,
        &buffer_manager.get_page_readonly(dir_file_id, 0)
    );
    return root.delete_record(record);
}


template <std::size_t N>
bool BPlusTree<N>::check(std::ostream& os) const {
    BPlusTreeDir<N> root(
        leaf_file_id,
        &buffer_manager.get_page_readonly(dir_file_id, 0)
    );
    return root.check(os);
}


uint64_t powi(uint64_t base, size_t exp) {
    uint64_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        exp >>= 1;
        base *= base;
    }
    return res;
}


template <std::size_t N>
double BPlusTree<N>::estimate_records(const Record<N>& min,
                                      const Record<N>& max) const
{
    BPlusTreeDir<N> root(
        leaf_file_id,
        &buffer_manager.get_page_readonly(dir_file_id, 0)
    );
    return BPlusTree<N>::estimate_records(root, min, max);
}


template <std::size_t N>
double BPlusTree<N>::estimate_records(const BPlusTreeDir<N>& root,
                                      const Record<N>& min,
                                      const Record<N>& max)
{
    std::vector<int> min_idxs;
    std::vector<int> max_idxs;

    root.get_branch_indexes(min, min_idxs);
    root.get_branch_indexes(max, max_idxs);

    assert(min_idxs.size() == max_idxs.size());
    assert(min_idxs.size() > 0);

    double estimated_min = 0;
    double estimated_max = 0;
    size_t current_exponent = max_idxs.size() - 1;
    for (size_t i = 0; i < min_idxs.size(); i++) {
        estimated_min += min_idxs[i] * powi(dir_max_records+1, current_exponent);
        estimated_max += max_idxs[i] * powi(dir_max_records+1, current_exponent);
        current_exponent--;
    }
    return 1 + (estimated_max - estimated_min) * leaf_max_records;
}


/******************************* BptIter ********************************/
template <std::size_t N>
const Record<N>* BptIter<N>::next() {
    while (true) {
        if (MDB_unlikely(*interruption_requested)) {
            throw InterruptedException();
        }
        if (current_pos < current_leaf.get_value_count()) {
            current_leaf.get_record(current_pos, &current_record);
            // check if res is less than max
            for (size_t i = 0; i < N; ++i) {
                if (current_record[i] < max[i]) {
                    ++current_pos;
                    return &current_record;
                }
                else if (current_record[i] > max[i]) {
                    return nullptr;
                }
                // continue iterating if current_record[i] == max[i]
            }
            ++current_pos;
            return &current_record; // res == max
        }
        else if (current_leaf.has_next()) {
            current_leaf.update_to_next_leaf();
            current_pos = 0;
            // continue while
        }
        else {
            return nullptr;
        }
    }
}


template class BPlusTree<1>;
template class BPlusTree<2>;
template class BPlusTree<3>;
template class BPlusTree<4>;

template class BptIter<1>;
template class BptIter<2>;
template class BptIter<3>;
template class BptIter<4>;
