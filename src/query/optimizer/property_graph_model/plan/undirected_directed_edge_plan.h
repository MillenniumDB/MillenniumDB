#pragma once

#include "query/optimizer/plan/plan.h"
#include "query/query_context.h"

namespace GQL {

class UndirectedDirectedEdgePlan : public Plan {
public:
    UndirectedDirectedEdgePlan(Id edge, Id from, Id to, VarId direction_var, ObjectId direction);

    UndirectedDirectedEdgePlan(const UndirectedDirectedEdgePlan& other) :
        edge(other.edge),
        from(other.from),
        to(other.to),
        direction_var(other.direction_var),
        edge_assigned(other.edge_assigned),
        from_assigned(other.from_assigned),
        to_assigned(other.to_assigned),
        direction(other.direction)
    { }

    std::unique_ptr<Plan> clone() const override
    {
        return std::make_unique<UndirectedDirectedEdgePlan>(*this);
    }

    int relation_size() const override
    {
        return 3;
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    std::unique_ptr<BindingIter> add_direction_iter(std::unique_ptr<BindingIter> iter) const;

    bool get_leapfrog_iter(
        std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
        std::vector<VarId>& var_order,
        uint_fast32_t& enumeration_level
    ) const override;

    void print(std::ostream& os, int indent) const override;

private:
    Id edge;
    Id from;
    Id to;
    VarId direction_var;

    bool edge_assigned;
    bool from_assigned;
    bool to_assigned;

    ObjectId direction;
};
} // namespace GQL
