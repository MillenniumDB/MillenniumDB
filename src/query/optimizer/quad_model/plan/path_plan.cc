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
#include "query/executor/binding_iter/paths/index_provider/quad_model_index_provider.h"
#include "query/executor/binding_iter/paths/shortest_k/simple/bfs_check.h"
#include "query/executor/binding_iter/paths/shortest_k/simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/shortest_k/trails/bfs_check.h"
#include "query/executor/binding_iter/paths/shortest_k/trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/shortest_k/walks/bfs_check.h"
#include "query/executor/binding_iter/paths/shortest_k/walks/bfs_enum.h"
#include "query/executor/binding_iter/paths/shortest_k_groups/simple/bfs_check.h"
#include "query/executor/binding_iter/paths/shortest_k_groups/simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/shortest_k_groups/trails/bfs_check.h"
#include "query/executor/binding_iter/paths/shortest_k_groups/trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/shortest_k_groups/walks/bfs_check.h"
#include "query/executor/binding_iter/paths/shortest_k_groups/walks/bfs_enum.h"
#include "query/executor/binding_iter/paths/unfixed_composite.h"
// #include "query/executor/binding_iter/paths/experimental/all_shortest_walks_count/bfs_check.h"
// #include "query/executor/binding_iter/paths/experimental/all_shortest_walks_count/bfs_enum.h"

using namespace MQL;
using namespace std;
using namespace Paths;

PathPlan::PathPlan(
    std::vector<bool>& begin_at_left,
    OpPath::Direction direction,
    VarId path_var,
    Id from,
    Id to,
    RegularPathExpr& path,
    PathSemantic path_semantic,
    uint64_t K
) :
    begin_at_left (begin_at_left),
    direction     (direction),
    path_var      (path_var),
    from          (from),
    to            (to),
    path          (path),
    K             (K),
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


unique_ptr<IndexProvider> PathPlan::get_provider(const RPQ_DFA& automaton) const {
    auto t_info = unordered_map<uint64_t, IndexType>();
    auto t_inv_info = unordered_map<uint64_t, IndexType>();
    for (size_t state = 0; state < automaton.from_to_connections.size(); state++) {
        for (auto& transition : automaton.from_to_connections[state]) {
            if (transition.inverse) {
                // Avoid transitions that are already stored
                if (t_inv_info.find(transition.type_id.id) != t_inv_info.end()) {
                    continue;
                }
                t_inv_info.insert({transition.type_id.id, IndexType::BTREE});

            } else {
                // Avoid transitions that are already stored
                if (t_info.find(transition.type_id.id) != t_info.end()) {
                    continue;
                }
                t_info.insert({transition.type_id.id, IndexType::BTREE});
            }
        }
    }
    return make_unique<QuadModelIndexProvider>(std::move(t_info),
                                                      std::move(t_inv_info),
                                                      &get_query_ctx().thread_info.interruption_requested);
}


std::unique_ptr<BindingIter> PathPlan::get_check(const RPQ_DFA& automaton, Id start, Id end) const {
    auto provider = get_provider(automaton);

    switch (path_semantic) {
    case PathSemantic::ALL_ACYCLIC: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AllSimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AllSimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_TRAILS: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AllTrails::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_ACYCLIC: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AnySimple::DFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AnySimple::DFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_TRAILS: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AnyTrails::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_WALKS: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<Any::DFSCheck>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return make_unique<AllShortestSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return make_unique<AllShortestSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_TRAILS:
        return make_unique<AllShortestTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        return make_unique<AllShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return make_unique<AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return make_unique<AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_TRAILS:
        return make_unique<AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
    case PathSemantic::DEFAULT:
        return make_unique<Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::SHORTEST_K_ACYCLIC:
        if (K > 1)
            return make_unique<ShortestKSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_SIMPLE:
        if (K > 1)
            return make_unique<ShortestKSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_TRAILS:
        if (K > 1)
            return make_unique<ShortestKTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_WALKS:
        if (K > 1)
            return make_unique<ShortestKWalks::BFSCheck>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<Any::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::SHORTEST_K_GROUPS_ACYCLIC:
        if (K > 1)
            return make_unique<ShortestKGroupsSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortestSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_GROUPS_SIMPLE:
        if (K > 1)
            return make_unique<ShortestKGroupsSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortestSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_GROUPS_TRAILS:
        if (K > 1)
            return make_unique<ShortestKGroupsTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortestTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_GROUPS_WALKS:
        if (K > 1)
            return make_unique<ShortestKGroupsWalks::BFSCheck>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT check not supported yet");

    }
    throw QuerySemanticException("PathSemantic not recognized");
}


std::unique_ptr<BindingIter> PathPlan::get_enum(const RPQ_DFA& automaton, Id start, VarId end) const {
    auto provider = get_provider(automaton);

    switch (path_semantic) {
    case PathSemantic::ALL_ACYCLIC: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AllSimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_SIMPLE: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AllSimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ALL_TRAILS: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AllTrails::DFSEnum>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_ACYCLIC: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AnySimple::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_SIMPLE: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AnySimple::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_TRAILS: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            return make_unique<AnyTrails::DFSEnum>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
        }
    }
    case PathSemantic::ANY_WALKS: {
        if (quad_model.path_mode == PathSearchMode::DFS) {
            if (automaton.total_final_states > 1) {
                return make_unique<Any::DFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return make_unique<Any::DFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        } else {
            if (automaton.total_final_states > 1) {
                return make_unique<Any::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return make_unique<Any::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return make_unique<AllShortestSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return make_unique<AllShortestSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_TRAILS:
        return make_unique<AllShortestTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        if (automaton.total_final_states > 1) {
            return make_unique<AllShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<AllShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return make_unique<AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return make_unique<AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_TRAILS:
        return make_unique<AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
    case PathSemantic::DEFAULT:
        if (automaton.total_final_states > 1) {
            return make_unique<Any::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Any::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }

    case PathSemantic::SHORTEST_K_ACYCLIC:
        if (K > 1)
            return make_unique<ShortestKSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AnySimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_SIMPLE:
        if (K > 1)
            return make_unique<ShortestKSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AnySimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_TRAILS:
        if (K > 1)
            return make_unique<ShortestKTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AnyTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_WALKS:
        if (K > 1)
            return make_unique<ShortestKWalks::BFSEnum>(path_var, start, end, automaton, std::move(provider), K);
        else
            if (automaton.total_final_states > 1) {
                return make_unique<Any::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return make_unique<Any::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }

    case PathSemantic::SHORTEST_K_GROUPS_ACYCLIC:
        if (K > 1)
            return make_unique<ShortestKGroupsSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortestSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_GROUPS_SIMPLE:
        if (K > 1)
            return make_unique<ShortestKGroupsSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortestSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_GROUPS_TRAILS:
        if (K > 1)
            return make_unique<ShortestKGroupsTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider), K);
        else
            return make_unique<AllShortestTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::SHORTEST_K_GROUPS_WALKS: {
        if (K > 1)
            return make_unique<ShortestKGroupsWalks::BFSEnum>(path_var, start, end, automaton, std::move(provider), K);
        else
            if (automaton.total_final_states > 1) {
                return make_unique<AllShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return make_unique<AllShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }

    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT check not supported yet");
    }
    }
    throw QuerySemanticException("PathSemantic not recognized");
}


std::unique_ptr<BindingIter> PathPlan::get_unfixed(const RPQ_DFA& automaton, VarId start, VarId end) const {
    auto iter = start == end
              ? get_check(automaton, start, end)
              : get_enum(automaton, start, end);

    return std::make_unique<UnfixedComposite>(
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
