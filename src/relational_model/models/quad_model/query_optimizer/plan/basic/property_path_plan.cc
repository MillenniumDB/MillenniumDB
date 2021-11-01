#include "property_path_plan.h"

#include "base/parser/logical_plan/op/op_path.h"
#include "relational_model/execution/binding_id_iter/property_paths/path_manager.h"

#include "relational_model/execution/binding_id_iter/property_paths/simple/property_path_bfs_check.h"
#include "relational_model/execution/binding_id_iter/property_paths/simple/property_path_bfs_simple_enum.h"
#include "relational_model/execution/binding_id_iter/property_paths/iter/property_path_bfs_iter_enum.h"
#include "relational_model/execution/binding_id_iter/property_paths/iter/property_path_dfs_iter_enum.h"
#include "relational_model/execution/binding_id_iter/property_paths/iter/property_path_a_star_iter_enum.h"

using namespace std;

using PropertyPathCheck = PropertyPathBFSCheck;
using PropertyPathEnum = PropertyPathBFSIterEnum;

PropertyPathPlan::PropertyPathPlan(const QuadModel &model, VarId path_var, Id from, Id to, OpPath &path) :
    model         (model),
    path_var      (path_var),
    from          (from),
    to            (to),
    path          (path),
    from_assigned (std::holds_alternative<ObjectId>(from)),
    to_assigned   (std::holds_alternative<ObjectId>(to)) { }


double PropertyPathPlan::estimate_cost() const {
    // TODO: find a better estimation
    if (!to_assigned && !from_assigned) {
        return std::numeric_limits<double>::max();
    }
    return /*100.0 +*/ estimate_output_size();
}


void PropertyPathPlan::print(std::ostream& os, int indent, const std::vector<std::string>& var_names) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "PropertyPathPlan(";
    if (std::holds_alternative<ObjectId>(from)) {
        os << "from: " << model.get_graph_object(std::get<ObjectId>(from));
    } else {
        os << "from: " << var_names[std::get<VarId>(from).id];
    }
    if (std::holds_alternative<ObjectId>(to)) {
        os << ", to: " << model.get_graph_object(std::get<ObjectId>(to));
    } else {
        os << ", to: " << var_names[std::get<VarId>(to).id];
    }
    os << ", path: " <<  var_names[path_var.id] << ": " << path.to_string();
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double PropertyPathPlan::estimate_output_size() const {
    // TODO: find a better estimation
    const auto total_connections = static_cast<double>(model.catalog().connections_count);
    return total_connections * total_connections;
}


std::set<VarId> PropertyPathPlan::get_vars() const {
    std::set<VarId> result;
    if ( std::holds_alternative<VarId>(from) && !from_assigned) {
        result.insert( std::get<VarId>(from) );
    }
    if ( std::holds_alternative<VarId>(to) && !to_assigned) {
        result.insert( std::get<VarId>(to) );
    }

    return result;
}


void PropertyPathPlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, from, &from_assigned);
    set_input_var(input_vars, to,   &to_assigned);
}


unique_ptr<BindingIdIter> PropertyPathPlan::get_binding_id_iter(ThreadInfo* thread_info) const {
    if (from_assigned) {
        auto automaton = path.get_transformed_automaton();
        set_automaton_transition_id(automaton);
        if (to_assigned) {
            // bool case
            return make_unique<PropertyPathCheck>(thread_info,
                                                  *model.nodes,
                                                  *model.type_from_to_edge,
                                                  *model.to_type_from_edge,
                                                  path_var,
                                                  from,
                                                  to,
                                                  automaton);
        } else {
            // enum starting on from
            return make_unique<PropertyPathEnum>(thread_info,
                                                 *model.nodes,
                                                 *model.type_from_to_edge,
                                                 *model.to_type_from_edge,
                                                 path_var,
                                                 from,
                                                 std::get<VarId>(to),
                                                 automaton);
        }
    } else {
        if (to_assigned) {
            // enum starting on to
            auto inverted_path = path.invert();
            auto automaton = inverted_path->get_transformed_automaton();
            set_automaton_transition_id(automaton);
            return make_unique<PropertyPathEnum>(thread_info,
                                                 *model.nodes,
                                                 *model.type_from_to_edge,
                                                 *model.to_type_from_edge,
                                                 path_var,
                                                 to,
                                                 std::get<VarId>(from),
                                                 automaton);
        } else {
            throw runtime_error("property path must have at least 1 node fixed");
        }
    }
    return nullptr;
}


void PropertyPathPlan::set_automaton_transition_id(PathAutomaton& automaton) const {
    // For each Transition instance in from_to vector, creates a TransitionId
    // instance that have an object id object of string label. It will be stored
    // in transition attribute of automaton
    for (size_t i = 0; i < automaton.from_to_connections.size(); i++) {
        vector<TransitionId> transition_id_vector;
        for (const auto &t : automaton.from_to_connections[i]) {
            transition_id_vector.push_back(
                TransitionId(t.to, model.get_object_id(GraphObject::make_identifiable(t.label)), t.inverse));
        }
        automaton.transitions.push_back(transition_id_vector);
    }
}
