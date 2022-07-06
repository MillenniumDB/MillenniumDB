// A TupleCollection allows you to store arrangements
// of GraphObjects on disk, the purpose of this class is to abstract the
// operations of saving and reading the tuples on disk that a physical operator requires.

// TupleCollection asumes that all the arrays of GraphObject have the same size
#pragma once

#include <map>
#include <vector>

#include "base/graph_object/graph_object.h"
#include "base/ids/var_id.h"
#include "storage/file_id.h"
#include "storage/page_id.h"
#include "storage/page.h"

class MergeOrderedTupleCollection;

class TupleCollection {
friend class MergeOrderedTupleCollection;
public:
    TupleCollection(Page& page,
                    const std::map<VarId, uint_fast32_t>& saved_vars,
                    const std::vector<VarId>& order_vars,
                    const std::vector<bool>& ascending);
    ~TupleCollection();

    bool is_full() const {
        return sizeof(tuple_count) + (sizeof(GraphObject)*saved_vars.size()*(1 + *tuple_count)) > Page::MDB_PAGE_SIZE;
    }

    inline uint64_t get_tuple_count() const noexcept { return *tuple_count; }

    std::vector<GraphObject> get(uint64_t n) const;

    bool has_priority(const std::vector<GraphObject>& lhs,
                      const std::vector<GraphObject>& rhs);

    void add(std::vector<GraphObject> new_tuple);
    void sort();
    void reset();

private:
    Page& page;
    const std::map<VarId, uint_fast32_t>& saved_vars;
    const std::vector<VarId>& order_vars;
    const std::vector<bool>& ascending;
    GraphObject* const tuples;
    uint64_t* const tuple_count;

    void swap(int x, int y);
    void override_tuple(const std::vector<GraphObject>& tuple, int position);
    int partition(int i, int f);
    void quicksort(int i, int f);
};


// MergeOrderedTupleCollection abstract the merge of two
// sorted runs (arrays of tuple collections)
class MergeOrderedTupleCollection {
public:
    MergeOrderedTupleCollection(const std::map<VarId, uint_fast32_t>& saved_vars,
                                const std::vector<VarId>&             order_vars,
                                const std::vector<bool>&              ascending,
                                bool*                                 interruption_requested);

    void merge(uint64_t  left_start,
               uint64_t  left_end,
               uint64_t  right_start,
               uint64_t  right_end,
               TmpFileId source_file_id,
               TmpFileId output_file_id);

    void copy_page(uint64_t  source_page,
                   TmpFileId source_file_id,
                   TmpFileId output_file_id);

private:
    const std::map<VarId, uint_fast32_t>& saved_vars;
    const std::vector<VarId>&             order_vars;
    const std::vector<bool>&              ascending;
    bool const *                          interruption_requested;

    std::unique_ptr<TupleCollection> get_run(Page& run_page);
};
