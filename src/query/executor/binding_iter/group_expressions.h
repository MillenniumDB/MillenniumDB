#pragma once

#include <vector>
#include <memory>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class GroupExpressions : public BindingIter {
public:
    GroupExpressions(
        std::unique_ptr<BindingIter>                                  child_iter,
        std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>&& expressions
    ) :
        child_iter    (std::move(child_iter)),
        expressions   (std::move(expressions)) { }

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;

    // Has to be a vector or an ordered map, because the order of evaluation of the expressions matters.
    const std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> expressions;

    Binding* parent_binding;
};
