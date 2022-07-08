#pragma once

#include "parser/query/op/graph_pattern/op_path.h"
#include "parser/query/paths/path.h"
#include "query_optimizer/quad_model/plan/plan.h"

class PathPlan : public Plan {
public:
    PathPlan(VarId path_var, Id from, Id to, IPath& path, PathSemantic semantic);

    PathPlan(const PathPlan& other) :
        path_var      (other.path_var),
        from          (other.from),
        to            (other.to),
        path          (other.path),
        from_assigned (other.from_assigned),
        to_assigned   (other.to_assigned),
        path_semantic (other.path_semantic) { }

    std::unique_ptr<Plan> duplicate() const override {
        return std::make_unique<PathPlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIdIter> get_binding_id_iter(ThreadInfo*) const override;

    bool get_leapfrog_iter(ThreadInfo*,
                           std::vector<std::unique_ptr<LeapfrogIter>>&,
                           std::vector<VarId>&,
                           uint_fast32_t&) const override { return false; }

    void print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const override;

private:
    const VarId path_var;
    const Id from;
    const Id to;
    IPath& path;

    bool from_assigned;
    bool to_assigned;

    PathSemantic path_semantic;
};
