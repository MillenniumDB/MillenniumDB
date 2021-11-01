#ifndef QUAD_MODEL__CONNECTION_PLAN_H_
#define QUAD_MODEL__CONNECTION_PLAN_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"

class ConnectionPlan : public Plan {
public:
    ConnectionPlan(const QuadModel& model, Id from, Id to, Id type, Id edge);
    ~ConnectionPlan() = default;

    ConnectionPlan(const ConnectionPlan& other) :
        model         (other.model),
        from          (other.from),
        to            (other.to),
        type          (other.type),
        edge          (other.edge),
        from_assigned (other.from_assigned),
        to_assigned   (other.to_assigned),
        type_assigned (other.type_assigned),
        edge_assigned (other.edge_assigned) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<ConnectionPlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo* thread_info) const override;

    std::unique_ptr<LeapfrogIter> get_leapfrog_iter(ThreadInfo*               thread_info,
                                                    const std::vector<VarId>& var_order,
                                                    uint_fast32_t             enumeration_level) const override;

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    const QuadModel& model;

    Id from;
    Id to;
    Id type;
    Id edge;

    bool from_assigned;
    bool to_assigned;
    bool type_assigned;
    bool edge_assigned;
};

#endif // QUAD_MODEL__CONNECTION_PLAN_H_
