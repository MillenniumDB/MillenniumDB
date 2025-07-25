#include "bfs_rdpq_enum.h"

#include <cassert>

#include "graph_models/quad_model/quad_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/record.h"
#include "system/path_manager.h"

using namespace std;
using namespace Paths::Any;

// Evaluate data checks for a specific object
bool BFS_RDPQEnum::eval_data_check(
    uint64_t obj,
    vector<tuple<Operators, ObjectId, ObjectId>>& property_checks
)
{
    // Perform all data checks in transition
    for (auto& property_check : property_checks) {
        // Extract tuple <operator,key,value>
        auto op = get<0>(property_check);
        auto key_id = get<1>(property_check).id;
        auto value_id = get<2>(property_check).id;

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
            &get_query_ctx().thread_info.interruption_requested,
            Record<3>(min_prop_ids),
            Record<3>(max_prop_ids)
        );
        auto prop_record = prop_iter.next();

        // Perform specific data check
        if (prop_record != nullptr) {
            auto record_value_id = (*prop_record)[2];
            // Cases by operator
            if (op == Operators::EQ && record_value_id == value_id) {
                continue;
            } else if (op == Operators::NOT_EQ && record_value_id != value_id) {
                continue;
            } else { // Data check fails
                return false;
            }
        } else {
            // If the key is not present, assume that the data check failed
            return false;
        }
    }
    return true;
}

void BFS_RDPQEnum::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    first_next = true;

    // Add start object id to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Obtain states connected with the start state
    for (auto& t : automaton.from_to_connections[automaton.get_start()]) {
        // Perform all data checks in transition
        bool check_succeeded = eval_data_check(start_object_id.id, t.property_checks);

        // All the data checks succeeded
        if (check_succeeded) {
            auto state_inserted = visited
                                      .emplace(t.to, start_object_id, nullptr, false, ObjectId::get_null());

            // Inserted_state.second = true if first time visiting state
            if (state_inserted.second) { // State was actually inserted
                // Add pointer to new state in visited
                open.push(state_inserted.first.operator->());
            }
        }
    }

    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}

bool BFS_RDPQEnum::_next()
{
    // Check if first state is final
    if (first_next) {
        const auto current_state = open.front();

        // Check if node is valid
        auto node_iter = quad_model.nodes->get_range(
            &get_query_ctx().thread_info.interruption_requested,
            Record<1>({ current_state->node_id.id }),
            Record<1>({ current_state->node_id.id })
        );
        // Return false if node does not exist in db
        if (node_iter.next() == nullptr) {
            open.pop();
            return false;
        }
        first_next = false;

        // Initial state is the final state (only one final state in a DE automaton)
        if (current_state->automaton_state == automaton.get_final_state()) {
            auto reached_key = SearchState(
                current_state->automaton_state,
                current_state->node_id,
                nullptr,
                false,
                ObjectId::get_null()
            );
            auto path_id = path_manager.set_path(visited.insert(reached_key).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            return true;
        }
    }

    // Iterate
    while (open.size() > 0) {
        const auto current_state = open.front();
        auto state_reached = expand_neighbors(*current_state);
        // If it has next state then state_reached does not point to visited.end()
        if (state_reached != visited.end()) {
            open.push(state_reached.operator->());

            // Check if state is a result
            if (state_reached->automaton_state == automaton.get_final_state()) {
                // set binding;
                auto path_id = path_manager.set_path(state_reached.operator->(), path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, state_reached->node_id);
                return true;
            }
        } else {
            // Pop and visit next state
            iter.set_null();
            open.pop();
        }
    }
    return false;
}

boost::unordered_node_set<SearchState, std::hash<SearchState>>::iterator
    BFS_RDPQEnum::expand_neighbors(const SearchState& current_state)
{
    if (iter.is_null()) { // The first time a state is explored
        current_edge_transition = 0;
        current_data_transition = 0;

        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0) {
            return visited.end();
        }
        // Construct iter for this state
        set_iter(current_state);
    }

    // Iterate over state edge transitions
    while (current_edge_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
        // Current transition
        auto& transition = automaton
                               .from_to_connections[current_state.automaton_state][current_edge_transition];

        // Iterate over states connected to this one
        if (current_data_transition == 0) {
            child_record = iter.next();
        }
        while (child_record != nullptr) {
            // Perform data checks for the edge transition
            bool edge_succeeded = true;
            if (current_data_transition == 0) { // Avoid performing data checks repeatedly for the same edge
                edge_succeeded = eval_data_check((*child_record)[3], transition.property_checks);
            }

            // Perform all data checks in transition if edge data check has succeeded
            while (edge_succeeded
                   && current_data_transition < automaton.from_to_connections[transition.to].size())
            {
                // Current data transition
                auto& data_transition = automaton.from_to_connections[transition.to][current_data_transition];
                current_data_transition++;

                // Perform data checks
                bool succeeded = eval_data_check((*child_record)[2], data_transition.property_checks);

                // Expand destination
                if (succeeded) {
                    // Possible result + state in the query path
                    auto next_state_key = SearchState(
                        data_transition.to,
                        ObjectId((*child_record)[2]),
                        &current_state,
                        transition.inverse,
                        transition.type_id
                    );
                    auto inserted_state = visited.insert(next_state_key);

                    // Inserted_state.second = true if first time visiting state
                    if (inserted_state.second) { // State was actually inserted
                        // Return pointer to new state in visited
                        return inserted_state.first;
                    }
                }
            }

            // Visit next result from this edge transition
            child_record = iter.next();
            current_data_transition = 0;
        }

        // Construct new iter
        current_edge_transition++; // Expand next transition from this state if it exists
        if (current_edge_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
            set_iter(current_state);
            assert(current_data_transition == 0);
        }
    }

    // Finished expanding this state
    return visited.end();
}

void BFS_RDPQEnum::set_iter(const SearchState& current_state)
{
    // Gets current transition object from automaton
    const auto& transition = automaton
                                 .from_to_connections[current_state.automaton_state][current_edge_transition];

    // Gets iter from correct bpt with transition.inverse
    if (transition.inverse) {
        min_ids[0] = current_state.node_id.id;
        max_ids[0] = current_state.node_id.id;
        min_ids[1] = transition.type_id.id;
        max_ids[1] = transition.type_id.id;
        iter = quad_model.to_type_from_edge->get_range(
            &get_query_ctx().thread_info.interruption_requested,
            Record<4>(min_ids),
            Record<4>(max_ids)
        );
    } else {
        min_ids[0] = transition.type_id.id;
        max_ids[0] = transition.type_id.id;
        min_ids[1] = current_state.node_id.id;
        max_ids[1] = current_state.node_id.id;
        iter = quad_model.type_from_to_edge->get_range(
            &get_query_ctx().thread_info.interruption_requested,
            Record<4>(min_ids),
            Record<4>(max_ids)
        );
    }
    idx_searches++;
}

void BFS_RDPQEnum::_reset()
{
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();

    // Set initial vars
    first_next = true;
    iter.set_null();

    // Add start object id to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Obtain states connected with the start state
    for (auto& t : automaton.from_to_connections[automaton.get_start()]) {
        // Perform all data checks in transition
        bool check_succeeded = eval_data_check(start_object_id.id, t.property_checks);

        // All the data checks succeeded
        if (check_succeeded) {
            auto state_inserted = visited
                                      .emplace(t.to, start_object_id, nullptr, false, ObjectId::get_null());

            // Inserted_state.second = true if first time visiting state
            if (state_inserted.second) { // State was actually inserted
                // Return pointer to new state in visited
                open.push(state_inserted.first.operator->());
            }
        }
    }
}
