#pragma once

#include <memory>

#include "query/executor/binding_iter.h"

class CrossProduct : public BindingIter {
public:
    CrossProduct(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs
    ) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

private:
    BindingIter* rhs_iter;

    Binding* parent_binding;
};