#include "dfs_enum.h"

#include <cassert>

#include "system/path_manager.h"

using namespace std;
using namespace Paths::Any;

template <bool MULTIPLE_FINAL>
void DFSEnum<MULTIPLE_FINAL>::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    // Add starting state to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(start_object_id, automaton.start_state);
    visited.emplace(automaton.start_state, start_object_id, nullptr, true, ObjectId::get_null());
}


template <bool MULTIPLE_FINAL>
bool DFSEnum<MULTIPLE_FINAL>::_next() {
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto& current_state = open.top();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state.node_id.id)) {
            open.pop();
            return false;
        }

        // Starting state is solution
        if (automaton.is_final_state[automaton.start_state]) {
            auto reached_state = Paths::Any::SearchState(
                automaton.start_state, open.top().node_id, nullptr, true, ObjectId::get_null());
            if (MULTIPLE_FINAL) {
                reached_final.insert(current_state.node_id.id);
            }
            auto path_id = path_manager.set_path(visited.insert(reached_state).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, open.top().node_id);
            return true;
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.top();

        // Get next state info (potential solution)
        auto reached_state = expand_neighbors(current_state);

        // A state was reached
        if (reached_state != visited.end()) {
            open.emplace(reached_state->node_id, reached_state->automaton_state);

            // Check if new path is solution
            if (automaton.is_final_state[reached_state->automaton_state]) {
                if (MULTIPLE_FINAL) {
                    auto node_reached_final = reached_final.find(reached_state->node_id.id);
                    if (node_reached_final == reached_final.end()) {
                        reached_final.insert(reached_state->node_id.id);
                        auto path_id = path_manager.set_path(reached_state.operator->(), path_var);
                        parent_binding->add(path_var, path_id);
                        parent_binding->add(end, reached_state->node_id);
                        return true;
                    }
                } else {
                    auto path_id = path_manager.set_path(reached_state.operator->(), path_var);
                    parent_binding->add(path_var, path_id);
                    parent_binding->add(end, reached_state->node_id);
                    return true;
                }
            }
        } else {
            // Pop and visit next state
            open.pop();
        }
    }
    return false;
}


template <bool MULTIPLE_FINAL>
robin_hood::unordered_node_set<Paths::Any::SearchState>::iterator
  DFSEnum<MULTIPLE_FINAL>::expand_neighbors(DFSSearchState& state)
{
    if (state.iter->at_end()) {  // Check if this is the first time that current_state is explored
        state.current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[state.state].size() == 0) {
            return visited.end();
        }
        set_iter(state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to state transition + iter (pipeline)
    while (state.current_transition < automaton.from_to_connections[state.state].size()) {
        auto& transition = automaton.from_to_connections[state.state][state.current_transition];

        // Iterate over records and return paths
        while (state.iter->next()) {
            auto current_state = Paths::Any::SearchState(state.state, state.node_id, nullptr, true, ObjectId::get_null());
            auto next_state = Paths::Any::SearchState(transition.to,
                                                              ObjectId(state.iter->get_reached_node()),
                                                              visited.find(current_state).operator->(),
                                                              transition.inverse,
                                                              transition.type_id);

            // Check if child state is not already visited
            auto inserted_state = visited.insert(next_state);
            // Inserted_state.second = true if and only if state was inserted
            if (inserted_state.second) {
                // Returns pointer to state
                return inserted_state.first;
            }
        }

        // Construct new iter with the next transition (if there exists one)
        state.current_transition++;
        if (state.current_transition < automaton.from_to_connections[state.state].size()) {
            set_iter(state);
        }
    }
    return visited.end();
}


template <bool MULTIPLE_FINAL>
void DFSEnum<MULTIPLE_FINAL>::_reset() {
    // Empty open and visited
    stack<DFSSearchState> empty;
    open.swap(empty);
    visited.clear();
    if (MULTIPLE_FINAL) {
        reached_final.clear();
    }
    first_next = true;

    // Add starting state to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(start_object_id, automaton.start_state);
    visited.emplace(automaton.start_state, start_object_id, nullptr, true, ObjectId::get_null());
}


template <bool MULTIPLE_FINAL>
void DFSEnum<MULTIPLE_FINAL>::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


template class Paths::Any::DFSEnum<true>;
template class Paths::Any::DFSEnum<false>;
