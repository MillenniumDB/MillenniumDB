#include "random_access_table_block.h"

#include "storage/buffer_manager.h"

using namespace std;

template <std::size_t N>
RandomAccessTableBlock<N>::RandomAccessTableBlock(Page& page) :
    page         (page),
    records      (reinterpret_cast<uint64_t*>(page.get_bytes())),
    record_count (reinterpret_cast<uint32_t*>(page.get_bytes()
                  + (N * sizeof(uint64_t) * RandomAccessTableBlock<N>::max_records)))
    { }


template <std::size_t N>
RandomAccessTableBlock<N>::~RandomAccessTableBlock() {
    buffer_manager.unpin(page);
}


template <std::size_t N>
bool RandomAccessTableBlock<N>::try_append_record(Record<N> record) {
    if (*record_count < max_records) {
        for (std::size_t i = 0; i < N; ++i) {
            records[ (*record_count)*N + i ] = record.ids[i];
        }
        ++(*record_count);
        page.make_dirty();
        return true;
    } else {
        return false;
    }
}


template <std::size_t N>
unique_ptr<Record<N>> RandomAccessTableBlock<N>::operator[](uint_fast32_t pos) {
    if (pos < max_records) {
        std::array<uint64_t, N> ids;
        for (uint_fast32_t i = 0; i < N; i++) {
            ids[i] = records[pos*N + i];
        }
        return make_unique<Record<N>>(move(ids));
    } else {
        return nullptr;
    }
}

template class RandomAccessTableBlock<3>;
