#ifndef QUAD_MODEL__PROPERTY_PATH_PLAN_H_
#define QUAD_MODEL__PROPERTY_PATH_PLAN_H_

#include "relational_model/models/quad_model/query_optimizer/plan/plan.h"
#include "base/parser/logical_plan/op/property_paths/path_automaton.h"

class PropertyPathPlan : public Plan {
public:
    PropertyPathPlan(const QuadModel& model, VarId path_var, Id from, Id to, OpPath& path);
    ~PropertyPathPlan() = default;

    PropertyPathPlan(const PropertyPathPlan& other) :
        model         (other.model),
        path_var      (other.path_var),
        from          (other.from),
        to            (other.to),
        path          (other.path),
        from_assigned (other.from_assigned),
        to_assigned   (other.to_assigned) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<PropertyPathPlan>(*this);
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
    const VarId path_var;
    const Id from;
    const Id to;
    OpPath& path;

    bool from_assigned;
    bool to_assigned;

    // Set the transitions of the automaton received with the corresponding ObjectId.
    // The automaton has the transition (Type) as string and it needs to be transformed into an ObjectId.
    void set_automaton_transition_id(PathAutomaton& automaton) const;
};

#endif // QUAD_MODEL__PROPERTY_PATH_PLAN_H_
