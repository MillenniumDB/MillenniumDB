#pragma once

#include <memory>

#include "storage/file_id.h"
#include "storage/index/text_search/index_iter.h"
#include "storage/tuple_collection/tuple_collection.h"

namespace TextSearch {

// This class materializes and sorts an IndexIter
class OrderedIter {
public:
    OrderedIter(std::unique_ptr<IndexIter> index_iter);

    ~OrderedIter();

    // Returns true when a result is available
    bool next();

    // Used to reset this iterator, currently not used
    void reset();

    // Used to obtain the table pointer after next() returns true
    uint64_t get_table_pointer() const;

private:
    const TmpFileId first_file_id;
    const TmpFileId second_file_id;

    std::unique_ptr<TupleCollectionPage> run;

    const TmpFileId* output_file_id;

    uint_fast32_t total_pages = 0;

    uint_fast32_t current_page = 0;

    uint64_t page_position = 0;

    OrderInfo order_info;

    // Set when next() return true, can be obtained using get_table_pointer
    uint64_t table_pointer;

    std::unique_ptr<TupleCollectionPage> get_run(PPage& run_page);

    void merge_sort();
};

} // namespace TextSearch
