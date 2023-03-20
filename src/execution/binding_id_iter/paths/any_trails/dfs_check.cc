#include "dfs_check.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnyTrails;

DFSCheck::DFSCheck(ThreadInfo* thread_info,
                   VarId       path_var,
                   Id          start,
                   Id          end,
                   RPQ_DFA     automaton,
                   unique_ptr<IndexProvider> provider) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton),
    provider    (move(provider)) { }


bool DFSCheck::is_trail(const SearchStateDFS& path_state, ObjectId new_edge) {
    // Iterate over path backwards
    auto current = &path_state;
    while (current != nullptr) {
        // Path is not a trail (two edges are equal)
        if (current->edge_id == new_edge) {
            return false;
        }
        current = current->prev_state;
    }
    return true;
}


void DFSCheck::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), ObjectId(), false, nullptr);

    // Store ID for end object
    end_object_id = std::holds_alternative<ObjectId>(end) ? std::get<ObjectId>(end)
                                                          : (*parent_binding)[std::get<VarId>(end)];
}


bool DFSCheck::next() {
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
        if (automaton.is_final_state[automaton.start_state] && current_state.node_id == end_object_id) {
            auto path_id = path_manager.set_path(&current_state, path_var);
            parent_binding->add(path_var, path_id);
            stack<SearchStateDFS> empty;
            open.swap(empty);
            results_found++;
            return true;
        }
    }

    // Enumerate
    while (open.size() > 0) {
        auto& current_state = open.top();

        // Get next state info (potential solution)
        auto state_reached = current_state_has_next(current_state);

        // A state was reached
        if (state_reached != nullptr) {
            // Check if new path is solution
            if (automaton.is_final_state[state_reached->automaton_state] &&
                state_reached->node_id == end_object_id)
            {
                auto path_id = path_manager.set_path(state_reached, path_var);
                parent_binding->add(path_var, path_id);
                stack<SearchStateDFS> empty;
                open.swap(empty);
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


SearchStateDFS* DFSCheck::current_state_has_next(SearchStateDFS& current_state) {
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
                ObjectId(current_state.iter->get_node()),
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


void DFSCheck::set_iter(SearchStateDFS& current_state) {
    // Get current transition object from automaton
    const auto& transition = automaton.from_to_connections[current_state.automaton_state][current_state.transition];

    // Get iterator from custom index
    current_state.iter = provider->get_iterator(transition.type_id.id, transition.inverse, current_state.node_id.id);
    idx_searches++;
}


void DFSCheck::reset() {
    // Empty open
    stack<SearchStateDFS> empty;
    open.swap(empty);
    first_next = true;

    // Add starting state to open
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    open.emplace(automaton.start_state, 0, make_unique<NullIndexIterator>(), start_object_id, ObjectId(), ObjectId(), false, nullptr);

    // Store ID for end object
    end_object_id = std::holds_alternative<ObjectId>(end) ? std::get<ObjectId>(end)
                                                          : (*parent_binding)[std::get<VarId>(end)];
}


void DFSCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyTrails::DFSCheck(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}
