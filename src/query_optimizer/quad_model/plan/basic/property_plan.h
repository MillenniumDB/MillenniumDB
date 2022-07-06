#pragma once

#include "query_optimizer/quad_model/plan/plan.h"

class PropertyPlan : public Plan {
public:
    PropertyPlan(Id object, Id key, Id value);

    PropertyPlan(const PropertyPlan& other) :
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

    bool get_leapfrog_iter(ThreadInfo*                                 thread_info,
                           std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                           std::vector<VarId>&                         var_order,
                           uint_fast32_t&                              enumeration_level) const override;

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    Id object;
    Id key;
    Id value;

    bool object_assigned;
    bool key_assigned;
    bool value_assigned;
};
