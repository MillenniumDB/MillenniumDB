#pragma once

#include "query/executor/binding_iter.h"

class ObjectEnum : public BindingIter {
public:
    ObjectEnum(VarId var, uint64_t mask, uint64_t max_count, uint64_t default_start = 0) :
        var(var),
        mask(mask),
        default_start(default_start),
        current_node(default_start),
        max_count(max_count + default_start)
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    const VarId var;

private:
    const uint64_t mask;
    const uint64_t default_start;
    uint64_t current_node;
    Binding* parent_binding;

public:
    const uint64_t max_count;
};
