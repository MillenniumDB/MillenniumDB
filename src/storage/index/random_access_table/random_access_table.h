#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "storage/index/random_access_table/random_access_table_block.h"
#include "storage/index/record.h"
#include "storage/page.h"

// N is the columns of the table
template <std::size_t N> class RandomAccessTable {
public:
    RandomAccessTable(const std::string& filename);
    ~RandomAccessTable() = default;

    void append_record(Record<N>);

    // in case of out-of-bounds returns nullptr
    std::unique_ptr<Record<N>> operator[](uint_fast32_t pos);

private:
    const FileId file_id;

    std::unique_ptr<RandomAccessTableBlock<N>> current_block;
    std::unique_ptr<RandomAccessTableBlock<N>> last_block;
};
