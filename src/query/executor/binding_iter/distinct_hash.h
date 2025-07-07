#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class DistinctHash : public BindingIter {
public:
    DistinctHash(std::unique_ptr<BindingIter> child_iter, std::vector<VarId>&& _projected_vars) :
        child_iter(std::move(child_iter)),
        projected_vars(std::move(_projected_vars)),
        extendable_table(projected_vars.size())
    { }

    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;
    void assign_nulls() override;

    void print(std::ostream& os, int indent, bool stats) const override;

    bool current_tuple_distinct();

    std::unique_ptr<BindingIter> child_iter;

private:
    std::vector<VarId> projected_vars;
    DistinctBindingHash extendable_table;

    std::vector<ObjectId> current_tuple;
    Binding* parent_binding;
};
