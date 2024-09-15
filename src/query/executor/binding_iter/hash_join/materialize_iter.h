#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "storage/index/hash/key_value_hash/key_value_pair_hasher.h"
#include "storage/page/private_page.h"

namespace HashJoin {
/*
MaterializeIter class is used to store outputs tuples in disk by hybrid
algorithms when build relation is too big to store all in memory.

A relations is partitioned in a specific numbers of partitions, each partitions
is stored in disk an handled by MaterializeIter.
*/
class MaterializeIter : public BindingIter {
public:
    // MAX_DEPTH on split to avoid infinite split
    static const uint64_t MAX_DEPTH = 64;

    MaterializeIter(
        const std::vector<VarId>& vars,
        uint64_t depth);

    ~MaterializeIter();

    // Takes two relations, and partition each one storing a pair
    // of corresponding partitions in partitions vector
    static void make_partitions(
        BindingIter* lhs,
        BindingIter* rhs,
        const uint64_t depth,
        const size_t max_hash_table_size,
        Binding& lhs_binding,
        Binding& rhs_binding,
        const std::vector<VarId>& lhs_vars,
        const std::vector<VarId>& rhs_vars,
        const std::vector<VarId>& join_vars,
        std::vector<std::pair<std::unique_ptr<MaterializeIter>, std::unique_ptr<MaterializeIter>>>& partitions
    );

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // begin must be called before add
    void add(); // Materialize current parent_binding to disk

    uint64_t total_tuples = 0;
    uint64_t depth;

    // When a partition stored partition is too big, it may be partitioned in
    // two new partitions with split method.
    std::unique_ptr<MaterializeIter> split(const std::vector<VarId>& join_vars);

private:
    // Materialized vars
    const std::vector<VarId>& vars;

    Binding* parent_binding;

    // File where relation will be placed
    TmpFileId tmp_file;

    // Pointer to page that will be read or written
    PPage* current_page;

    ObjectId* start_page_pointer;

    // Current page id that will be read or written
    uint64_t current_page_number;

    // Current index of page where next tuple will be read or written
    uint64_t current_page_index;

    uint64_t tuples_returned;

    const uint64_t max_tuples_per_page;
};
}
