#include "dfs_check.h"

#include "system/path_manager.h"

using namespace std;
using namespace Paths::AnySimple;

template<bool CYCLIC>
void DFSCheck<CYCLIC>::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    first_next = true;

    // Add starting state to open
    current_start = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(
        automaton.start_state,
        0,
        make_unique<NullIndexIterator>(),
        current_start,
        ObjectId(),
        false,
        nullptr
    );

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();

    current_state_nodes.insert(current_start.id);
}

template<bool CYCLIC>
bool DFSCheck<CYCLIC>::_next()
{
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto& current_state = open.top();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state.node_id.id)) {
            open.pop();
            current_state_nodes.clear();
            return false;
        }

        // Starting state is solution
        if (current_state.node_id == end_object_id) {
            if (automaton.is_final_state[automaton.start_state]) {
                auto path_id = path_manager.set_path(&current_state, path_var);
                parent_binding->add(path_var, path_id);
                // We just want one solution, so we avoid expanding
                current_state.transition = automaton.from_to_connections[current_state.automaton_state].size(
                );
                return true;
            } else if (!CYCLIC) {
                // Acyclic can't have any more solutions when start node = end node
                open.pop();
                current_state_nodes.clear();
                return false;
            }
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.top();

        // current_state.transition == automaton.from_to_connections[current_state.automaton_state].size()
        // is used in expand_neighbors to mark a state that shouldn't be expanded
        if (current_state.transition >= automaton.from_to_connections[current_state.automaton_state].size()) {
            if (automaton.is_final_state[current_state.automaton_state]) {
                current_state_nodes.clear();
                stack<SearchStateDFS> empty;
                open.swap(empty);
            } else {
                // no need to erase from current_state_nodes
                open.pop();
            }
            return false;
        }

        // Get next state info (potential solution)
        auto state_reached = expand_neighbors(current_state);

        // A state was reached
        if (state_reached != nullptr) {
            // Check if new path is solution
            if (automaton.is_final_state[state_reached->automaton_state]
                && state_reached->node_id == end_object_id)
            {
                auto path_id = path_manager.set_path(state_reached, path_var);
                parent_binding->add(path_var, path_id);
                return true;
            }
        } else {
            // Pop and visit next state
            current_state_nodes.erase(current_state.node_id.id);
            open.pop();
        }
    }
    return false;
}

template<bool CYCLIC>
SearchStateDFS* DFSCheck<CYCLIC>::expand_neighbors(SearchStateDFS& current_state)
{
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
        auto& transition = automaton
                               .from_to_connections[current_state.automaton_state][current_state.transition];

        // Iterate over records and return simple paths
        while (current_state.iter->next()) {
            auto it = current_state_nodes.insert(current_state.iter->get_reached_node());
            if (!it.second) { // it.second is false if element was present before insert
                // If path can be cyclic, return solution only when the new node is the starting node and is also final
                if (CYCLIC && automaton.is_final_state[transition.to]) {
                    // This case only happens if the starting node and end node are the same
                    if (current_start == end_object_id
                        && ObjectId(current_state.iter->get_reached_node()) == current_start)
                    {
                        // Return new state, but avoid expand later
                        return &open.emplace(
                            transition.to,
                            automaton.from_to_connections[transition.to].size(),
                            make_unique<NullIndexIterator>(),
                            ObjectId(current_state.iter->get_reached_node()),
                            transition.type_id,
                            transition.inverse,
                            &current_state
                        );
                    }
                }
                continue;
            }

            if (current_state.iter->get_reached_node() == end_object_id.id) {
                current_state_nodes.erase(current_state.iter->get_reached_node());
                // Return new state, but avoid expand later
                return &open.emplace(
                    transition.to,
                    automaton.from_to_connections[transition.to].size(),
                    make_unique<NullIndexIterator>(),
                    ObjectId(current_state.iter->get_reached_node()),
                    transition.type_id,
                    transition.inverse,
                    &current_state
                );
            } else {
                // Return new state to expand later
                return &open.emplace(
                    transition.to,
                    0,
                    make_unique<NullIndexIterator>(),
                    ObjectId(current_state.iter->get_reached_node()),
                    transition.type_id,
                    transition.inverse,
                    &current_state
                );
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_state.transition++;
        if (current_state.transition < automaton.from_to_connections[current_state.automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return nullptr;
}

template<bool CYCLIC>
void DFSCheck<CYCLIC>::_reset()
{
    // Empty open
    stack<SearchStateDFS> empty;
    open.swap(empty);
    first_next = true;

    // Add starting state to open
    current_start = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    open.emplace(
        automaton.start_state,
        0,
        make_unique<NullIndexIterator>(),
        current_start,
        ObjectId(),
        false,
        nullptr
    );

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();

    current_state_nodes.clear();
    current_state_nodes.insert(current_start.id);
}

template<bool CYCLIC>
void DFSCheck<CYCLIC>::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        if (stats) {
            os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
               << " reset: " << stat_reset << " results: " << results << " idx_searches: " << idx_searches
               << "]\n";
        }
    }
    os << std::string(indent, ' ') << "Paths::AnySimple::DFSCheck(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}


template class Paths::AnySimple::DFSCheck<true>;
template class Paths::AnySimple::DFSCheck<false>;
