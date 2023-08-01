#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class SubSelect : public BindingIter {
public:
    SubSelect(
        std::unique_ptr<BindingIter> child_iter,
        std::vector<VarId>             projection_vars,
        std::vector<VarId>             safe_assigned_vars
    ) :
        projection_vars    (projection_vars),
        safe_assigned_vars (safe_assigned_vars),
        child_iter         (std::move(child_iter)) { }

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    Binding*  parent_binding;

    std::vector<VarId> projection_vars;
    std::vector<VarId> safe_assigned_vars;

    std::unique_ptr<BindingIter> child_iter;
    std::unique_ptr<Binding>     child_binding;

    uint64_t count      = 0;
    uint64_t executions = 0;
};
