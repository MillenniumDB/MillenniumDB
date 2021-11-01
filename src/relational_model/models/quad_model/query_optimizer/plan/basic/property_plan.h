#ifndef QUAD_MODEL__PROPERTY_PLAN_H_
#define QUAD_MODEL__PROPERTY_PLAN_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class PropertyPlan : public Plan {
public:
    PropertyPlan(const QuadModel& model, Id object, Id key, Id value);
    ~PropertyPlan() = default;

    PropertyPlan(const PropertyPlan& other) :
        model           (other.model),
        object          (other.object),
        key             (other.key),
        value           (other.value),
        object_assigned (other.object_assigned),
        key_assigned    (other.key_assigned),
        value_assigned  (other.value_assigned) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<PropertyPlan>(*this);
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

    Id object;
    Id key;
    Id value;

    bool object_assigned;
    bool key_assigned;
    bool value_assigned;
};

#endif // QUAD_MODEL__PROPERTY_PLAN_H_
