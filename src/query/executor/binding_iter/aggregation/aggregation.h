#pragma once

#include <map>
#include <memory>
#include <set>
#include <utility>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/aggregation/agg.h"

class Aggregation : public BindingIter {
public:
    Aggregation(
        std::unique_ptr<BindingIter> child,
        std::map<VarId, std::unique_ptr<Agg>> aggregations,
        std::set<VarId> group_vars
    ) :
        child(std::move(child)),
        aggregations(std::move(aggregations)),
        group_vars(std::move(group_vars))
    { }

    ~Aggregation() = default;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void print(std::ostream& os, int indent, bool stats) const override;

    std::unique_ptr<BindingIter> child;

    const std::map<VarId, std::unique_ptr<Agg>> aggregations;

    // may be empty if when the query uses aggregates without any group
    const std::set<VarId> group_vars;
    Binding group_vars_binding;

    Binding* parent_binding;
    Binding child_binding;

    bool new_group;

    uint32_t groups = 0;
};
