#pragma once

#include "query/executor/binding_iter.h"

class ObjectEnum : public BindingIter {
public:
    ObjectEnum(VarId var, uint64_t mask, uint64_t max_count) :
        var(var),
        mask(mask),
        current_node(0),
        max_count(max_count)
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    const VarId var;

private:
    const uint64_t mask;
    uint64_t current_node;
    Binding* parent_binding;

public:
    const uint64_t max_count;
};
