#include "path_plan.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/empty_binding_iter.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_walks/bfs_check.h"
#include "query/executor/binding_iter/paths/any_walks/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_walks/dfs_check.h"
#include "query/executor/binding_iter/paths/any_walks/dfs_enum.h"
#include "query/executor/binding_iter/paths/experimental/all_shortest_walks_count/bfs_check.h"
#include "query/executor/binding_iter/paths/experimental/all_shortest_walks_count/bfs_enum.h"
#include "query/executor/binding_iter/paths/index_provider/rdf_model_index_provider.h"
#include "query/executor/binding_iter/paths/unfixed_composite.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "query/parser/paths/regular_path_expr.h"

using namespace SPARQL;

PathPlan::PathPlan(
    std::vector<bool>& begin_at_left,
    VarId path_var,
    Id subject,
    RegularPathExpr& path,
    Id object,
    PathSemantic path_semantic
) :
    begin_at_left    (begin_at_left),
    path_var         (path_var),
    subject          (subject),
    path             (path),
    object           (object),
    subject_assigned (subject.is_OID()),
    object_assigned  (object.is_OID()),
    path_semantic    (path_semantic)
{
    automaton = path.get_rpq_automaton(&SPARQL::Conversions::pack_iri);
    auto inverted_path = path.clone()->invert();
    automaton_inverted = inverted_path->get_rpq_automaton(&SPARQL::Conversions::pack_iri);
}


double PathPlan::estimate_cost() const {
    // TODO: find a better estimation
    if (!subject_assigned && !object_assigned) {
        return std::numeric_limits<double>::max();
    }
    return /*100.0 +*/ estimate_output_size();
}


void PathPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "PathPlan(subject: " << subject;
    os << ", object: " << object;
    os << ", path: " << get_query_ctx().get_var_name(path_var) << ": " << path.to_string();
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size() << '\n';
}


double PathPlan::estimate_output_size() const {
    if (!cached_output_estimation_is_valid) {
        cached_output_estimation = _estimate_output_size();
        cached_output_estimation_is_valid = true;
    }

    return cached_output_estimation;
}


double PathPlan::_estimate_output_size() const {
    double res = 0;
    // cost is the sum of all properties multiplied with a factor
    // we chose 1.1 so a simple path (eg: :P1*) is more expensive than a simple triple
    Record<3> min;
    Record<3> max;
    if (subject.is_OID()) {
        if (object.is_OID()) {
            // TODO: mandatory check? cost doesn't matter
            return 1.0;
        } else {
            for (auto& transition : automaton.from_to_connections[0]) {
                auto predicate_id = transition.type_id.id;

                // SPO
                min[0] = subject.get_OID().id;
                min[1] = predicate_id;
                min[2] = 0;

                max[0] = subject.get_OID().id;
                max[1] = predicate_id;
                max[2] = UINT64_MAX;

                res += 1.1 * rdf_model.spo->estimate_records(min, max);
            }
        }
    } else if (object.is_OID()) {
        for (auto& transition : automaton_inverted.from_to_connections[0]) {
            auto predicate_id = transition.type_id.id;

            // POS
            min[0] = predicate_id;
            min[1] = object.get_OID().id;
            min[2] = 0;

            max[0] = predicate_id;
            max[1] = object.get_OID().id;
            max[2] = UINT64_MAX;

            res += 1.1 * rdf_model.pos->estimate_records(min, max);
        }
    } else {
        for (auto& state_transitions : automaton.from_to_connections) {
            for (auto& transition : state_transitions) {
                auto predicate_id = transition.type_id.id;
                res += rdf_model.catalog.get_predicate_count(predicate_id) * 1.1;
            }
        }
    }
    return res;
}


std::set<VarId> PathPlan::get_vars() const {
    std::set<VarId> result;
    if (subject.is_var() && !subject_assigned) {
        result.insert(subject.get_var());
    }
    if (object.is_var() && !object_assigned) {
        result.insert(object.get_var());
    }

    return result;
}


void PathPlan::set_input_vars(const std::set<VarId>& input_vars) {
    auto previous_assigned_count = static_cast<int>(subject_assigned)
                                 + static_cast<int>(object_assigned);

    set_input_var(input_vars, subject, &subject_assigned);
    set_input_var(input_vars, object,  &object_assigned);

    auto after_assigned_count = static_cast<int>(subject_assigned)
                              + static_cast<int>(object_assigned);

    if (previous_assigned_count != after_assigned_count) {
        cached_output_estimation_is_valid = false;
    }
}


std::unique_ptr<Paths::IndexProvider> PathPlan::get_provider(const RPQ_DFA& automaton) const {
    auto t_info = std::unordered_map<uint64_t, Paths::IndexType>();
    auto t_inv_info = std::unordered_map<uint64_t, Paths::IndexType>();
    for (size_t state = 0; state < automaton.from_to_connections.size(); state++) {
        for (auto& transition : automaton.from_to_connections[state]) {
            if (transition.inverse) {
                // Avoid transitions that are already stored
                if (t_inv_info.find(transition.type_id.id) != t_inv_info.end()) {
                    continue;
                }
                t_inv_info.insert({transition.type_id.id, Paths::IndexType::BTREE});

            } else {
                // Avoid transitions that are already stored
                if (t_info.find(transition.type_id.id) != t_info.end()) {
                    continue;
                }
                t_info.insert({transition.type_id.id, Paths::IndexType::BTREE});
            }
        }
    }
    return std::make_unique<Paths::RdfModelIndexProvider>(std::move(t_info),
                                                          std::move(t_inv_info),
                                                          &get_query_ctx().thread_info.interruption_requested);
}


std::unique_ptr<BindingIter> PathPlan::get_check(const RPQ_DFA& automaton, Id start, Id end) const {
    auto provider = get_provider(automaton);

    switch (path_semantic) {
    case PathSemantic::ALL_ACYCLIC: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_ACYCLIC: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_WALKS: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::Any::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AllShortestSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AllShortestSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        return std::make_unique<Paths::AllShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
    case PathSemantic::DEFAULT:
        return std::make_unique<Paths::Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT check not supported yet");
    default:
        throw QuerySemanticException("PathSemantic not recognized");
    }
}


std::unique_ptr<BindingIter> PathPlan::get_enum(const RPQ_DFA& automaton, Id start, VarId end) const {
    auto provider = get_provider(automaton);

    switch (path_semantic) {
    case PathSemantic::ALL_ACYCLIC: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_ACYCLIC: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_WALKS: {
        if (rdf_model.path_mode == PathSearchMode::DFS) {
            if (automaton.total_final_states > 1) {
                return std::make_unique<Paths::Any::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return std::make_unique<Paths::Any::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        } else {
            if (automaton.total_final_states > 1) {
                return std::make_unique<Paths::Any::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return std::make_unique<Paths::Any::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AllShortestSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AllShortestSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        if (automaton.total_final_states > 1) {
            return std::make_unique<Paths::AllShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
    case PathSemantic::DEFAULT:
        if (automaton.total_final_states > 1) {
            return std::make_unique<Paths::Any::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::Any::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT enum not supported yet");

    default:
        throw QuerySemanticException("PathSemantic not recognized");
    }
}


std::unique_ptr<BindingIter> PathPlan::get_unfixed(const RPQ_DFA& automaton, VarId start, VarId end) const {
    auto iter = start == end
              ? get_check(automaton, start, end)
              : get_enum(automaton, start, end);

    return std::make_unique<Paths::UnfixedComposite>(
        path_var,
        start,
        end,
        automaton,
        get_provider(automaton),
        std::move(iter)
    );
}


bool PathPlan::subject_is_better_start_direction() const {
    if (automaton.total_states == 1 || automaton.is_final_state[0]) {
        return true;
    }

    double cost_normal_dir = 0;
    double cost_inverse_dir = 0;

    for (auto& transition : automaton.from_to_connections[0]) {
        auto predicate_id = transition.type_id.id;
        cost_normal_dir += rdf_model.catalog.get_predicate_count(predicate_id);
    }

    for (auto& transition : automaton_inverted.from_to_connections[0]) {
        auto predicate_id = transition.type_id.id;
        cost_inverse_dir += rdf_model.catalog.get_predicate_count(predicate_id);
    }

    if (cost_inverse_dir < cost_normal_dir) {
        return false;
    } else {
        return true;
    }
}


std::unique_ptr<BindingIter> PathPlan::get_binding_iter() const {
    // border case with nullable paths and empty database, example at test
    // tests/sparql/test_suites/sparql11/property-path/values_and_path.rq
    if (rdf_model.catalog.get_triples_count() == 0) {
        return std::make_unique<EmptyBindingIter>();
    }
    if (subject_assigned) {
        if (object_assigned) {
            auto star_at_subject = subject_is_better_start_direction();
            begin_at_left[path_var.id] = star_at_subject;
            const RPQ_DFA& best_automaton = star_at_subject ? automaton : automaton_inverted;
            Id start = star_at_subject ? subject : object;
            Id end   = star_at_subject ? object  : subject;

            return get_check(best_automaton, start, end);
        } else {
            begin_at_left[path_var.id] = true;
            return get_enum(automaton, subject, object.get_var());
        }
    } else {
        if (object_assigned) {
            // enum starting on object
            begin_at_left[path_var.id] = false;
            return get_enum(automaton_inverted, object, subject.get_var());
        } else {
            auto star_at_subject = subject_is_better_start_direction();
            begin_at_left[path_var.id] = star_at_subject;
            const RPQ_DFA& best_automaton = star_at_subject ? automaton : automaton_inverted;
            Id start = star_at_subject ? subject : object;
            Id end   = star_at_subject ? object  : subject;
            return get_unfixed(best_automaton, start.get_var(), end.get_var());
        }
    }
}
