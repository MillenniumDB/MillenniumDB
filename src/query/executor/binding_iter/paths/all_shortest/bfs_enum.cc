#include "bfs_enum.h"

#include <cassert>

#include "query/exceptions.h"
#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllShortest;

template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    // setted at object initialization:
    // first_next = true;
    // solution_found = nullptr;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto state_inserted = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::reset() {
    // Empty open, visited and optimal_distances
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();

    if (MULTIPLE_FINAL) {
        optimal_distances.clear();
    }

    first_next = true;
    solution_found = nullptr;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto state_inserted = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}


template <bool MULTIPLE_FINAL>
bool BFSEnum<MULTIPLE_FINAL>::next() {
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state->node_id.id)) {
            open.pop();
            return false;
        }

        // Starting state is solution
        if (automaton.is_final_state[automaton.start_state]) {
            if (MULTIPLE_FINAL) {
                optimal_distances.insert({current_state->node_id.id, 0});
            }
            auto new_state = visited.emplace(current_state->node_id, automaton.start_state, 0);
            auto path_id = path_manager.set_path(new_state.first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            results_found++;
            return true;
        }
    }

    // Check for next enumeration of state_reached
    if (solution_found != nullptr) {
enumeration:
        // Timeout
        if (__builtin_expect(!!(get_query_ctx().thread_info.interruption_requested), 0)) {
            throw InterruptedException();
        }
        if (solution_found->path_iter.next()) {
            auto path_id = path_manager.set_path(solution_found, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, solution_found->node_id);
            results_found++;
            return true;
        } else {
            solution_found = nullptr;
        }
    }

    while (open.size() > 0) {
        auto current_state = open.front();
        bool reached_final_state = expand_neighbors(*current_state);

        // Start enumeration for reached solutions
        if (reached_final_state) {
            solution_found->path_iter.start_enumeration();
            goto enumeration;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }
    return false;
}


template <bool MULTIPLE_FINAL>
bool BFSEnum<MULTIPLE_FINAL>::expand_neighbors(const SearchState& current_state) {
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
                    visited_search->path_iter.add(&current_state, transition.inverse, transition.type_id);

                    // Check if path is solution
                    if (automaton.is_final_state[next_state.automaton_state]) {
                        if (MULTIPLE_FINAL) {
                            auto node_reached_final = optimal_distances.find(next_state.node_id.id);  // must exist
                            if (node_reached_final->second == next_state.distance) {
                                solution_found = visited_search.operator->();
                                return true;
                            }
                        } else {
                            solution_found = visited_search.operator->();
                            return true;
                        }
                    }
                }
            } else {
                // Add state to visited and open and keep going unless it's an optimal final state
                auto reached_state = visited.emplace(ObjectId(iter->get_reached_node()),
                                                     transition.to,
                                                     current_state.distance + 1,
                                                     &current_state,
                                                     transition.inverse,
                                                     transition.type_id).first;
                open.push(reached_state.operator->());

                // Check if path is solution
                if (automaton.is_final_state[transition.to]) {
                    if (MULTIPLE_FINAL) {
                        auto node_reached_final = optimal_distances.find(next_state.node_id.id);
                        if (node_reached_final != optimal_distances.end()) {
                            if (node_reached_final->second == next_state.distance) {
                                solution_found = reached_state.operator->();
                                return true;
                            }
                        } else {
                            optimal_distances.insert({next_state.node_id.id, next_state.distance});
                            solution_found = reached_state.operator->();
                            return true;
                        }
                    } else {
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


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllShortest::BFSEnum(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}


template class Paths::AllShortest::BFSEnum<true>;
template class Paths::AllShortest::BFSEnum<false>;
