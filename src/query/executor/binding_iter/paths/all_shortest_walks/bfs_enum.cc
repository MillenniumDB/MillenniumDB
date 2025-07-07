#include "bfs_enum.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "system/path_manager.h"

using namespace std;
using namespace Paths::AllShortest;

template<bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto state_inserted = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}

template<bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::_reset()
{
    // Empty open, visited and optimal_distances
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    iter_arena.clear();

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

template<bool MULTIPLE_FINAL>
bool BFSEnum<MULTIPLE_FINAL>::_next()
{
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
                optimal_distances.insert({ current_state->node_id.id, 0 });
            }
            auto new_state = visited.emplace(current_state->node_id, automaton.start_state, 0);
            auto path_id = path_manager.set_path(new_state.first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            return true;
        }
    }

    // Check for next enumeration of state_reached
    if (solution_found != nullptr) {
        // Timeout
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        if (solution_found->next()) {
            auto path_id = path_manager.set_path(solution_found, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, solution_found->node_id);
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
            solution_found->start_enumeration();
            auto path_id = path_manager.set_path(solution_found, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, solution_found->node_id);
            return true;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }
    return false;
}

template<bool MULTIPLE_FINAL>
bool BFSEnum<MULTIPLE_FINAL>::expand_neighbors(const SearchState& current_state)
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

        // Iterate over records until a final state is reached with a shortest path
        while (iter->next()) {
            ObjectId reached_node(iter->get_reached_node());
            SearchState next_state(reached_node, transition.to, current_state.distance + 1);

            // Check if next state has already been visited
            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                // Consider next_state only if it has an optimal distance
                if (visited_search->distance == next_state.distance) {
                    auto new_iter_transition = iter_arena.add(
                        &current_state,
                        transition.inverse,
                        transition.type_id
                    );
                    visited_search->add_transition(new_iter_transition);

                    // Check if path is solution
                    if (automaton.is_final_state[next_state.automaton_state]) {
                        if (MULTIPLE_FINAL) {
                            // must exist
                            auto node_reached_final = optimal_distances.find(next_state.node_id.id);
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
                auto new_iter_transition = iter_arena.add(&current_state, transition.inverse, transition.type_id);
                // Add state to visited and open and keep going unless it's an optimal final state
                auto reached_state = visited.emplace(
                                             reached_node,
                                             transition.to,
                                             current_state.distance + 1,
                                             new_iter_transition
                                         ).first;
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
                            optimal_distances.insert({ next_state.node_id.id, next_state.distance });
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

template<bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        if (stats) {
            os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
               << " reset: " << stat_reset << " results: " << results << " idx_searches: " << idx_searches
               << "]\n";
        }
    }
    os << std::string(indent, ' ') << "Paths::AllShortest::BFSEnum(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}

template class Paths::AllShortest::BFSEnum<true>;
template class Paths::AllShortest::BFSEnum<false>;
