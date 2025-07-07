#pragma once

#include "query/executor/binding_iter.h"

using namespace GQL;

class EdgeDirectionRight : public BindingIter {
public:
    EdgeDirectionRight(std::unique_ptr<BindingIter>&& iter, VarId direction_var) :
        child_iter(std::move(iter)),
        direction_var(direction_var)
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> child_iter;
    VarId direction_var;

private:
    Binding* parent_binding;
};
