#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/empty_binding_iter.h"

class IndexNestedLoopJoin : public BindingIter {
public:
    IndexNestedLoopJoin(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs
    ) :
        lhs           (std::move(lhs)),
        original_rhs  (std::move(rhs)) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> original_rhs;

private:
    BindingIter* rhs; // will point to original_rhs or a EmptyBindingIter

    Binding* parent_binding;

    EmptyBindingIter empty_iter;
};
