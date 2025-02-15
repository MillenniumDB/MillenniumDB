#include "path_plan.h"

#include "graph_models/quad_model/quad_model.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_trails/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_enum.h"
#include "query/executor/binding_iter/paths/all_trails/bfs_check.h"
#include "query/executor/binding_iter/paths/all_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_trails/dfs_check.h"
#include "query/executor/binding_iter/paths/all_trails/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_trails/bfs_check.h"
#include "query/executor/binding_iter/paths/any_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_trails/dfs_check.h"
#include "query/executor/binding_iter/paths/any_trails/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_walks/bfs_check.h"
#include "query/executor/binding_iter/paths/any_walks/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_walks/dfs_check.h"
#include "query/executor/binding_iter/paths/any_walks/dfs_enum.h"
#include "query/executor/binding_iter/paths/experimental/all_shortest_walks_count/bfs_check.h"
#include "query/executor/binding_iter/paths/experimental/all_shortest_walks_count/bfs_enum.h"
#include "query/executor/binding_iter/paths/index_provider/quad_model_index_provider.h"
#include "system/path_manager.h"
#include "query/executor/binding_iter/paths/unfixed_composite.h"
#include "query/executor/binding_iter/paths/any_walks/bfs_multiple_starts.h"
#include "query/executor/binding_iter/paths/any_walks/debug_mati.h"

using namespace MQL;
using namespace std;

PathPlan::PathPlan(
    std::vector<bool>& begin_at_left,
    OpPath::Direction direction,
    VarId path_var,
    Id from,
    Id to,
    RegularPathExpr& path,
    PathSemantic path_semantic
) :
    begin_at_left (begin_at_left),
    direction     (direction),
    path_var      (path_var),
    from          (from),
    to            (to),
    path          (path),
    from_assigned (from.is_OID()),
    to_assigned   (to.is_OID()),
    path_semantic (path_semantic)
{
    automaton = path.get_rpq_automaton(&QuadObjectId::get_named_node);
    auto inverted_path = path.clone()->invert();
    automaton_inverted = inverted_path->get_rpq_automaton(&QuadObjectId::get_named_node);
}


double PathPlan::estimate_cost() const {
    // TODO: find a better estimation
    if (!to_assigned && !from_assigned) {
        return std::numeric_limits<double>::max();
    }
    return /*100.0 +*/ estimate_output_size();
}


void PathPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "PathPlan(";
    os << "from: " << from;
    os << ", to: " << to;
    os << ", path: " << path_var << ": " << path.to_string();
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double PathPlan::estimate_output_size() const {
    // TODO: find a better estimation
    const auto total_connections = static_cast<double>(quad_model.catalog.edge_count);
    return total_connections * total_connections;
}


std::set<VarId> PathPlan::get_vars() const {
    std::set<VarId> result;
    if (from.is_var() && !from_assigned) {
        result.insert(from.get_var());
    }
    if (to.is_var() && !to_assigned) {
        result.insert(to.get_var());
    }

    return result;
}


void PathPlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
}


unique_ptr<Paths::IndexProvider> PathPlan::get_provider(const RPQ_DFA& automaton) const {
    auto t_info = unordered_map<uint64_t, Paths::IndexType>();
    auto t_inv_info = unordered_map<uint64_t, Paths::IndexType>();
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
    return make_unique<Paths::QuadModelIndexProvider>(std::move(t_info),
                                                      std::move(t_inv_info),
                                                      &get_query_ctx().thread_info.interruption_requested);
}


std::unique_ptr<BindingIter> PathPlan::get_check(const RPQ_DFA& automaton, Id start, Id end) const {
    auto provider = get_provider(automaton);
    _debug_mati() << "path_semantic: " << Paths::get_semantic_str(path_semantic) << std::endl;
    switch (path_semantic) {
    case PathSemantic::ALL_ACYCLIC: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AllSimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AllSimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_TRAILS: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AllTrails::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_ACYCLIC: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AnySimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AnySimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_TRAILS: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AnyTrails::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_WALKS: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::Any::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return make_unique<Paths::AllShortestSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return make_unique<Paths::AllShortestSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_TRAILS:
        return make_unique<Paths::AllShortestTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        return make_unique<Paths::AllShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_TRAILS:
        return make_unique<Paths::AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
    case PathSemantic::DEFAULT:
        return make_unique<Paths::Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));

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
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AllSimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AllSimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_TRAILS: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AllTrails::DFSEnum>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_ACYCLIC: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AnySimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AnySimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_TRAILS: {
        if (quad_model.path_mode == PathMode::DFS) {
            return make_unique<Paths::AnyTrails::DFSEnum>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_WALKS: {
        if (quad_model.path_mode == PathMode::DFS) {
            if (automaton.total_final_states > 1) {
                return make_unique<Paths::Any::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return make_unique<Paths::Any::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        } else {
            if (automaton.total_final_states > 1) {
                return make_unique<Paths::Any::BFSMultipleStarts<true>>(path_var, std::vector<Id>({start}), end, automaton, std::move(provider));
            } else {
                return make_unique<Paths::Any::BFSMultipleStarts<false>>(path_var, std::vector<Id>({start}), end, automaton, std::move(provider));
            }
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return make_unique<Paths::AllShortestSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return make_unique<Paths::AllShortestSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_TRAILS:
        return make_unique<Paths::AllShortestTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        if (automaton.total_final_states > 1) {
            return make_unique<Paths::AllShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AllShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return make_unique<Paths::AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return make_unique<Paths::AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_TRAILS:
        return make_unique<Paths::AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
        if (automaton.total_final_states > 1) {
            return make_unique<Paths::Any::BFSMultipleStarts<true>>(path_var, std::vector<Id>({start}), end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::Any::BFSMultipleStarts<false>>(path_var, std::vector<Id>({start}), end, automaton, std::move(provider));
        }
    case PathSemantic::DEFAULT: {
        std::vector<Id> start_nodes;
        if (automaton.total_final_states > 1) {
            return make_unique<Paths::Any::BFSMultipleStarts<true>>(path_var, start_nodes, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::Any::BFSMultipleStarts<false>>(path_var, start_nodes, end, automaton, std::move(provider));
        }
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


bool PathPlan::from_is_better_start_direction() const {
    if (automaton.total_states == 1 || automaton.is_final_state[0]) {
        return true;
    }

    double cost_normal_dir = 0;
    double cost_inverse_dir = 0;

    for (auto& transition : automaton.from_to_connections[0]) {
        auto predicate_id = transition.type_id.id;
        auto it = quad_model.catalog.type2total_count.find(predicate_id);
        if (it != quad_model.catalog.type2total_count.end()) {
            cost_normal_dir += it->second;
        }
    }

    for (auto& transition : automaton_inverted.from_to_connections[0]) {
        auto predicate_id = transition.type_id.id;
        auto it = quad_model.catalog.type2total_count.find(predicate_id);
        if (it != quad_model.catalog.type2total_count.end()) {
            cost_inverse_dir += it->second;
        }
    }

    if (cost_inverse_dir < cost_normal_dir) {
        return false;
    } else {
        return true;
    }
}

unique_ptr<BindingIter> PathPlan::get_binding_iter() const {
    bool right_to_left = direction == OpPath::Direction::RIGHT_TO_LEFT;
    if (from_assigned) {
        if (to_assigned) {
            auto star_at_from = from_is_better_start_direction();
            begin_at_left[path_var.id] = star_at_from != right_to_left;
            const RPQ_DFA& best_automaton = star_at_from ? automaton : automaton_inverted;
            Id start = star_at_from ? from : to;
            Id end   = star_at_from ? to   : from;
            return get_check(best_automaton, start, end);
        } else {
            begin_at_left[path_var.id] = !right_to_left;
            return get_enum(automaton, from, to.get_var());
        }
    } else {
        if (to_assigned) {
            // enum starting on to
            begin_at_left[path_var.id] = right_to_left;
            return get_enum(automaton_inverted, to, from.get_var());
        } else {
            auto star_at_from = from_is_better_start_direction();
            begin_at_left[path_var.id] =  star_at_from != right_to_left;
            const RPQ_DFA& best_automaton = star_at_from ? automaton : automaton_inverted;
            Id start = star_at_from ? from : to;
            Id end   = star_at_from ? to   : from;
            return get_unfixed(best_automaton, start.get_var(), end.get_var());
        }
    }
}
