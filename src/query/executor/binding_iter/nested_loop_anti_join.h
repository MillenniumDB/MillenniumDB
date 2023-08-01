#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"

// match_without_common_variables:
//  false: lhs binding will only be deleted if lhs and rhs are compatible and have at least one variable in common, used for OpMinus
//  true:  lhs binding will be deleted if lhs and rhs are compatible even without variables in common, used for OpNotExists
template<bool match_without_common_variables>
class NestedLoopAntiJoin : public BindingIter {
public:
    NestedLoopAntiJoin(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>             safe_join_vars,
        std::vector<VarId>             unsafe_join_vars,
        std::vector<VarId>             parent_safe_vars,
        std::vector<VarId>             lhs_only_vars,
        std::vector<VarId>             rhs_only_vars
    ) :
        lhs              (std::move(lhs)),
        rhs              (std::move(rhs)),
        safe_join_vars   (std::move(safe_join_vars)),
        unsafe_join_vars (std::move(unsafe_join_vars)),
        parent_safe_vars (std::move(parent_safe_vars)),
        lhs_only_vars    (std::move(lhs_only_vars)),
        rhs_only_vars    (std::move(rhs_only_vars)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

    Binding* parent_binding;
    std::unique_ptr<Binding> lhs_binding;
    std::unique_ptr<Binding> rhs_binding;

    std::vector<VarId> safe_join_vars;
    std::vector<VarId> unsafe_join_vars;
    std::vector<VarId> parent_safe_vars;
    std::vector<VarId> lhs_only_vars;
    std::vector<VarId> rhs_only_vars;

    uint64_t result_count = 0;
    uint64_t executions = 0;
};
