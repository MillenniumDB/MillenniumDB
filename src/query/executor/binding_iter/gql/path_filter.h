#pragma once

#include <memory>
#include <vector>

#include "path_binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/parser/expr/gql/expr_property.h"

namespace GQL {

// Filters the child's paths by a WHERE inside a quantifier (per iteration) or a union branch
class PathFilter : public PathBindingIter {
public:
    PathFilter(
        std::unique_ptr<PathBindingIter> child_iter,
        std::vector<std::unique_ptr<BindingExpr>>&& filters,
        const std::vector<ExprProperty>& properties
    );

    void begin(Binding& binding) override;
    void reset() override;
    void set_left_boundary(ObjectId boundary) override;
    void set_right_boundary(ObjectId boundary) override;
    std::unique_ptr<PathBinding> next() override;
    void assign_nulls() override;
    void assign_empty() override;

    void print(std::ostream& os, int indent, bool stats) override;

private:
    std::unique_ptr<PathBindingIter> child_iter;
    std::vector<std::unique_ptr<BindingExpr>> filters;

    // fetch the properties used by the filters (x.key) for the current values of the variables
    std::vector<IndexScan<3>> prop_scans;

    Binding* parent_binding;

    uint64_t results_checked = 0;
    uint64_t results_passed = 0;
};

} // namespace GQL
