#include "dfs_check.h"

#include <cassert>

#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnySimple;

template <bool CYCLIC>
void DFSCheck<CYCLIC>::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), false, nullptr);

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
}


template <bool CYCLIC>
bool DFSCheck<CYCLIC>::next() {
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
        if (current_state.node_id == end_object_id) {
            if (automaton.is_final_state[automaton.start_state]) {
                auto path_id = path_manager.set_path(&current_state, path_var);
                parent_binding->add(path_var, path_id);
                results_found++;
                return true;
            } else if (!CYCLIC) {
                // Acyclic can't have any more solutions when start node = end node
                open.pop();
                return false;
            }
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.top();

        // Discard states that contain the end node
        // since we already returned it and there are not more viable simple paths
        // We don't discard it immediately when its found because we need the reference to reconstruct the path
        if (current_state.node_id == end_object_id) {
            stack<SearchStateDFS> empty;
            open.swap(empty);
            return false;
        }

        // Get next state info (potential solution)
        auto state_reached = expand_neighbors(current_state);

        // A state was reached
        if (state_reached != nullptr) {
            // Check if new path is solution
            if (automaton.is_final_state[state_reached->automaton_state] &&
                state_reached->node_id == end_object_id)
            {
                auto path_id = path_manager.set_path(state_reached, path_var);
                parent_binding->add(path_var, path_id);
                results_found++;
                return true;
            }
        } else {
            // Pop and visit next state
            open.pop();
        }
    }
    return false;
}


template <bool CYCLIC>
SearchStateDFS* DFSCheck<CYCLIC>::expand_neighbors(SearchStateDFS& current_state) {
    // Check if this is the first time that current_state is explored
    if (current_state.iter->at_end()) {
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0) {
            return nullptr;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to state transition + iter (pipeline)
    while (current_state.transition < automaton.from_to_connections[current_state.automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_state.transition];

        // Iterate over records and return simple paths
        while (current_state.iter->next()) {
            // Reconstruct path and check if it's simple, discard paths that are not simple
            if (!is_simple_path(current_state, ObjectId(current_state.iter->get_reached_node()))) {
                // If path can be cyclic, return solution only when the new node is the starting node and is also final
                if (CYCLIC && automaton.is_final_state[transition.to]) {
                    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
                    // This case only happens if the starting node and end node are the same
                    if (start_object_id == end_object_id && ObjectId(current_state.iter->get_reached_node()) == start_object_id) {
                        return &open.emplace(
                            transition.to, // new state
                            0, // new current transition starts at the beginning
                            make_unique<NullIndexIterator>(),
                            ObjectId(current_state.iter->get_reached_node()),
                            transition.type_id,
                            transition.inverse,
                            &current_state);
                    }
                }
                continue;
            }

            // Return new state to expand later
            return &open.emplace(
                transition.to, // new state
                0, // new current transition starts at the beginning
                make_unique<NullIndexIterator>(),
                ObjectId(current_state.iter->get_reached_node()),
                transition.type_id,
                transition.inverse,
                &current_state);
        }

        // Construct new iter with the next transition (if there exists one)
        current_state.transition++;
        if (current_state.transition < automaton.from_to_connections[current_state.automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return nullptr;
}


template <bool CYCLIC>
void DFSCheck<CYCLIC>::reset() {
    // Empty open
    stack<SearchStateDFS> empty;
    open.swap(empty);
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), false, nullptr);

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
}


template <bool CYCLIC>
void DFSCheck<CYCLIC>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    if (CYCLIC) {
        os << "Paths::AnySimple::DFSCheck<SIMPLE>(idx_searches: " << idx_searches << ", found: " << results_found << ")";
    } else {
        os << "Paths::AnySimple::DFSCheck<ACYCLIC>(idx_searches: " << idx_searches << ", found: " << results_found << ")";
    }
}


template class Paths::AnySimple::DFSCheck<true>;
template class Paths::AnySimple::DFSCheck<false>;
