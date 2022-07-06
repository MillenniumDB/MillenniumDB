#pragma once

#include "query_optimizer/quad_model/plan/plan.h"

class IndexNestedLoopPlan : public Plan {
public:
    IndexNestedLoopPlan(std::unique_ptr<Plan> lhs, std::unique_ptr<Plan> rhs);

    IndexNestedLoopPlan(const IndexNestedLoopPlan& other) :
        lhs                   (other.lhs->duplicate()),
        rhs                   (other.rhs->duplicate()),
        estimated_cost        (other.estimated_cost),
        estimated_output_size (other.estimated_output_size)  { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<IndexNestedLoopPlan>(*this);
    }

    double estimate_cost()        const override { return estimated_cost; }
    double estimate_output_size() const override { return estimated_output_size; }

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo*) const override;

    bool get_leapfrog_iter(ThreadInfo*,
                           std::vector<std::unique_ptr<LeapfrogIter>>&,
                           std::vector<VarId>&,
                           uint_fast32_t&) const override { return false; }

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    std::unique_ptr<Plan> lhs;
    std::unique_ptr<Plan> rhs;

    double estimated_cost;
    double estimated_output_size;
};
