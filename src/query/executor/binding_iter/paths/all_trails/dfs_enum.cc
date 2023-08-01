#include "dfs_enum.h"

#include <cassert>

#include "query/var_id.h"
#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllTrails;

void DFSEnum::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), ObjectId(), false, nullptr);
}


bool DFSEnum::next() {
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
            auto path_id = path_manager.set_path(&current_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state.node_id);
            results_found++;
            return true;
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.top();

        // Get next state info (potential solution)
        auto state_reached = expand_neighbors(current_state);

        // A state was reached
        if (state_reached != nullptr) {
            // Check if new path is solution
            if (automaton.is_final_state[state_reached->automaton_state]) {
                auto path_id = path_manager.set_path(state_reached, path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, state_reached->node_id);
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


SearchStateDFS* DFSEnum::expand_neighbors(SearchStateDFS& current_state) {
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

        // Iterate over records and return trails
        while (current_state.iter->next()) {
            // Reconstruct path and check if it's a trail, discard paths that are not trails
            if (!is_trail(current_state, ObjectId(current_state.iter->get_edge()))) {
                continue;
            }

            // Return new state to expand later
            return &open.emplace(
                transition.to,
                0, make_unique<NullIndexIterator>(),
                ObjectId(current_state.iter->get_reached_node()),
                ObjectId(current_state.iter->get_edge()),
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


void DFSEnum::reset() {
    // Empty open
    stack<SearchStateDFS> empty;
    open.swap(empty);
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), ObjectId(), false, nullptr);
}


void DFSEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllTrails::DFSEnum(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}
