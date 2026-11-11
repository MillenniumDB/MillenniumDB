#pragma once

#include <memory>
#include <vector>

#include "path_binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/parser/expr/gql/expr_property.h"

namespace GQL {

// Evaluates a WHERE clause of a quantified path pattern on every iteration, before the iteration is
// concatenated with the others, i.e., while the variables of the iteration still hold single elements.
// The variables of the child pattern are bound in the parent binding when the child returns a path.
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
