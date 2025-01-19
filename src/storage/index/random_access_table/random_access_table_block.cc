#include "random_access_table_block.h"

#include "system/buffer_manager.h"

template<std::size_t N>
RandomAccessTableBlock<N>::RandomAccessTableBlock(VPage& page) :
    page(page),
    records(reinterpret_cast<uint64_t*>(page.get_bytes())),
    record_count(reinterpret_cast<uint32_t*>(
        page.get_bytes() + (N * sizeof(uint64_t) * RandomAccessTableBlock<N>::max_records)
    ))
{ }

template<std::size_t N>
RandomAccessTableBlock<N>::~RandomAccessTableBlock()
{
    buffer_manager.unpin(page);
}

template<std::size_t N>
void RandomAccessTableBlock<N>::append_record(const Record<N>& record)
{
    assert(*record_count < max_records);
    for (std::size_t i = 0; i < N; ++i) {
        records[(*record_count) * N + i] = record[i];
    }
    ++(*record_count);
    // page.make_dirty();
}

template<std::size_t N>
Record<N>* RandomAccessTableBlock<N>::operator[](uint64_t pos)
{
    if (pos < max_records) {
        return reinterpret_cast<Record<N>*>(&records[pos * N]);
    } else {
        return nullptr;
    }
}

template class RandomAccessTableBlock<3>;
template class RandomAccessTableBlock<2>;
