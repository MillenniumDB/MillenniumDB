#pragma once

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class SetVariableValues : public BindingIter {
public:
    SetVariableValues(std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> items) :
        items(std::move(items))
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> items;

private:
    bool returned = false;
    Binding* parent_binding;
};
