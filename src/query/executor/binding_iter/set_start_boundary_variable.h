#pragma once

#include "query/executor/binding_iter.h"

using namespace GQL;

class SetStartBoundaryVariable : public BindingIter {
public:
    SetStartBoundaryVariable(std::unique_ptr<BindingIter>&& iter, VarId start_var) :
        child_iter(std::move(iter)),
        start_var_to_set(start_var)
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> child_iter;

    VarId start_var_to_set;

private:
    Binding* parent_binding;
};
