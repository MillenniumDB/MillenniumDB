#pragma once

#include <memory>

#include "query/executor/binding_iter.h"

class Slice : public BindingIter {
public:
    Slice(
        std::unique_ptr<BindingIter> child_iter,
        uint64_t                     offset,
        uint64_t                     limit
    ) :
        child_iter (std::move(child_iter)),
        offset     (offset),
        limit      (limit) { }

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void accept_visitor(BindingIterVisitor& visitor) override;

    std::unique_ptr<BindingIter> child_iter;
    uint64_t offset;
    uint64_t limit;

private:
    Binding* parent_binding;

    uint64_t count = 0;
    uint64_t position = 0;
};
