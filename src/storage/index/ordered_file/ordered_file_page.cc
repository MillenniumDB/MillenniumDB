#include "ordered_file_page.h"

#include <cassert>
#include <iostream>

#include "storage/buffer_manager.h"

using namespace std;

template <std::size_t N>
OrderedFilePage<N>::OrderedFilePage(Page& page) noexcept :
    page    (page),
    size    (reinterpret_cast<uint32_t*>(page.get_bytes())),
    // OrderedFilePage must be coherent with the alignment of a BPlusTreeLeaf so then we can copy the
    // entire page at bulk import, so we skiped space for next_leaf when setting records
    records (reinterpret_cast<uint64_t*>(page.get_bytes() + (2*sizeof(uint32_t)) ))
{
    assert(*size <= MAX_RECORDS);
}


template <std::size_t N>
OrderedFilePage<N>::~OrderedFilePage() {
    page.make_dirty();
    buffer_manager.unpin(page);
}


template <std::size_t N>
void OrderedFilePage<N>::append_record(const std::array<uint64_t, N>& record) noexcept {
    assert(*size < MAX_RECORDS);
    for (uint_fast8_t i = 0; i < N; ++i) {
        records[(*size)*N + i] = record[i];
    }
    (*size)++;
}


template <std::size_t N>
void OrderedFilePage<N>::reorder_columns(const std::array<uint_fast8_t, N>& column_order) noexcept {
    std::array<uint64_t, N> key;
    for (uint32_t i = 0; i < *size; i++) {
        for (size_t n = 0; n < N; n++) {
            key[column_order[n]] = records[i*N + n];
        }
        for (size_t n = 0; n < N; n++) {
            records[i*N + n] = key[n];
        }
    }
}


template <std::size_t N>
void OrderedFilePage<N>::order() noexcept {
    assert(*size <= MAX_RECORDS);

    // insertion sort
    for (uint32_t i = 1; i < *size; i++) {
        const auto key = get(i);
        uint32_t j = i - 1;
        while ( key < get(j) ) {
            // move record j to the right
            for (size_t n = 0; n < N; n++) {
                records[(j+1)*N + n] = records[j*N + n];
            }

            if (j == 0) { // necesary to check this because j is unsigned
                j--; // this will overflow but we will use j+1 to overflow back to 0
                break;
            }
            j--;
        }
        // insert key at position j+1
        for (size_t n = 0; n < N; n++) {
            records[(j+1)*N + n] = key[n];
        }
    }
}


template <std::size_t N>
std::array<uint64_t, N> OrderedFilePage<N>::get(const uint_fast32_t pos) const noexcept {
    assert(pos <= MAX_RECORDS);
    std::array<uint64_t, N> res;
    for (size_t i = 0; i < N; ++i) {
        res[i] = records[pos*N + i];
    }
    return res;
}

template class OrderedFilePage<1>;
template class OrderedFilePage<2>;
template class OrderedFilePage<3>;
template class OrderedFilePage<4>;
