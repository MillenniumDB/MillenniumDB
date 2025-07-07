#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"

class NotExists : public BindingIter {
public:
    NotExists(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId> unsafe_join_vars
    ) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs)),
        unsafe_join_vars(std::move(unsafe_join_vars))
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

    std::vector<VarId> unsafe_join_vars;

private:
    Binding* parent_binding;
    std::unique_ptr<Binding> rhs_binding;
};
