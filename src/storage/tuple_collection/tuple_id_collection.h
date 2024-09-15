// A TupleIdCollection allows you to store arrangements
// of ObjectIds on disk, the purpose of this class is to abstract the
// operations of saving and reading the tuples on disk that a physical operator requires.

// TupleIdCollection assumes that all the arrays of ObjectId have the same size
#pragma once

#include <map>
#include <memory>
#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "storage/file_id.h"
#include "storage/page/private_page.h"

class MergeOrderedTupleIdCollection;

class TupleIdCollection {
friend class MergeOrderedTupleIdCollection;
public:
    TupleIdCollection(
        PPage& page,
        const std::map<VarId, uint_fast32_t>& saved_vars,
        const std::vector<VarId>& order_vars,
        const std::vector<bool>& ascending,
        int64_t(*compare)(ObjectId, ObjectId)
    );

    ~TupleIdCollection();

    bool is_full() const {
        return sizeof(tuple_count) + (sizeof(ObjectId)*saved_vars.size()*(1 + *tuple_count)) > PPage::SIZE;
    }

    inline uint64_t get_tuple_count() const noexcept { return *tuple_count; }

    std::vector<ObjectId> get(uint64_t n) const;

    bool has_priority(const std::vector<ObjectId>& lhs,
                      const std::vector<ObjectId>& rhs);

    void add(std::vector<ObjectId> new_tuple);
    void sort();
    void reset();

private:
    PPage& page;
    const std::map<VarId, uint_fast32_t>& saved_vars;
    const std::vector<VarId>& order_vars;
    const std::vector<bool>& ascending;
    ObjectId* const tuples;
    uint64_t* const tuple_count;

    int64_t(*compare)(ObjectId, ObjectId);

    void swap(int x, int y);
    void override_tuple(const std::vector<ObjectId>& tuple, int position);
    int partition(int i, int f);
    void quicksort(int i, int f);
};


// MergeOrderedTupleIdCollection abstract the merge of two
// sorted runs (arrays of tuple collections)
class MergeOrderedTupleIdCollection {
public:
    MergeOrderedTupleIdCollection(const std::map<VarId, uint_fast32_t>& saved_vars,
                                  const std::vector<VarId>&             order_vars,
                                  const std::vector<bool>&              ascending,
                                  int64_t(*compare)(ObjectId, ObjectId));

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

    int64_t(*compare)(ObjectId, ObjectId);

    std::unique_ptr<TupleIdCollection> get_run(PPage& run_page);
};
