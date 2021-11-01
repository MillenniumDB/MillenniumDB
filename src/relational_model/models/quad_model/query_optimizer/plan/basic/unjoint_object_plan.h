#ifndef QUAD_MODEL__UNJOINT_OBJECT_PLAN_H_
#define QUAD_MODEL__UNJOINT_OBJECT_PLAN_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class UnjointObjectPlan : public Plan {
public:
    UnjointObjectPlan(const QuadModel& model, const VarId object_var_id);
    ~UnjointObjectPlan() = default;

    UnjointObjectPlan(const UnjointObjectPlan& other) :
        model         (other.model),
        object_var_id (other.object_var_id) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<UnjointObjectPlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo*) const override;

    std::unique_ptr<LeapfrogIter> get_leapfrog_iter(ThreadInfo*               /*thread_info*/,
                                                    const std::vector<VarId>& /*var_order*/,
                                                    uint_fast32_t             /*enumeration_level*/) const override
                                                    { return nullptr; }

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    const QuadModel& model;
    const VarId object_var_id;
};

#endif // QUAD_MODEL__UNJOINT_OBJECT_PLAN_H_
