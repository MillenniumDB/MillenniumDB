#pragma once

#include <vector>
#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class ExprEvaluator : public BindingIter {
public:
    ExprEvaluator(
        std::unique_ptr<BindingIter>                                  child_iter,
        std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>&& exprs
    ) :
        child_iter  (std::move(child_iter)),
        exprs (std::move(exprs)) { }

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void accept_visitor(BindingIterVisitor& visitor) override;

    std::unique_ptr<BindingIter> child_iter;

    // We use vector because the order of evaluation of the expressions matters.
    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> exprs;

private:
    Binding* parent_binding;
};
