#include "random_access_table.h"

#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;

template <size_t N>
RandomAccessTable<N>::RandomAccessTable(const string& name) :
    file_id(file_manager.get_file_id(name))
{
    // important to call buffer manager twice, so pins = 2
    last_block    = make_unique<RandomAccessTableBlock<N>>(buffer_manager.get_last_page(file_id));
    current_block = make_unique<RandomAccessTableBlock<N>>(buffer_manager.get_last_page(file_id));
}


template <size_t N>
unique_ptr<Record<N>> RandomAccessTable<N>::operator[](uint_fast32_t pos) {
    auto block        = pos / RandomAccessTableBlock<N>::max_records;
    auto pos_in_block = pos % RandomAccessTableBlock<N>::max_records;

    if (current_block->page.get_page_number() != block) {
        current_block = make_unique<RandomAccessTableBlock<N>>(buffer_manager.get_page(file_id, block));
    }
    if (pos_in_block < *(current_block->record_count)) {
        return (*current_block)[pos_in_block];
    } else {
        return nullptr;
    }
}


template <size_t N>
void RandomAccessTable<N>::append_record(Record<N> record) {
    while (!last_block->try_append_record(record)) {
        last_block = make_unique<RandomAccessTableBlock<N>>(buffer_manager.append_page(file_id));
    }
}

template class RandomAccessTable<3>;
