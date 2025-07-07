#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"

class Minus : public BindingIter {
public:
    Minus(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId> safe_join_vars,
        std::vector<VarId> unsafe_join_vars,
        std::vector<VarId> parent_safe_vars
    ) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs)),
        safe_join_vars(std::move(safe_join_vars)),
        unsafe_join_vars(std::move(unsafe_join_vars)),
        parent_safe_vars(std::move(parent_safe_vars))
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

    std::vector<VarId> safe_join_vars;
    std::vector<VarId> unsafe_join_vars;
    std::vector<VarId> parent_safe_vars;

private:
    Binding* parent_binding;
    std::unique_ptr<Binding> rhs_binding;
};
