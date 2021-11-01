#ifndef STORAGE__B_PLUS_TREE_SPLIT_H_
#define STORAGE__B_PLUS_TREE_SPLIT_H_

#include "storage/index/record.h"

template <std::size_t N>
struct BPlusTreeSplit {
    BPlusTreeSplit(const Record<N>& record, int encoded_page_number) :
        record(record),
        encoded_page_number(encoded_page_number) { }

    Record<N> record;
    // positive number: pointer to leaf, negative number: pointer to dir
    int encoded_page_number;
};

#endif // STORAGE__B_PLUS_TREE_SPLIT_H_
