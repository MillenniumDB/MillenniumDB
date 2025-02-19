#include "bfs_check.h"

#include "system/path_manager.h"

using namespace std;
using namespace Paths::ShortestKSimple;

template<bool CYCLIC>
void BFSCheck<CYCLIC>::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();

    expand_first_state(start_object_id);
}


template<bool CYCLIC>
void BFSCheck<CYCLIC>::_reset()
{
    // Empty BFS structures
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    solutions.clear();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();

    expand_first_state(start_object_id);
}

template<bool CYCLIC>
void BFSCheck<CYCLIC>::expand_first_state(ObjectId start)
{
    iter = make_unique<NullIndexIterator>();

    auto start_node_visited = visited.add(start, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Check if first state is final
    auto& current_state = open.front();

    // Return false if node does not exist in the database
    if (!provider->node_exists(current_state.path_state->node_id.id)) {
        open.pop();
        return;
    }

    first_state_is_solution = false;
    if (current_state.path_state->node_id == end_object_id) {
        if (automaton.is_final_state[automaton.start_state]) {
            first_state_is_solution = true;
            solutions.push_back(current_state.path_state);
        } else if constexpr (!CYCLIC) {
            // Acyclic can't have any more solutions when start node = end node
            open.pop();
            return;
        }
    }
}

template<bool CYCLIC>
bool BFSCheck<CYCLIC>::_next()
{
    if (first_state_is_solution) {
        first_state_is_solution = false;
        auto path_id = path_manager.set_path(solutions.back(), path_var);
        parent_binding->add(path_var, path_id);
        return true;
    }

    while (open.size() > 0) {
        auto& current_state = open.front();

        if (expand_neighbors(current_state)) {
            if (solutions.size() == K) {
                queue<SearchState> empty;
                open.swap(empty);
            }

            auto path_id = path_manager.set_path(solutions.back(), path_var);
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

template<bool CYCLIC>
bool BFSCheck<CYCLIC>::expand_neighbors(const SearchState& current_state)
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
            bool add_to_open = true;
            if (CYCLIC) { // SIMPLE
                if (repeats_node(current_state.path_state, reached_node)) {
                    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()]
                                                              : start.get_OID();
                    if (!automaton.is_final_state[transition.to]
                        || reached_node != start_object_id)
                    {
                        continue;
                    }
                    add_to_open = false;
                }
            } else { // ACYCLIC
                if (repeats_node(current_state.path_state, reached_node)) {
                    continue;
                }
            }

            if (reached_node == end_object_id) {
                if (!automaton.is_final_state[transition.to]) {
                    // avoid continuing the exploration, as it is impossible to have a simple or acyclic path
                    continue;
                }
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(
                reached_node,
                transition.type_id,
                transition.inverse,
                current_state.path_state
            );

            if (add_to_open) {
                open.emplace(new_visited_ptr, transition.to);
            }

            // Check if new path is solution
            if (automaton.is_final_state[transition.to]
                && reached_node == end_object_id)
            {
                solutions.push_back(new_visited_ptr);
                return true;
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


template<bool CYCLIC>
void BFSCheck<CYCLIC>::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}


template class Paths::ShortestKSimple::BFSCheck<true>;
template class Paths::ShortestKSimple::BFSCheck<false>;
