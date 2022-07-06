#pragma once

#include "query_optimizer/quad_model/plan/plan.h"

class EdgePlan : public Plan {
public:
    EdgePlan(Id from, Id to, Id type, Id edge);

    EdgePlan(const EdgePlan& other) :
        from          (other.from),
        to            (other.to),
        type          (other.type),
        edge          (other.edge),
        from_assigned (other.from_assigned),
        to_assigned   (other.to_assigned),
        type_assigned (other.type_assigned),
        edge_assigned (other.edge_assigned) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<EdgePlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo* thread_info) const override;

    bool get_leapfrog_iter(ThreadInfo*                                 thread_info,
                           std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                           std::vector<VarId>&                         var_order,
                           uint_fast32_t&                              enumeration_level) const override;

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    Id from;
    Id to;
    Id type;
    Id edge;

    bool from_assigned;
    bool to_assigned;
    bool type_assigned;
    bool edge_assigned;
};
