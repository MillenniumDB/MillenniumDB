#include "bfs_check.h"

#include <cassert>

#include "system/path_manager.h"

using namespace std;
using namespace Paths::ShortestKGroupsTrails;

void BFSCheck::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();

    expand_first_state(start_object_id);
}

void BFSCheck::_reset()
{
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();

    expand_first_state(start_object_id);
}

void BFSCheck::expand_first_state(ObjectId start)
{
    iter = make_unique<NullIndexIterator>();

    auto start_node_visited = visited.add(start, ObjectId(), ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state, 0);

    // Check if first state is final
    auto& current_state = open.front();

    // Return false if node does not exist in the database
    if (!provider->node_exists(current_state.path_state->node_id.id)) {
        open.pop();
        return;
    }

    solution.path_states.clear();
    solution.num_groups = 0;
    solution.last_depth = 0;

    first_state_is_solution = false;

    // Starting state is solution
    if (automaton.is_final_state[automaton.start_state]) {
        first_state_is_solution = true;
        solution.path_states.push_back(current_state.path_state);
        solution.num_groups = 1;
    }
}

bool BFSCheck::_next()
{
    if (first_state_is_solution) {
        first_state_is_solution = false;
        auto path_id = path_manager.set_path(solution.path_states.back(), path_var);
        parent_binding->add(path_var, path_id);
        return true;
    }

    while (open.size() > 0) {
        auto& current_state = open.front();

        // Enumerate reached solutions
        if (expand_neighbors(current_state)) {
            if (solution.num_groups > K) {
                queue<SearchState> empty;
                open.swap(empty);
                return false;
            }
            auto path_id = path_manager.set_path(solution.path_states.back(), path_var);
            parent_binding->add(path_var, path_id);
            return true;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }

    return false;
}

bool BFSCheck::expand_neighbors(const SearchState& current_state)
{
    // Check if this is the first time that current_state is explored
    if (iter->at_end()) {
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0) {
            return false;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Iterate over records until a final state is reached
        while (iter->next()) {
            auto reached_node = ObjectId(iter->get_reached_node());
            ObjectId edge(iter->get_edge());
            if (!is_trail(current_state.path_state, edge)) {
                continue;
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(
                reached_node,
                edge,
                transition.type_id,
                transition.inverse,
                current_state.path_state
            );

            auto reached_state_distance = current_state.distance + 1;
            open.emplace(new_visited_ptr, transition.to, reached_state_distance);

            // Check if new path is solution
            if (automaton.is_final_state[transition.to] && reached_node == end_object_id) {
                solution.path_states.push_back(new_visited_ptr);
                if (solution.last_depth < reached_state_distance) {
                    solution.last_depth = reached_state_distance;
                    solution.num_groups++;
                    return true;
                }
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return false;
}

void BFSCheck::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}
