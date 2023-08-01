#pragma once

#include "query/query_context.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/op/mql/graph_pattern/op_path.h"
#include "query/parser/paths/regular_path_expr.h"
#include "query/optimizer/plan/plan.h"

class PathPlan : public Plan {
public:
    PathPlan(VarId path_var, Id from, Id to, RegularPathExpr& path, PathSemantic semantic);

    PathPlan(const PathPlan& other) :
        path_var      (other.path_var),
        from          (other.from),
        to            (other.to),
        path          (other.path),
        from_assigned (other.from_assigned),
        to_assigned   (other.to_assigned),
        path_semantic (other.path_semantic) { }

    std::unique_ptr<Plan> clone() const override {
        return std::make_unique<PathPlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>&,
                           std::vector<VarId>&,
                           uint_fast32_t&) const override { return false; }

    void print(std::ostream& os, int indent) const override;

private:
    const VarId path_var;
    const Id from;
    const Id to;
    RegularPathExpr& path;

    bool from_assigned;
    bool to_assigned;

    PathSemantic path_semantic;

    // Construct index provider for this automaton
    std::unique_ptr<Paths::IndexProvider> get_provider(RPQ_DFA automaton) const;

    std::unique_ptr<BindingIter> get_check(RPQ_DFA automaton, Id start, Id end) const;
    std::unique_ptr<BindingIter> get_enum(RPQ_DFA automaton, Id start, VarId end) const;
    std::unique_ptr<BindingIter> get_unfixed(RPQ_DFA automaton, VarId start, VarId end) const;
};
