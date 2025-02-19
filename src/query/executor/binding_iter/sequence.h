#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "query/executor/binding_iter.h"

class Sequence : public BindingIter {
public:
    Sequence(std::vector<std::unique_ptr<BindingIter>>&& iters) :
        iters(std::move(iters))
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    void set_start_variable();
    void store_boundary_variable();

    std::vector<std::unique_ptr<BindingIter>> iters;
    uint_fast32_t current_iter = 0;

    std::stack<ObjectId> end_vars_stack;

private:
    Binding* parent_binding;
};
