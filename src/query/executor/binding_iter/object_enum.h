#pragma once

#include "query/executor/binding_iter.h"

class ObjectEnum : public BindingIter {
public:
    ObjectEnum(
        VarId         var,
        const         uint64_t mask,
        const         uint64_t max_count
    ) :
        var           (var),
        mask          (mask),
        max_count     (max_count) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    const VarId var;
    const uint64_t mask;
    const uint64_t max_count;
    uint64_t current_node = 0;
    uint64_t results = 0;

    Binding* parent_binding;
};
