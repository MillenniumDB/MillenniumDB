#include "bplus_tree_leaf.h"

#include <cstring>

#include "storage/index/bplus_tree/bplus_tree.h"
#include "system/buffer_manager.h"

using namespace std;

template <std::size_t N>
BPlusTreeLeaf<N>::~BPlusTreeLeaf() {
    if (page != nullptr)
        buffer_manager.unpin(*page);
}


template <std::size_t N>
BPlusTreeLeaf<N> BPlusTreeLeaf<N>::clone() const {
    buffer_manager.pin(*page);
    return BPlusTreeLeaf<N>(page);
}


template <std::size_t N>
void BPlusTreeLeaf<N>::update_to_next_leaf() {
    auto next_page_number = *next_leaf;

    assert(page->page_id.page_number != next_page_number);

    buffer_manager.unpin(*page);

    page        = &buffer_manager.get_page_readonly(leaf_file_id, next_page_number);
    records     = reinterpret_cast<uint64_t*>(page->get_bytes() + (2*sizeof(uint32_t)) );
    value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
    next_leaf   = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
}


template <std::size_t N>
void BPlusTreeLeaf<N>::upgrade_to_editable() {
    if (buffer_manager.need_edit_version(*page)) {

        auto new_page = &buffer_manager.get_page_editable(leaf_file_id, page->get_page_number());
        buffer_manager.unpin(*page);
        page = new_page;

        records     = reinterpret_cast<uint64_t*>(page->get_bytes() + (2*sizeof(uint32_t)) );
        value_count = reinterpret_cast<uint32_t*>(page->get_bytes());
        next_leaf   = reinterpret_cast<uint32_t*>(page->get_bytes() + sizeof(uint32_t));
    }
}


template <std::size_t N>
void BPlusTreeLeaf<N>::get_record(uint_fast32_t pos, Record<N>* out) const {
    for (uint_fast32_t i = 0; i < N; i++) {
        (*out)[i] = records[pos*N + i];
    }
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::delete_record(const Record<N>& record) {
    if (*value_count == 0) {
        return false;
    }

    uint_fast32_t index = search_index(record);
    if (equal_record(record, index)) {
        upgrade_to_editable();
        --(*value_count);
        for (auto i = index; i < (*value_count); i++) {
            for (uint_fast32_t j = 0; j < N; j++) {
                records[i*N + j] = records[(i+1)*N + j];
            }
        }
        return true;
    } else {
        return false;
    }
}


template <std::size_t N>
unique_ptr<BPlusTreeSplit<N>> BPlusTreeLeaf<N>::insert(const Record<N>& record, bool& error) {
    if (*value_count == 0) {
        upgrade_to_editable();

        for (uint_fast32_t i = 0; i < N; i++) {
            records[i] = record[i];
        }
        ++(*value_count);
        error = false;
        return nullptr;
    }
    uint_fast32_t index = search_index(record);

    // avoid inserting duplicated record
    if (equal_record(record, index)) {
        error = true;
        return nullptr;
    }

    error = false;
    upgrade_to_editable();

    if ((*value_count) < BPlusTree<N>::leaf_max_records) {
        shift_right_records(index, (*value_count)-1);

        for (uint_fast32_t i = 0; i < N; i++) {
            records[index*N + i] = record[i];
        }
        ++(*value_count);
        return nullptr;
    } else {
        // put new record and save the last (that does not fit)
        auto last_key = std::array<uint64_t, N>();
        if (index == (*value_count)) { // the last_key will be inserted
            for (uint_fast32_t i = 0; i < N; i++) {
                last_key[i] = record[i];
            }
        }
        else {
            // save last key
            for (uint_fast32_t i = 0; i < N; i++) {
                last_key[i] = records[((*value_count)-1)*N + i];
            }

            shift_right_records(index, (*value_count)-2);
            for (uint_fast32_t i = 0; i < N; i++) {
                records[index*N + i] = record[i];
            }
        }

        // create new leaf
        auto& new_page = buffer_manager.append_vpage(leaf_file_id);
        auto new_leaf = BPlusTreeLeaf<N>(&new_page);

        *new_leaf.next_leaf = *next_leaf;
        *next_leaf = new_leaf.page->get_page_number();

        // write records
        auto middle_index = ((*value_count)+1)/2;

        std::memcpy(
            new_leaf.records,
            &records[middle_index * N],
            (BPlusTree<N>::leaf_max_records - middle_index) * N * sizeof(uint64_t)
        );

        std::memcpy(
            &new_leaf.records[(BPlusTree<N>::leaf_max_records - middle_index) * N],
            last_key.data(),
            N * sizeof(uint64_t)
        );

        // update counts
        *value_count = middle_index;
        *new_leaf.value_count = (BPlusTree<N>::leaf_max_records/2) + 1;

        // split_key is the first in the new leaf
        std::array<uint64_t, N> split_key;
        for (uint_fast32_t i = 0; i < N; i++) {
            split_key[i] = new_leaf.records[i];
        }
        auto split_record = Record<N>(std::move(split_key));

        return make_unique<BPlusTreeSplit<N>>(split_record, new_page.get_page_number());
    }
}


// returns the position of the minimum key greater or equal than the record given.
// if there is no such key, returns (to + 1)
template <std::size_t N>
uint_fast32_t BPlusTreeLeaf<N>::search_index(const Record<N>& record) const noexcept {
    int_fast32_t from = 0;
    int_fast32_t to = static_cast<int_fast32_t>(*value_count)-1;
search_index_begin:
    if (from < to) {
        auto middle = (from + to) / 2;

        for (uint_fast32_t i = 0; i < N; i++) {
            auto id = records[middle*N + i];
            if (record[i] < id) { // record is smaller
                to = middle - 1;
                goto search_index_begin;
            } else if (record[i] > id) { // record is greater
                from = middle + 1;
                goto search_index_begin;
            }
        }
        // record is equal
        return middle;
    }
    // from >= to
    for (uint_fast32_t i = 0; i < N; ++i) {
        auto id = records[from*N + i];
        if (record[i] < id) {
            return from;
        } else if (record[i] > id) {
            return from + 1;
        }
        // continue if they were equal
    }
    return from;
}


template <std::size_t N>
void BPlusTreeLeaf<N>::shift_right_records(int_fast32_t from, int_fast32_t to) {
    for (auto i = to; i >= from; i--) {
        for (uint_fast32_t j = 0; j < N; j++) {
            records[(i+1)*N + j] = records[i*N + j];
        }
    }
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::equal_record(const Record<N>& record, uint_fast32_t index) {
    for (uint_fast32_t i = 0; i < N; i++) {
        if (records[N*index + i] != record[i]) {
            return false;
        }
    }
    return true;
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::check_range(const Record<N>& r) const {
    if (*value_count == 0) {
        return false;
    }
    std::array<uint64_t, N> min;
    std::array<uint64_t, N> max;
    for (uint_fast32_t i = 0; i < N; i++) {
        min[i] = records[0*N + i];
        max[i] = records[(*value_count-1)*N + i];
    }

    return min <= r && r <= max;
}


template <std::size_t N>
void BPlusTreeLeaf<N>::print(std::ostream& os) const {
    os << "Printing Leaf:\n";
    for (uint_fast32_t i = 0; i < (*value_count); i++) {
        os << "  (";
        for (uint_fast32_t j = 0; j < N; j++) {
            if (j != 0)
                os << ", ";
            os << records[i*N + j];
        }
        os << ")\n";
    }
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::check(std::ostream& os) const {
    if ((*value_count) == 0) {
        if (page->get_page_number() == 0) {
            os << "  WARNING: empty leaf. Ok only if the b+tree is empty.\n";
        } else {
            os << "  ERROR: BPlusTreeLeaf empty (page: " << page->get_page_number() << ")\n";
            return false;
        }
    } else {
        // check keys are ordered
        std::array<uint64_t, N> x;
        std::array<uint64_t, N> y;

        uint_fast32_t current_pos = 0;
        while (current_pos < N) {
            if (records[current_pos] == 0xFFFF'FFFF'FFFF'FFFF) {
                os << "  ERROR: record not_found(0xFFFF'FFFF'FFFF'FFFF) at BPlusTreeLeaf\n";
                return false;
            }
            x[current_pos] = records[current_pos];
            current_pos++;
        }

        for (uint_fast32_t k = 1; k < (*value_count); k++) {
            for (uint_fast32_t i = 0; i < N; i++) {
                y[i] = records[current_pos++];
            }
            if (y <= x) {
                os << "  ERROR: bad record order at BPlusTreeLeaf(page: " << page->get_page_number() << ")\n";
                for (size_t n = 0; n < N; n++) {
                    os << "\t" << x[n];
                }
                os << "\n";

                for (size_t n = 0; n < N; n++) {
                    os << "\t" << y[n];
                }
                os << "\n";

                // print(os);
                return false;
            }
            x = y;
        }
    }
    return true;
}

template class BPlusTreeLeaf<1>;
template class BPlusTreeLeaf<2>;
template class BPlusTreeLeaf<3>;
template class BPlusTreeLeaf<4>;
