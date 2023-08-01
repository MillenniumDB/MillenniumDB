#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>
#include <utility>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/aggregation/agg.h"

class Aggregation : public BindingIter {
public:
    Aggregation(
        std::unique_ptr<BindingIter>          child_iter,
        std::map<VarId, std::unique_ptr<Agg>> aggregations,
        std::vector<VarId>                    group_vars
    ) :
        child         (std::move(child_iter)),
        aggregations  (std::move(aggregations)),
        group_vars    (std::move(group_vars)) { }

    ~Aggregation() = default;

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

    std::unique_ptr<BindingIter> child;
private:

    const std::map<VarId, std::unique_ptr<Agg>> aggregations;

    // may be empty if when the query uses aggregates without any group
    const std::vector<VarId> group_vars;
    Binding group_vars_binding;

    Binding* parent_binding;
    Binding child_binding;

    bool new_group;
    bool has_returned = false;
};
