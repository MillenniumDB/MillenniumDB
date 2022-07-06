#include "bplus_tree_leaf.h"

#include <iostream>
#include <cstring>

#include "base/exceptions.h"
#include "storage/index/bplus_tree/bplus_tree.h"

using namespace std;

template <std::size_t N>
unique_ptr<Record<N>> BPlusTreeLeaf<N>::get_record(uint_fast32_t pos) const {
    std::array<uint64_t, N> ids;
    for (uint_fast32_t i = 0; i < N; i++) {
        ids[i] = records[pos*N + i];
    }
    return make_unique<Record<N>>(move(ids));
}


template <std::size_t N>
unique_ptr<BPlusTreeSplit<N>> BPlusTreeLeaf<N>::insert(const Record<N>& record) {
    if (*value_count == 0) {
        for (uint_fast32_t i = 0; i < N; i++) {
            records[i] = record.ids[i];
        }
        ++(*value_count);
        this->page.make_dirty();
        return nullptr;
    }
    uint_fast32_t index = search_index(record);
    if (equal_record(record, index)) {
        for (uint_fast32_t i = 0; i < N; i++) {
            cout << record.ids[i] << " ";
        }
        cout << "\n";
        for (uint_fast32_t i = 0; i < N; i++) {
            cout << records[N*index + i] << " ";
        }
        cout << "\n";

        throw LogicException("Inserting duplicated record into BPlusTree.");
    }

    if ((*value_count) < BPlusTree<N>::leaf_max_records) {
        shift_right_records(index, (*value_count)-1);

        for (uint_fast32_t i = 0; i < N; i++) {
            records[index*N + i] = record.ids[i];
        }
        ++(*value_count);
        this->page.make_dirty();
        return nullptr;
    }
    else {
        // poner nuevo record y guardar el ultimo (que no cabe)
        auto last_key = std::array<uint64_t, N>();
        if (index == (*value_count)) { // la llave a insertar es la ultima
            for (uint_fast32_t i = 0; i < N; i++) {
                last_key[i] = record.ids[i];
            }
        }
        else {
            // guardar ultima llave
            for (uint_fast32_t i = 0; i < N; i++) {
                last_key[i] = records[((*value_count)-1)*N + i];
            }

            shift_right_records(index, (*value_count)-2);
            for (uint_fast32_t i = 0; i < N; i++) {
                records[index*N + i] = record.ids[i];
            }
        }

        // crear nueva hoja
        auto& new_page = buffer_manager.append_page(leaf_file_id);
        auto new_leaf = BPlusTreeLeaf<N>(new_page);

        *new_leaf.next_leaf = *next_leaf;
        *next_leaf = new_leaf.page.get_page_number();

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
        auto split_record = Record<N>(move(split_key));
        this->page.make_dirty();
        new_page.make_dirty();

        return make_unique<BPlusTreeSplit<N>>(split_record, new_page.get_page_number());
    }
}


// returns the position of the minimum key greater or equal than the record given.
// if there is no such key, returns (to + 1)
template <std::size_t N>
uint_fast32_t BPlusTreeLeaf<N>::search_index(const Record<N>& record) const noexcept {
    int_fast32_t from = 0;
    int_fast32_t to = (*value_count)-1;
search_index_begin:
    if (from < to) {
        auto middle = (from + to) / 2;

        for (uint_fast32_t i = 0; i < N; i++) {
            auto id = records[middle*N + i];
            if (record.ids[i] < id) { // record is smaller
                to = middle - 1;
                goto search_index_begin;
            } else if (record.ids[i] > id) { // record is greater
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
        if (record.ids[i] < id) {
            return from;
        } else if (record.ids[i] > id) {
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
        if (records[N*index + i] != record.ids[i]) {
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

    return min <= r.ids && r.ids <= max;
}


template <std::size_t N>
void BPlusTreeLeaf<N>::print() const {
    cout << "Printing Leaf:\n";
    for (uint_fast32_t i = 0; i < (*value_count); i++) {
        cout << "  (";
        for (uint_fast32_t j = 0; j < N; j++) {
            if (j != 0)
                cout << ", ";
            cout << records[i*N + j];
        }
        cout << ")\n";
    }
}


template <std::size_t N>
bool BPlusTreeLeaf<N>::check() const {
    if ((*value_count) == 0) {
        if (page.get_page_number() == 0) {
            cout << "  WARNING: empty leaf. Ok only if the b+tree is empty.\n";
        } else {
            cerr << "  ERROR: BPlusTreeLeaf empty (page: " << page.get_page_number() << ")\n";
            return false;
        }
    } else {
        // check keys are ordered
        std::array<uint64_t, N> x;
        std::array<uint64_t, N> y;

        uint_fast32_t current_pos = 0;
        while (current_pos < N) {
            if (records[current_pos] == 0xFFFF'FFFF'FFFF'FFFF) {
                cerr << "  ERROR: record not_found(0xFFFF'FFFF'FFFF'FFFF) at BPlusTreeLeaf\n";
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
                cerr << "  ERROR: bad record order at BPlusTreeLeaf(page: " << page.get_page_number() << ")\n";
                for (size_t n = 0; n < N; n++) {
                    cerr << "\t" << x[n];
                }
                cerr << "\n";

                for (size_t n = 0; n < N; n++) {
                    cerr << "\t" << y[n];
                }
                cerr << "\n";

                // print();
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
