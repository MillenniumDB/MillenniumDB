#include "dijkstra_check.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/index/record.h"

using namespace std;
using namespace Paths::AnyShortest;

DijkstraCheck::DijkstraCheck(ThreadInfo*   thread_info,
                             VarId         path_var,
                             Id            start,
                             Id            end,
                             RDPQAutomaton automaton,
                             ObjectId      cost_key) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton),
    cost_key    (cost_key) { }


// Evaluate data checks for a specific object
bool DijkstraCheck::eval_data_check(uint64_t obj, vector<tuple<Operators, string, QueryElement>>& property_checks) {
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


void DijkstraCheck::begin(BindingId& _parent_binding) {
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
                // Add new queue state to open
                open.push(DijkstraQueueState(state_inserted.first.operator->(), 0));
            }
        }
    }

    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}


bool DijkstraCheck::next() {
    // Check if first state is final
    if (is_first) {
        const auto queue_state = open.top();

        // Check if node is valid
        auto node_iter = quad_model.nodes->get_range(&thread_info->interruption_requested,
                                                     Record<1>({queue_state.state->node_id.id}),
                                                     Record<1>({queue_state.state->node_id.id}));
        // Return false if node does not exist in db
        if (node_iter->next() == nullptr) {
            open.pop();
            return false;
        }
        is_first = false;
    }

    // Iterate
    while (open.size() > 0) {
        const auto queue_state = open.top();

        // Check if this is a valid queue state (up to date with the best cost)
        if (queue_state.cost != queue_state.state->cost) {
            open.pop();
            continue;
        }

        // Iterate over state edge transitions
        for (auto& transition : automaton.from_to_connections[queue_state.state->automaton_state]) {
            // Iterate over states connected to this one
            set_iter(transition, queue_state.state);
            auto child_record = iter->next();
            while (child_record != nullptr) {
                // Perform data checks for the edge transition
                if (!eval_data_check(child_record->ids[3], transition.property_checks)) {
                    child_record = iter->next();
                    continue;
                }

                // Get projection for edge cost
                // Search B+Tree for *cost* given <edge,key>
                array<uint64_t, 3> min_prop_ids;
                array<uint64_t, 3> max_prop_ids;
                min_prop_ids[0] = child_record->ids[3];
                max_prop_ids[0] = child_record->ids[3];
                min_prop_ids[1] = cost_key.id;
                max_prop_ids[1] = cost_key.id;
                min_prop_ids[2] = 0;
                max_prop_ids[2] = UINT64_MAX;
                auto prop_iter = quad_model.object_key_value->get_range(
                    &thread_info->interruption_requested,
                    Record<3>(min_prop_ids),
                    Record<3>(max_prop_ids));
                auto prop_record = prop_iter->next();

                // Check cost
                // TODO: Discuss what to do if the cost is negative, float or non numeric
                if (prop_record != nullptr) {
                    auto record_value_id = prop_record->ids[2];
                    edge_cost = quad_model.get_graph_object(ObjectId(record_value_id)).value.i;
                } else {
                    // If the key is not present, discard this path
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
                        auto next_state_key = SearchStateDijkstra(
                            data_transition.to,
                            ObjectId(child_record->ids[2]),
                            queue_state.state,
                            transition.inverse,
                            transition.type_id,
                            queue_state.state->cost + edge_cost);
                        auto inserted_state = visited.insert(next_state_key);

                        // Inserted_state.second = true if first time visiting state
                        if (inserted_state.second) {  // State was actually inserted
                            open.push(DijkstraQueueState(inserted_state.first.operator->(), inserted_state.first->cost));
                        } else {
                            auto state_entry = inserted_state.first;

                            // Update visited entry with better cost
                            if (next_state_key.cost < state_entry->cost) {
                                state_entry->previous = next_state_key.previous;
                                state_entry->inverse_direction = next_state_key.inverse_direction;
                                state_entry->type_id = next_state_key.type_id;
                                state_entry->cost = next_state_key.cost;

                                // Push 'better' alternative path to the queue
                                open.push(DijkstraQueueState(state_entry.operator->(), state_entry->cost));
                            }
                        }
                    }
                }

                // Visit next result from this edge transition
                child_record = iter->next();
            }
        }

        // Pop to visit next state
        open.pop();

        // Check if a result was found
        if (queue_state.state->automaton_state == automaton.get_final_state() && queue_state.state->node_id == end_object_id) {
            // set binding
            auto path_id = path_manager.set_path(queue_state.state, path_var);
            parent_binding->add(path_var, path_id);
            priority_queue<DijkstraQueueState, vector<DijkstraQueueState>, DijkstraQueueStateComp> empty;
            open.swap(empty);
            results_found++;
            return true;
        }
    }
    return false;
}


void DijkstraCheck::set_iter(const RDPQTransition& transition, const SearchStateDijkstra* current_state) {
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


void DijkstraCheck::reset() {
    // Empty open and visited
    priority_queue<DijkstraQueueState, vector<DijkstraQueueState>, DijkstraQueueStateComp> empty;
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
                // Add new queue state to open
                open.push(DijkstraQueueState(state_inserted.first.operator->(), 0));
            }
        }
    }
}


void DijkstraCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyShortest::DijkstraCheck(bpt_searches: " << bpt_searches
       << ", found: " << results_found <<")";
}
