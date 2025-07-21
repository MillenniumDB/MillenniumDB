#pragma once

#include "query/executor/binding_iter.h"

using namespace GQL;

class SetVariableValues : public BindingIter {
public:
    SetVariableValues(
        std::unique_ptr<BindingIter> child,
        std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> items
    ) :
        items(std::move(items)),
        child(std::move(child))
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> items;

private:
    Binding* parent_binding;
    std::unique_ptr<BindingIter> child;
};
