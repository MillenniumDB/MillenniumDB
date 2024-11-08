#include "bfs_check.h"

#include "macros/likely.h"
#include "query/exceptions.h"
#include "query/var_id.h"
#include "system/path_manager.h"

using namespace std;
using namespace Paths::AllShortest;

void BFSCheck::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    // setted at object initialization:
    // first_next = true;
    // solution_found = nullptr;
    // optimal_distance = UINT64_MAX

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_state = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();
}


void BFSCheck::_reset() {
    // Empty BFS structures
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    iter_arena.clear();

    first_next = true;
    solution_found = nullptr;
    optimal_distance = UINT64_MAX;

    // Add starting state to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_state = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();
}


bool BFSCheck::_next() {
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state->node_id.id)) {
            open.pop();
            return false;
        }

        // Starting state is a solution
        if (automaton.is_final_state[automaton.start_state] && current_state->node_id == end_object_id) {
            auto new_state = visited.emplace(current_state->node_id, automaton.start_state, 0);
            auto path_id = path_manager.set_path(new_state.first.operator->(), path_var);
            parent_binding->add(path_var, path_id);

            // The empty path is the shortest path possible and we can stop searching
            open.pop(); // after pop the queue is empty
            return true;
        }
    }

    // Check if we have a pending result to enumerate
    if (solution_found != nullptr) {
enumeration:
        // Handle timeout
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        if (solution_found->path_iter.next()) {
            auto path_id = path_manager.set_path(solution_found, path_var);
            parent_binding->add(path_var, path_id);
            return true;
        } else {
            solution_found = nullptr;
        }
    }

    while (open.size() > 0) {
        auto current_state = open.front();

        if (expand_neighbors(*current_state)) {
            solution_found->path_iter.start_enumeration();
            goto enumeration;
        } else {
            // Pop and visit next state in the queue
            open.pop();
        }
    }
    return false;
}


bool BFSCheck::expand_neighbors(const SearchState& current_state) {
    // if iter->at_end() this is the first time that current_state is explored and we need to set the iter
    if (iter->at_end()) {
        // stop if automaton state has not outgoing transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0) {
            return false;
        }
        current_transition = 0;
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Iterate over records until a final state is reached with a shortest path
        while (iter->next()) {
            SearchState next_state(ObjectId(iter->get_reached_node()),
                                   transition.to,
                                   current_state.distance + 1);

            // Check if next state has already been visited
            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                // Consider next_state only if it has an optimal distance
                if (visited_search->distance == next_state.distance) {
                    auto new_iter_transition = iter_arena.add(&current_state, transition.inverse, transition.type_id);
                    visited_search->path_iter.add(new_iter_transition);

                    // Check if path is solution
                    if (next_state.node_id == end_object_id &&
                        automaton.is_final_state[next_state.automaton_state])
                    {
                        if (optimal_distance == next_state.distance) {
                            solution_found = visited_search.operator->();
                            return true;
                        }
                    }
                }
            } else {
                auto new_iter_transition = iter_arena.add(
                    &current_state,
                    transition.inverse,
                    transition.type_id);
                // Add state to visited and open and keep going unless it's an optimal final state
                auto reached_state = visited.emplace(
                    ObjectId(iter->get_reached_node()),
                    transition.to,
                    current_state.distance + 1,
                    new_iter_transition).first;
                open.push(reached_state.operator->());

                // Check if path is solution
                if (reached_state->node_id == end_object_id
                    && automaton.is_final_state[transition.to])
                {
                    if (optimal_distance == UINT64_MAX) {
                        optimal_distance = next_state.distance;
                        solution_found = reached_state.operator->();
                        return true;
                    } else if (optimal_distance == next_state.distance) {
                        solution_found = reached_state.operator->();
                        return true;
                    }
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


void BFSCheck::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
