#include "path_plan.h"

#include "base/exceptions.h"
#include "execution/binding_id_iter/paths/all_shortest/bfs_check.h"
#include "execution/binding_id_iter/paths/all_shortest/bfs_enum.h"
#include "execution/binding_id_iter/paths/any/dfs_enum.h"
#include "execution/binding_id_iter/paths/any_shortest/iter/a_star_iter_enum.h"
#include "execution/binding_id_iter/paths/any_shortest/iter/bfs_iter_enum.h"
#include "execution/binding_id_iter/paths/any_shortest/simple/bfs_check.h"
#include "execution/binding_id_iter/paths/any_shortest/simple/bfs_simple_enum.h"
#include "execution/binding_id_iter/paths/any_shortest/simple/unfixed_composite.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"

using namespace std;

PathPlan::PathPlan(VarId path_var, Id from, Id to, IPath& path, PathSemantic path_semantic) :
    path_var      (path_var),
    from          (from),
    to            (to),
    path          (path),
    from_assigned (std::holds_alternative<ObjectId>(from)),
    to_assigned   (std::holds_alternative<ObjectId>(to)),
    path_semantic (path_semantic) { }


double PathPlan::estimate_cost() const {
    // TODO: find a better estimation
    if (!to_assigned && !from_assigned) {
        return std::numeric_limits<double>::max();
    }
    return /*100.0 +*/ estimate_output_size();
}


void PathPlan::print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "PathPlan(";
    if (std::holds_alternative<ObjectId>(from)) {
        os << "from: " << quad_model.get_graph_object(std::get<ObjectId>(from));
    } else {
        os << "from: " << var_names[std::get<VarId>(from).id];
    }
    if (std::holds_alternative<ObjectId>(to)) {
        os << ", to: " << quad_model.get_graph_object(std::get<ObjectId>(to));
    } else {
        os << ", to: " << var_names[std::get<VarId>(to).id];
    }
    os << ", path: " << var_names[path_var.id] << ": " << path.to_string();
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
    if (std::holds_alternative<VarId>(from) && !from_assigned) {
        result.insert(std::get<VarId>(from));
    }
    if (std::holds_alternative<VarId>(to) && !to_assigned) {
        result.insert(std::get<VarId>(to));
    }

    return result;
}


void PathPlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to, &to_assigned);
}


unique_ptr<BindingIdIter> PathPlan::get_binding_id_iter(ThreadInfo* thread_info) const {
    if (path_semantic == PathSemantic::ANY) {
        if (from_assigned) {
            auto automaton = path.get_transformed_automaton();
            set_automaton_transition_id(automaton);
            if (to_assigned) {
                // bool case
                return make_unique<Paths::AnyShortest::BFSCheck>(thread_info,
                                                                 *quad_model.nodes,
                                                                 *quad_model.type_from_to_edge,
                                                                 *quad_model.to_type_from_edge,
                                                                 path_var,
                                                                 from,
                                                                 to,
                                                                 automaton);
            } else {
                // enum starting on from
                return make_unique<Paths::AnyShortest::BFSIterEnum>(thread_info,
                                                                    *quad_model.nodes,
                                                                    *quad_model.type_from_to_edge,
                                                                    *quad_model.to_type_from_edge,
                                                                    path_var,
                                                                    from,
                                                                    std::get<VarId>(to),
                                                                    automaton);
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton     = inverted_path->get_transformed_automaton();
                set_automaton_transition_id(automaton);
                return make_unique<Paths::AnyShortest::BFSIterEnum>(thread_info,
                                                                    *quad_model.nodes,
                                                                    *quad_model.type_from_to_edge,
                                                                    *quad_model.to_type_from_edge,
                                                                    path_var,
                                                                    to,
                                                                    std::get<VarId>(from),
                                                                    automaton);
            } else {
                if (path.nullable()) {
                    throw QuerySemanticException("Nullable property paths must have at least 1 node fixed");
                }
                auto automaton = path.get_transformed_automaton();
                set_automaton_transition_id(automaton);
                return make_unique<Paths::AnyShortest::UnfixedComposite>(thread_info,
                                                                         *quad_model.nodes,
                                                                         *quad_model.type_from_to_edge,
                                                                         *quad_model.to_type_from_edge,
                                                                         path_var,
                                                                         std::get<VarId>(to),
                                                                         std::get<VarId>(from),
                                                                         automaton);
            }
        }
    } else {
        // ALL SHORTEST
        if (from_assigned) {
            auto automaton = path.get_transformed_automaton();
            set_automaton_transition_id(automaton);
            if (to_assigned) {
                // bool case
                return make_unique<Paths::AllShortest::BFSCheck>(thread_info,
                                                                 *quad_model.nodes,
                                                                 *quad_model.type_from_to_edge,
                                                                 *quad_model.to_type_from_edge,
                                                                 path_var,
                                                                 from,
                                                                 to,
                                                                 automaton);
            } else {
                // enum starting on from
                return make_unique<Paths::AllShortest::BFSEnum>(thread_info,
                                                                *quad_model.nodes,
                                                                *quad_model.type_from_to_edge,
                                                                *quad_model.to_type_from_edge,
                                                                path_var,
                                                                from,
                                                                std::get<VarId>(to),
                                                                automaton);
            }
        } else {
            if (to_assigned) {
                // enum starting on to
                auto inverted_path = path.invert();
                auto automaton     = inverted_path->get_transformed_automaton();
                set_automaton_transition_id(automaton);
                return make_unique<Paths::AllShortest::BFSEnum>(thread_info,
                                                                *quad_model.nodes,
                                                                *quad_model.type_from_to_edge,
                                                                *quad_model.to_type_from_edge,
                                                                path_var,
                                                                to,
                                                                std::get<VarId>(from),
                                                                automaton);
            } else {
                // TODO: allow no-nullable unfixed paths
                throw QuerySemanticException("property paths must have at least 1 node fixed.");
            }
        }
    }
    return nullptr;
}


void PathPlan::set_automaton_transition_id(PathAutomaton& automaton) const {
    // For each Transition instance in from_to vector, creates a TransitionId
    // instance that have an object id object of string label. It will be stored
    // in transition attribute of automaton
    for (size_t i = 0; i < automaton.from_to_connections.size(); i++) {
        vector<TransitionId> transition_id_vector;
        for (const auto& t : automaton.from_to_connections[i]) {
            transition_id_vector.push_back(
              TransitionId(t.to, quad_model.get_object_id(GraphObject::make_named_node(t.type)), t.inverse));
        }
        automaton.transitions.push_back(transition_id_vector);
    }
}
