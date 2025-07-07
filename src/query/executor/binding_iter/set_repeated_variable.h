#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "query/executor/binding_iter.h"

class SetRepeatedVariable : public BindingIter {
public:
    SetRepeatedVariable(std::unique_ptr<BindingIter>&& child_iter, VarId start_var) :
        child_iter(std::move(child_iter)),
        start_var(start_var)
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> child_iter;
    VarId start_var;
    bool vars_are_good;
    bool written = false;

private:
    Binding* parent_binding;
};
