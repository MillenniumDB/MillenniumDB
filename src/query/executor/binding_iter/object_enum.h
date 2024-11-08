#pragma once

#include "query/executor/binding_iter.h"

class ObjectEnum : public BindingIter {
public:
    ObjectEnum(VarId var, const uint64_t mask, const uint64_t max_count) :
        var(var),
        max_count(max_count),
        mask(mask) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    const VarId var;
    const uint64_t max_count;

private:
    const uint64_t mask;

    uint64_t current_node = 0;

    Binding* parent_binding;
};
