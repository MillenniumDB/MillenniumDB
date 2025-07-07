#pragma once

#include "storage/index/text_search/index_iter.h"
#include "storage/index/text_search/ordered_iter.h"
#include "storage/index/text_search/text_search_iter.h"

namespace TextSearch {

class MultiIter : public TextSearchIter {
public:
    MultiIter(std::vector<std::unique_ptr<IndexIter>> index_iters);

    bool next() override;

    uint64_t get_table_pointer() const override;

    uint64_t get_score() const override;

private:
    std::vector<std::unique_ptr<OrderedIter>> iters;
    uint64_t table_pointer;
    uint64_t score;
};

} // namespace TextSearch
