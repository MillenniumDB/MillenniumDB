#ifndef QUAD_MODEL__INDEX_NESTED_LOOP_PLAN_H_
#define QUAD_MODEL__INDEX_NESTED_LOOP_PLAN_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class IndexNestedLoopPlan : public Plan {
public:
    IndexNestedLoopPlan(std::unique_ptr<Plan> lhs, std::unique_ptr<Plan> rhs);
    ~IndexNestedLoopPlan() = default;

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

    std::unique_ptr<LeapfrogIter> get_leapfrog_iter(ThreadInfo*               /*thread_info*/,
                                                    const std::vector<VarId>& /*var_order*/,
                                                    uint_fast32_t             /*enumeration_level*/) const override
                                                    { return nullptr; }

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    std::unique_ptr<Plan> lhs;
    std::unique_ptr<Plan> rhs;

    double estimated_cost;
    double estimated_output_size;
};

#endif // QUAD_MODEL__INDEX_NESTED_LOOP_PLAN_H_
