#include "dfs_enum.h"

#include <cassert>

#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnySimple;

template <bool CYCLIC>
void DFSEnum<CYCLIC>::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), false, nullptr);
}


template <bool CYCLIC>
bool DFSEnum<CYCLIC>::next() {
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
            reached_final.insert(current_state.node_id.id);  // Return a single path per reached node
            auto path_id = path_manager.set_path(&current_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state.node_id);
            results_found++;
            return true;
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.top();

        // Avoid cyclic solutions from expanding, using the fact that valid states from open always have valid transitions
        if (CYCLIC && current_state.transition >= automaton.from_to_connections[current_state.automaton_state].size()) {
            open.pop();
            continue;
        }

        // Get next state info (potential solution)
        auto state_reached = expand_neighbors(current_state);

        // A state was reached
        if (state_reached != nullptr) {
            // Check if new path is solution
            if (automaton.is_final_state[state_reached->automaton_state]) {
                auto node_reached_final = reached_final.find(state_reached->node_id.id);
                if (node_reached_final == reached_final.end()) {  // Return a single path per reached node
                    reached_final.insert(state_reached->node_id.id);
                    auto path_id = path_manager.set_path(state_reached, path_var);
                    parent_binding->add(path_var, path_id);
                    parent_binding->add(end, state_reached->node_id);
                    results_found++;
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


template <bool CYCLIC>
SearchStateDFS* DFSEnum<CYCLIC>::expand_neighbors(SearchStateDFS& current_state) {
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
                    if (ObjectId(current_state.iter->get_reached_node()) == start_object_id) {
                        auto node_reached_final = reached_final.find(current_state.iter->get_reached_node());
                        if (node_reached_final == reached_final.end()) {
                            return &open.emplace(
                                transition.to,
                                automaton.from_to_connections[transition.to].size(),  // Add invalid transition to discard state later
                                make_unique<NullIndexIterator>(),
                                ObjectId(current_state.iter->get_reached_node()),
                                transition.type_id,
                                transition.inverse,
                                &current_state);
                        }
                    }
                }
                continue;
            }

            // Return new state to expand later
            return &open.emplace(
                transition.to,
                0, make_unique<NullIndexIterator>(),
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
void DFSEnum<CYCLIC>::reset() {
    // Empty open
    stack<SearchStateDFS> empty;
    open.swap(empty);
    reached_final.clear();
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), false, nullptr);
}


template <bool CYCLIC>
void DFSEnum<CYCLIC>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    if (CYCLIC) {
        os << "Paths::AnySimple::DFSEnum<SIMPLE>(idx_searches: " << idx_searches << ", found: " << results_found << ")";
    } else {
        os << "Paths::AnySimple::DFSEnum<ACYCLIC>(idx_searches: " << idx_searches << ", found: " << results_found << ")";
    }
}


template class Paths::AnySimple::DFSEnum<true>;
template class Paths::AnySimple::DFSEnum<false>;
