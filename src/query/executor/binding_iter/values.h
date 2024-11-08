#pragma once

#include <vector>

#include "query/executor/binding_iter.h"

class Values : public BindingIter {
public:
    Values(std::vector<std::pair<VarId, bool>>&& vars, std::vector<ObjectId>&& values) :
        vars(std::move(vars)),
        values(std::move(values))
    { }

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void accept_visitor(BindingIterVisitor& visitor) override;

    // true means value is fixed by parent
    std::vector<std::pair<VarId, bool>> vars;

    std::vector<ObjectId> values;

    uint64_t current;

    Binding* parent_binding;
};
