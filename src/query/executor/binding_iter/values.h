#pragma once

#include <memory>
#include <vector>
#include <set>

#include "query/executor/binding_iter.h"

class Values : public BindingIter {
public:
    Values(
        std::vector<std::pair<VarId, bool>>&& vars,
        std::vector<ObjectId>&&               values
    ) :
        vars          (std::move(vars)),
        values        (std::move(values)) { }

    ~Values() = default;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void accept_visitor(BindingIterVisitor& visitor) override;

    // true means value is fixed by parent
    std::vector<std::pair<VarId, bool>> vars;
    std::vector<ObjectId> values;

private:
    uint64_t current = 0;

    Binding* parent_binding;
};
