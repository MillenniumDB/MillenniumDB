#include "bfs_rdpq_check.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/index/record.h"

using namespace std;
using namespace Paths::AnyShortest;

BFS_RDPQCheck::BFS_RDPQCheck(ThreadInfo*   thread_info,
                             VarId         path_var,
                             Id            start,
                             Id            end,
                             RDPQAutomaton automaton) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton) { }


// Evaluate data checks for a specific object
bool BFS_RDPQCheck::eval_data_check(uint64_t obj, vector<tuple<Operators, string, QueryElement>>& property_checks) {
    // Perform all data checks in transition
    for (auto& property_check: property_checks) {
        // Extract tuple <operator,key,value>
        auto op = get<0>(property_check);
        auto key = get<1>(property_check);
        auto key_id = quad_model.get_object_id(GraphObject::make_string(key)).id;
        auto value = get<2>(property_check);
        auto value_id = quad_model.get_object_id(value.to_graph_object()).id;

        // Search B+Tree for *values* given <obj,key>
        array<uint64_t, 3> min_prop_ids;
        array<uint64_t, 3> max_prop_ids;
        min_prop_ids[0] = obj;
        max_prop_ids[0] = obj;
        min_prop_ids[1] = key_id;
        max_prop_ids[1] = key_id;
        min_prop_ids[2] = 0;
        max_prop_ids[2] = UINT64_MAX;
        auto prop_iter = quad_model.object_key_value->get_range(
            &thread_info->interruption_requested,
            Record<3>(min_prop_ids),
            Record<3>(max_prop_ids));
        auto prop_record = prop_iter->next();

        // Perform specific data check
        if (prop_record != nullptr) {
            auto record_value_id = prop_record->ids[2];
            // Cases by operator
            if (op == Operators::EQ && record_value_id == value_id) {
                continue;
            } else if (op == Operators::NOT_EQ && record_value_id != value_id) {
                continue;
            } else {  // Data check fails
                return false;
            }
        } else {
            // If the key is not present, assume that the data check failed
            return false;
        }
    }
    return true;
}


void BFS_RDPQCheck::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    is_first = true;

    // Init start object id
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ?
        std::get<ObjectId>(start) :
        (*parent_binding)[std::get<VarId>(start)]);

    // Init end object id
    end_object_id = (std::holds_alternative<ObjectId>(end)) ?
        std::get<ObjectId>(end) :
        (*parent_binding)[std::get<VarId>(end)];

    // Obtain states connected with the start state
    for (auto& t : automaton.from_to_connections[automaton.get_start()]) {

        // Perform all data checks in transition
        bool check_succeeded = eval_data_check(start_object_id.id, t.property_checks);

        // All the data checks succeeded
        if (check_succeeded) {
            auto state_inserted = visited.emplace(t.to,
                                                  start_object_id,
                                                  nullptr,
                                                  false,
                                                  ObjectId::get_null());

            // Inserted_state.second = true if first time visiting state
            if (state_inserted.second) {  // State was actually inserted
                // Add pointer to state in visited
                open.push(state_inserted.first.operator->());
            }
        }
    }

    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}


bool BFS_RDPQCheck::next() {
    // Check if first state is final
    if (is_first) {
        const auto current_state = open.front();

        // Check if node is valid
        auto node_iter = quad_model.nodes->get_range(&thread_info->interruption_requested,
                                                     Record<1>({current_state->node_id.id}),
                                                     Record<1>({current_state->node_id.id}));
        // Return false if node does not exist in bd
        if (node_iter->next() == nullptr) {
            open.pop();
            return false;
        }
        is_first = false;

        // State is final and current node is the end node
        if (current_state->automaton_state == automaton.get_final_state() && current_state->node_id == end_object_id) {
            auto path_id = path_manager.set_path(current_state, path_var);
            parent_binding->add(path_var, path_id);
            queue<const SearchState*> empty;
            open.swap(empty);
            results_found++;
            return true;
        }
    }

    // Iterate
    while (open.size() > 0) {
        const auto current_state = open.front();

        // Iterate over state edge transitions
        for (auto& transition : automaton.from_to_connections[current_state->automaton_state]) {
            // Iterate over states connected to this one
            set_iter(transition, current_state);
            auto child_record = iter->next();
            while (child_record != nullptr) {
                // Perform data checks for the edge transition
                if (!eval_data_check(child_record->ids[3], transition.property_checks)) {
                    child_record = iter->next();
                    continue;
                }

                // Perform all data checks in transition if edge data check has succeeded
                for (auto& data_transition : automaton.from_to_connections[transition.to]) {
                    // Perform data checks
                    bool succeeded = eval_data_check(child_record->ids[2], data_transition.property_checks);

                    // Expand destination
                    if (succeeded) {
                        // Possible result + state in the query path
                        auto next_state_key = SearchState(
                            data_transition.to,
                            ObjectId(child_record->ids[2]),
                            current_state,
                            transition.inverse,
                            transition.type_id);
                        auto inserted_state = visited.insert(next_state_key);

                        // Inserted_state.second = true if first time visiting state
                        if (inserted_state.second) {  // State was actually inserted
                            open.push(inserted_state.first.operator->());
                        }

                        // Check if state is final
                        if (next_state_key.automaton_state == automaton.get_final_state()
                            && next_state_key.node_id == end_object_id) {
                            auto path_id = path_manager.set_path(inserted_state.first.operator->(),
                                                                 path_var);
                            parent_binding->add(path_var, path_id);
                            queue<const SearchState*> empty;
                            open.swap(empty);
                            results_found++;
                            return true;
                        }
                    }
                }

                // Visit next result from this edge transition
                child_record = iter->next();
            }
        }

        // Pop to visit next state
        open.pop();
    }
    return false;
}


void BFS_RDPQCheck::set_iter(const RDPQTransition& transition, const SearchState* current_state) {
    // Get iter from correct bpt_tree according to inverse attribute
    if (transition.inverse) {
        min_ids[0] = current_state->node_id.id;
        max_ids[0] = current_state->node_id.id;
        min_ids[1] = transition.type_id.id;
        max_ids[1] = transition.type_id.id;
        iter = quad_model.to_type_from_edge->get_range(&thread_info->interruption_requested,
                                                       Record<4>(min_ids),
                                                       Record<4>(max_ids));
    } else {
        min_ids[0] = transition.type_id.id;
        max_ids[0] = transition.type_id.id;
        min_ids[1] = current_state->node_id.id;
        max_ids[1] = current_state->node_id.id;
        iter = quad_model.type_from_to_edge->get_range(&thread_info->interruption_requested,
                                                       Record<4>(min_ids),
                                                       Record<4>(max_ids));
    }
    bpt_searches++;
}


void BFS_RDPQCheck::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();

    // Set initial vars
    is_first = true;

    // Add start object id to structures
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ?
        std::get<ObjectId>(start) :
        (*parent_binding)[std::get<VarId>(start)]
    );

    // Set end object id
    end_object_id = (std::holds_alternative<ObjectId>(end)) ?
        std::get<ObjectId>(end) :
        (*parent_binding)[std::get<VarId>(end)];

    // Obtain states connected with the start state
    for (auto& t : automaton.from_to_connections[automaton.get_start()]) {

        // Perform all data checks in transition
        bool check_succeeded = eval_data_check(start_object_id.id, t.property_checks);

        // All the data checks succeeded
        if (check_succeeded) {
            auto state_inserted = visited.emplace(t.to,
                                                  start_object_id,
                                                  nullptr,
                                                  false,
                                                  ObjectId::get_null());

            // Inserted_state.second = true if first time visiting state
            if (state_inserted.second) {  // State was actually inserted
                // Add pointer to state in visited
                open.push(state_inserted.first.operator->());
            }
        }
    }
}


void BFS_RDPQCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyShortest::BFS_RDPQCheck(bpt_searches: " << bpt_searches
       << ", found: " << results_found <<")";
}
