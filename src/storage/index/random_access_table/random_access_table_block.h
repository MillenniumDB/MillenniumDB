#pragma once

#include "storage/index/record.h"
#include "storage/page/versioned_page.h"

// N is the columns of the table
template <std::size_t N> class RandomAccessTableBlock {
public:
    static constexpr auto max_records = (VPage::SIZE - sizeof(uint32_t)) / (sizeof(uint64_t) * N);

    RandomAccessTableBlock(VPage& page);
    ~RandomAccessTableBlock();

    // assumes a new record fits (*record_count < max_records)
    void append_record(const Record<N>&);

    // in case of out-of-bounds returns nullptr
    // pointer is valid until next operator[] call or the destruction of this object
    Record<N>* operator[](uint64_t pos);

// private:
    VPage& page;
    uint64_t* const records;
    uint32_t* const record_count;
};
