#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class Union : public BindingIter {
public:
    Union(
        std::vector<std::unique_ptr<BindingIter>>&& iters
    ) :
        iters         (std::move(iters)) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::vector<std::unique_ptr<BindingIter>> iters;
    uint_fast32_t current_iter = 0;

private:
    Binding* parent_binding;
};
