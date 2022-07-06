#pragma once

#include "storage/index/record.h"

template <std::size_t N>
struct BPlusTreeSplit {
    BPlusTreeSplit(const Record<N>& record, int_fast32_t encoded_page_number) :
        record(record),
        encoded_page_number(encoded_page_number) { }

    Record<N> record;
    // positive number: pointer to leaf, negative number: pointer to dir
    int_fast32_t encoded_page_number;
};
