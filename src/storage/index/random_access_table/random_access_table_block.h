#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "storage/index/record.h"
#include "storage/page.h"

// N is the columns of the table
template <std::size_t N> class RandomAccessTableBlock {
public:
    static constexpr auto max_records = (Page::MDB_PAGE_SIZE - sizeof(uint32_t)) / (sizeof(uint64_t) * N);

    RandomAccessTableBlock(Page& page);
    ~RandomAccessTableBlock();

    // returns false if record was not inserted
    bool try_append_record(Record<N>);

    // in case of out-of-bounds returns nullptr
    std::unique_ptr<Record<N>> operator[](uint_fast32_t pos);

// private:
    Page& page;
    uint64_t* const records;
    uint32_t* const record_count;
};
