#pragma once

#include <memory>
#include <set>
#include <vector>

#include "graph_models/object_id.h"
#include "query/executor/binding.h"
#include "query/var_id.h"
#include "storage/file_id.h"
#include "storage/page/private_page.h"

struct OrderedGroupInfo {
    // saved_vars must start with the order_vars
    std::vector<VarId> saved_vars;

    size_t order_size;

    OrderedGroupInfo(const std::set<VarId>& order_vars, std::set<VarId>&& _saved_vars)
    {
        // to avoid having duplicates in saved_vars
        std::set<VarId> inserted_vars;

        order_size = order_vars.size();

        for (auto& var : order_vars) {
            if (inserted_vars.insert(var).second) {
                saved_vars.push_back(var);
            }
        }

        for (auto& var : _saved_vars) {
            if (inserted_vars.insert(var).second) {
                saved_vars.push_back(var);
            }
        }
    }
};

class OrderedGroupPage {
    friend class OrderedGroupMerger;

public:
    OrderedGroupPage(PPage& page, OrderedGroupInfo& info);

    ~OrderedGroupPage();

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

    OrderedGroupInfo& info;

    ObjectId* const tuples;

    uint64_t* const tuple_count;

    void swap(int x, int y);

    void quick_sort(int lo, int hi);

    void insertion_sort(int lo, int hi);

    int partition(int lo, int hi);
};

// MergeOrderedTupleIdCollection abstract the merge of two
// sorted runs (arrays of tuple collections)
class OrderedGroupMerger {
public:
    OrderedGroupMerger(OrderedGroupInfo& info);

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
    OrderedGroupInfo& info;

    std::unique_ptr<OrderedGroupPage> get_run(PPage& run_page);
};

class OrderedGroup {
public:
    OrderedGroup(const std::set<VarId>& group_vars, std::set<VarId>&& order_saved_vars);

    ~OrderedGroup();

    // must call this after finishing the adds
    // does external merge sort
    void end_appends();

    void begin(Binding&);

    void add(Binding&);

    void reset();

    // sets the binding received at begin
    bool next();

private:
    std::unique_ptr<OrderedGroupPage> run;

    OrderedGroupInfo order_info;

    TmpFileId current_file_id;

    TmpFileId aux_file_id;

    uint_fast32_t total_pages = 0;

    uint_fast32_t current_page = 0;

    uint64_t page_position = 0;

    Binding* parent_binding;

    // buffer to add to runs
    std::vector<ObjectId> object_ids;

    std::unique_ptr<OrderedGroupPage> get_run(PPage& run_page);

    void merge_sort();
};
