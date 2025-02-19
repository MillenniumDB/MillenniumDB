#pragma once

#include "query/executor/binding_iter.h"

using namespace GQL;

class SetEndBoundaryVariable : public BindingIter {
public:
    SetEndBoundaryVariable(std::unique_ptr<BindingIter>&& iter, VarId end_var) :
        child_iter(std::move(iter)),
        end_var_to_set(end_var)
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> child_iter;

    VarId end_var_to_set;

private:
    Binding* parent_binding;
};
