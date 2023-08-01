#include "path_plan.h"

#include "graph_models/quad_model/quad_model.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/paths/all_shortest/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_count/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_count/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_shortest_trails/bfs_check.h"
#include "query/executor/binding_iter/paths/all_shortest_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/all_simple/dfs_enum.h"
#include "query/executor/binding_iter/paths/all_trails/bfs_check.h"
#include "query/executor/binding_iter/paths/all_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/all_trails/dfs_check.h"
#include "query/executor/binding_iter/paths/all_trails/dfs_enum.h"
#include "query/executor/binding_iter/paths/any/dfs_check.h"
#include "query/executor/binding_iter/paths/any/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_shortest/bfs_check.h"
#include "query/executor/binding_iter/paths/any_shortest/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_check.h"
#include "query/executor/binding_iter/paths/any_simple/dfs_enum.h"
#include "query/executor/binding_iter/paths/any_trails/bfs_check.h"
#include "query/executor/binding_iter/paths/any_trails/bfs_enum.h"
#include "query/executor/binding_iter/paths/any_trails/dfs_check.h"
#include "query/executor/binding_iter/paths/any_trails/dfs_enum.h"
#include "query/executor/binding_iter/paths/index_provider/quad_model_index_provider.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/binding_iter/paths/unfixed_composite.h"

using namespace std;

PathPlan::PathPlan(
    VarId path_var,
    Id from,
    Id to,
    RegularPathExpr& path,
    PathSemantic path_semantic
) :
    path_var      (path_var),
    from          (from),
    to            (to),
    path          (path),
    from_assigned (from.is_OID()),
    to_assigned   (to.is_OID()),
    path_semantic (path_semantic) { }


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
    const auto total_connections = static_cast<double>(quad_model.catalog().connections_count);
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


unique_ptr<Paths::IndexProvider> PathPlan::get_provider(RPQ_DFA automaton) const {
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


std::unique_ptr<BindingIter> PathPlan::get_check(RPQ_DFA automaton, Id start, Id end) const {
    auto provider = get_provider(automaton);

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
            return make_unique<Paths::AnyShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return make_unique<Paths::AllSimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return make_unique<Paths::AllSimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_TRAILS:
        return make_unique<Paths::AllTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_WALKS:
        return make_unique<Paths::AllShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ANY_SHORTEST_ACYCLIC:
        return make_unique<Paths::AnySimple::BFSCheck<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_SIMPLE:
        return make_unique<Paths::AnySimple::BFSCheck<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_TRAILS:
        return make_unique<Paths::AnyTrails::BFSCheck>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ANY_SHORTEST_WALKS:
        return make_unique<Paths::AnyShortest::BFSCheck>(path_var, start, end, automaton, std::move(provider));

    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT check not supported yet");
    default:
        throw QuerySemanticException("PathSemantic not recognized");
    }
}


std::unique_ptr<BindingIter> PathPlan::get_enum(RPQ_DFA automaton, Id start, VarId end) const {
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
                return make_unique<Paths::AnyShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
            } else {
                return make_unique<Paths::AnyShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
            }
        }
    }

    // Shortest only support BFS
    case PathSemantic::ALL_SHORTEST_ACYCLIC:
        return make_unique<Paths::AllSimple::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_SIMPLE:
        return make_unique<Paths::AllSimple::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
    case PathSemantic::ALL_SHORTEST_TRAILS:
        return make_unique<Paths::AllTrails::BFSEnum>(path_var, start, end, automaton, std::move(provider));
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
            return make_unique<Paths::AnyShortest::BFSEnum<true>>(path_var, start, end, automaton, std::move(provider));
        } else {
            return make_unique<Paths::AnyShortest::BFSEnum<false>>(path_var, start, end, automaton, std::move(provider));
        }
    case PathSemantic::ALL_SHORTEST_WALKS_COUNT:
        throw QuerySemanticException("ALL_SHORTEST_WALKS_COUNT enum not supported yet");
    default:
        throw QuerySemanticException("PathSemantic not recognized");
    }
}


std::unique_ptr<BindingIter> PathPlan::get_unfixed(RPQ_DFA /*automaton*/, VarId /*start*/, VarId /*end*/) const {
    throw QuerySemanticException("UNFIXED paths not supported yet");
}


unique_ptr<BindingIter> PathPlan::get_binding_iter() const {
    std::function<ObjectId(const std::string&)> str_to_object_id_f = [](const std::string& str) {
        return QuadObjectId::get_named_node(str);
    };

    if (from_assigned) {
        auto automaton = path.get_rpq_automaton(str_to_object_id_f);
        if (to_assigned) {
            return get_check(automaton, from, to);
        } else {
            return get_enum(automaton, from, to.get_var());
        }
    } else {
        if (to_assigned) {
            // enum starting on to
            auto inverted_path = path.invert();
            auto automaton = inverted_path->get_rpq_automaton(str_to_object_id_f);
            return get_enum(automaton, to, from.get_var());
        } else {
            auto automaton = path.get_rpq_automaton(str_to_object_id_f);
            return get_unfixed(automaton, from.get_var(), to.get_var());
        }
    }
}
