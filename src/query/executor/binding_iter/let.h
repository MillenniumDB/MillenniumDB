#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class Let : public BindingIter {
public:
    using VarBindingExprType = std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>;

    VarBindingExprType var_binding_expr;

    Let(VarBindingExprType&& var_binding_expr_) :
        var_binding_expr { std::move(var_binding_expr_) }
    { }

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void print(std::ostream& os, int indent, bool stats) const override;

private:
    Binding* parent_binding;

    bool evaluated { false };
};
