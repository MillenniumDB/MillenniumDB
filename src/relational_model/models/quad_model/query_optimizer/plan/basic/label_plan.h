#ifndef RELATIONAL_MODEL__LABEL_PLAN_H_
#define RELATIONAL_MODEL__LABEL_PLAN_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class LabelPlan : public Plan {
public:
    LabelPlan(const QuadModel& model, Id node, Id label);
    ~LabelPlan() = default;

    LabelPlan(const LabelPlan& other) :
        model          (other.model),
        node           (other.node),
        label          (other.label),
        node_assigned  (other.node_assigned),
        label_assigned (other.label_assigned) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<LabelPlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo*) const override;

    std::unique_ptr<LeapfrogIter> get_leapfrog_iter(ThreadInfo*               thread_info,
                                                    const std::vector<VarId>& var_order,
                                                    uint_fast32_t             enumeration_level) const override;


    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    const QuadModel& model;

    Id node;
    Id label;

    bool node_assigned;
    bool label_assigned;
};

#endif // RELATIONAL_MODEL__LABEL_PLAN_H_
