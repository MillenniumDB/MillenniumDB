#pragma once

#include "query/optimizer/plan/plan.h"

namespace GQL {

class NodePlan : public Plan {
public:
    NodePlan(const VarId node_id) :
        node_id(node_id)
    { }

    std::unique_ptr<Plan> clone() const override
    {
        return std::make_unique<NodePlan>(*this);
    }

    int relation_size() const override
    {
        return 1;
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>&, std::vector<VarId>&, uint_fast32_t&)
        const override
    {
        return false;
    }

    void print(std::ostream& os, int indent) const override;

private:
    const VarId node_id;
    bool node_assigned = false;
};
} // namespace GQL
