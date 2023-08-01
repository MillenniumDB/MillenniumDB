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

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    // true means value is fixed by parent
    std::vector<std::pair<VarId, bool>> vars;
    std::vector<ObjectId> values;

    uint64_t current = 0;

    Binding* parent_binding;

    uint64_t result_count = 0;
    uint64_t executions = 0;
};
