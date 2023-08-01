#include "path_plan.h"

#include "query/exceptions.h"
#include "query/executor/binding_iter/paths/all_shortest/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_count/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_count/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_enum.h"
// #include "query/executor/binding_iter/paths/all_shortest_trails/bfs_check.h"
// #include "query/executor/binding_iter/paths/all_shortest_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_enum.h"
// #include "query/executor/binding_iter/paths/all_trails/bfs_check.h"
// #include "query/executor/binding_iter/paths/all_trails/bfs_enum.h"
// #include "query/executor/binding_iter/paths/all_trails/dfs_check.h"
// #include "query/executor/binding_iter/paths/all_trails/dfs_enum.h"
#include "query/executor/binding_iter/paths/any/dfs_check.h"
#include "query/executor/binding_iter/paths/any/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_shortest/bfs_check.h"
#include "query/executor/binding_iter/paths/any_shortest/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_enum.h"
// #include "query/executor/binding_iter/paths/any_trails/bfs_check.h"
// #include "query/executor/binding_iter/paths/any_trails/bfs_enum.h"
// #include "query/executor/binding_iter/paths/any_trails/dfs_check.h"
// #include "query/executor/binding_iter/paths/any_trails/dfs_enum.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "query/executor/binding_iter/paths/index_provider/rdf_model_index_provider.h"
#include "query/executor/binding_iter/paths/unfixed_composite.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "query/parser/paths/regular_path_expr.h"

using namespace SPARQL;

std::function<ObjectId(const std::string&)> str_to_object_id_f = [](const std::string& str) {
    return RDFObjectId::get_Iri(str);
};

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
    automaton = path.get_rpq_automaton(str_to_object_id_f);
    auto inverted_path = path.clone()->invert();
    automaton_inverted = inverted_path->get_rpq_automaton(str_to_object_id_f);
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
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double PathPlan::estimate_output_size() const {
    double res = 0;
    // TODO: find a better estimation
    // cost is the sum of all properties multiplied with a factor
    // we chose 1.1 so a simple path (eg: :P1*) is more expensive than a simple triple
    for (auto& state_transitions : automaton.from_to_connections) {
        for (auto& transition : state_transitions) {
            auto predicate_id = transition.type_id.id;
            res += rdf_model.catalog().predicate2total_count[predicate_id] * 1.1;
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
    set_input_var(input_vars, subject, &subject_assigned);
    set_input_var(input_vars, object,  &object_assigned);
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
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    // case PathSemantic::ALL_TRAILS: {
    //     if (rdf_model.path_mode == PathMode::DFS) {
    //         return std::make_unique<Paths::AllTrails::DFSCheck>(path_var, start, end, automaton, std::move(provider));
    //     } else {
    //         return std::make_unique<Paths::AllTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    //     }
    // }
    case PathSemantic::ANY_ACYCLIC: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    // case PathSemantic::ANY_TRAILS: {
    //     if (rdf_model.path_mode == PathMode::DFS) {
    //         return std::make_unique<Paths::AnyTrails::DFSCheck>(path_var, start, end, automaton, std::move(provider));
    //     } else {
    //         return std::make_unique<Paths::AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    //     }
    // }
    case PathSemantic::ANY_WALKS: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::Any::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnyShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AllSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AllSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    // case PathSemantic::ALL_SHORTEST_TRAILS:
    //     return std::make_unique<Paths::AllTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        return std::make_unique<Paths::AllShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    // case PathSemantic::ANY_SHORTEST_TRAILS:
    //     return std::make_unique<Paths::AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
        return std::make_unique<Paths::AnyShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

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
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AllSimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AllSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    // case PathSemantic::ALL_TRAILS: {
    //     if (rdf_model.path_mode == PathMode::DFS) {
    //         return std::make_unique<Paths::AllTrails::DFSEnum>(path_var, start, end, automaton, std::move(provider));
    //     } else {
    //         return std::make_unique<Paths::AllTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    //     }
    // }
    case PathSemantic::ANY_ACYCLIC: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (rdf_model.path_mode == PathMode::DFS) {
            return std::make_unique<Paths::AnySimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    // case PathSemantic::ANY_TRAILS: {
    //     if (rdf_model.path_mode == PathMode::DFS) {
    //         return std::make_unique<Paths::AnyTrails::DFSEnum>(path_var, start, end, automaton, std::move(provider));
    //     } else {
    //         return std::make_unique<Paths::AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    //     }
    // }
    case PathSemantic::ANY_WALKS: {
        if (rdf_model.path_mode == PathMode::DFS) {
            if (automaton.total_final_states > 1) {
                return std::make_unique<Paths::Any::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return std::make_unique<Paths::Any::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        } else {
            if (automaton.total_final_states > 1) {
                return std::make_unique<Paths::AnyShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return std::make_unique<Paths::AnyShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return std::make_unique<Paths::AllSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return std::make_unique<Paths::AllSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    // case PathSemantic::ALL_SHORTEST_TRAILS:
    //     return std::make_unique<Paths::AllTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
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
    // case PathSemantic::ANY_SHORTEST_TRAILS:
    //     return std::make_unique<Paths::AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
        if (automaton.total_final_states > 1) {
            return std::make_unique<Paths::AnyShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return std::make_unique<Paths::AnyShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT enum not supported yet");

    default:
        throw QuerySemanticException("PathSemantic not recognized");
    }
}


std::unique_ptr<BindingIter> PathPlan::get_unfixed(const RPQ_DFA& automaton, VarId start, VarId end) const {
    auto provider = get_provider(automaton);
    auto enum_iter = get_enum(automaton, start, end);
    return std::make_unique<Paths::UnfixedComposite>(path_var, start, end, automaton, std::move(provider), std::move(enum_iter));
}


bool PathPlan::left_is_better_start_direction() const {
    // TODO: wrong if automaton.is_final_state[0], maybe we don't want to support that
    // in the way SPARQL defines it (returns something greater or equal than all distinct
    // subjects and objects)
    if (automaton.total_states == 1 || automaton.is_final_state[0]) {
        return true;
    }

    double cost_normal_dir = 0;
    double cost_inverse_dir = 0;

    for (auto& transition : automaton.from_to_connections[0]) {
        auto predicate_id = transition.type_id.id;
        cost_normal_dir += rdf_model.catalog().predicate2total_count[predicate_id];
    }

    for (auto& transition : automaton_inverted.from_to_connections[0]) {
        auto predicate_id = transition.type_id.id;
        cost_inverse_dir += rdf_model.catalog().predicate2total_count[predicate_id];
    }

    if (cost_inverse_dir < cost_normal_dir) {
        return false;
    } else {
        return true;
    }
}


std::unique_ptr<BindingIter> PathPlan::get_binding_iter() const {
    if (subject_assigned) {
        if (object_assigned) {
            auto star_at_left = left_is_better_start_direction();
            begin_at_left[path_var.id] = star_at_left;
            const RPQ_DFA& best_automaton = star_at_left ? automaton : automaton_inverted;
            return get_check(best_automaton, subject, object);
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
            auto star_at_left = left_is_better_start_direction();
            begin_at_left[path_var.id] = star_at_left;
            const RPQ_DFA& best_automaton = star_at_left ? automaton : automaton_inverted;
            return get_unfixed(best_automaton, subject.get_var(), object.get_var());
        }
    }
}
