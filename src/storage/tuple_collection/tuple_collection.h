// A TupleIdCollection allows you to store arrangements
// of ObjectIds on disk, the purpose of this class is to abstract the
// operations of saving and reading the tuples on disk that a physical operator requires.

// TupleIdCollection assumes that all the arrays of ObjectId have the same size
#pragma once

#include <memory>
#include <set>
#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "storage/file_id.h"
#include "storage/page/private_page.h"

struct OrderInfo {
    // saved_vars must start with the order_vars
    std::vector<VarId> saved_vars;

    std::vector<bool> ascending;

    OrderInfo(
        std::vector<VarId>&& order_vars,
        std::vector<bool>&& ascending,
        const std::set<VarId>& other_saved_vars
    ) :
        ascending(ascending)
    {
        // to avoid having duplicates in saved_vars
        std::set<VarId> inserted_vars;

        for (auto& var : order_vars) {
            if (inserted_vars.insert(var).second) {
                saved_vars.push_back(var);
            }
        }

        assert(saved_vars.size() == ascending.size());

        for (auto& var : other_saved_vars) {
            if (inserted_vars.insert(var).second) {
                saved_vars.push_back(var);
            }
        }
    }
};

class TupleCollectionPage {
    friend class TupleCollectionMerger;

public:
    TupleCollectionPage(PPage& page, OrderInfo& info, int64_t (*compare)(ObjectId, ObjectId));

    ~TupleCollectionPage();

    bool is_full() const
    {
        return sizeof(tuple_count) + (sizeof(ObjectId) * info.saved_vars.size() * (1 + *tuple_count))
             > PPage::SIZE;
    }

    inline uint64_t get_tuple_count() const noexcept
    {
        return *tuple_count;
    }

    const ObjectId* get(uint64_t n) const;

    bool less_or_equal(const ObjectId* lhs, const ObjectId* rhs) const;

    // expects a tuple with values corresponding to saved_vars
    void add(const ObjectId* tuple);

    void sort();

    void reset();

private:
    PPage& page;

    OrderInfo& info;

    ObjectId* const tuples;

    uint64_t* const tuple_count;

    int64_t (*compare)(ObjectId, ObjectId);

    void swap(int x, int y);

    void quick_sort(int lo, int hi);

    void insertion_sort(int lo, int hi);

    int partition(int lo, int hi);
};

// MergeOrderedTupleIdCollection abstract the merge of two
// sorted runs (arrays of tuple collections)
class TupleCollectionMerger {
public:
    TupleCollectionMerger(OrderInfo& info, int64_t (*compare)(ObjectId, ObjectId));

    void merge(
        uint64_t left_start,
        uint64_t left_end,
        uint64_t right_start,
        uint64_t right_end,
        TmpFileId source_file_id,
        TmpFileId output_file_id
    );

    void copy_page(uint64_t source_page, TmpFileId source_file_id, TmpFileId output_file_id);

private:
    OrderInfo& info;

    int64_t (*compare)(ObjectId, ObjectId);

    std::unique_ptr<TupleCollectionPage> get_run(PPage& run_page);
};
