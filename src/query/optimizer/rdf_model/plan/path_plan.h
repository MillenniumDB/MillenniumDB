#pragma once

#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/op/sparql/op_path.h"
#include "query/parser/paths/regular_path_expr.h"
#include "query/optimizer/plan/plan.h"

namespace SPARQL {
class PathPlan : public Plan {
public:
    PathPlan(
        std::vector<bool>& begin_at_left,
        VarId path_var,
        Id subject,
        RegularPathExpr& path,
        Id object,
        PathSemantic path_semantic);

    PathPlan(const PathPlan& other) :
        begin_at_left      (other.begin_at_left),
        path_var           (other.path_var),
        subject            (other.subject),
        path               (other.path),
        object             (other.object),
        subject_assigned   (other.subject_assigned),
        object_assigned    (other.object_assigned),
        path_semantic      (other.path_semantic),
        automaton          (other.automaton),
        automaton_inverted (other.automaton_inverted) { }

    std::unique_ptr<Plan> clone() const override {
        return std::make_unique<PathPlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void            set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>&,
                           std::vector<VarId>&,
                           uint_fast32_t&) const override { return false; }

    void print(std::ostream& os, int indent) const override;

    // Construct index provider for this automaton
    std::unique_ptr<Paths::IndexProvider> get_provider(const RPQ_DFA& automaton) const;

    std::unique_ptr<BindingIter> get_check(const RPQ_DFA& automaton, Id start, Id end) const;
    std::unique_ptr<BindingIter> get_enum(const RPQ_DFA& automaton, Id start, VarId end) const;
    std::unique_ptr<BindingIter> get_unfixed(const RPQ_DFA& automaton, VarId start, VarId end) const;

private:
    std::vector<bool>& begin_at_left;
    VarId path_var;
    Id subject;
    RegularPathExpr& path;
    Id object;

    bool subject_assigned;
    bool object_assigned;

    PathSemantic path_semantic;

    RPQ_DFA automaton;
    RPQ_DFA automaton_inverted;

    // When we can start the path in either way, we want to choose the cheapest one
    // This method returns `automaton` or `automaton_inverted` depending on cardinality of
    // the outer transitions of the starting state
    bool left_is_better_start_direction() const;
};
} // namespace SPARQL
