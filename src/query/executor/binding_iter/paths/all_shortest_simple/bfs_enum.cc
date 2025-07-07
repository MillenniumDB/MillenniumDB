#include "bfs_enum.h"

#include <cassert>

#include "system/path_manager.h"

using namespace std;
using namespace Paths::AllShortestSimple;

template<bool CYCLIC>
void BFSEnum<CYCLIC>::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state, 0);
}

template<bool CYCLIC>
bool BFSEnum<CYCLIC>::_next()
{
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto& current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state.path_state->node_id.id)) {
            open.pop();
            return false;
        }

        // Starting state is solution
        if (automaton.is_final_state[automaton.start_state]) {
            optimal_distances.insert({ current_state.path_state->node_id.id, 0 }
            ); // Store optimal distance to node
            auto path_id = path_manager.set_path(current_state.path_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state.path_state->node_id);
            return true;
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.front();
        auto reached_final_state = expand_neighbors(current_state);

        // Enumerate reached solutions
        if (reached_final_state != nullptr) {
            auto path_id = path_manager.set_path(reached_final_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, reached_final_state->node_id);
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
const PathState* BFSEnum<CYCLIC>::expand_neighbors(const SearchState& current_state)
{
    // Check if this is the first time that current_state is explored
    if (iter->at_end()) {
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0) {
            return nullptr;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Iterate over records until a final state is reached
        while (iter->next()) {
            // Reconstruct path and check if it's simple, discard paths that are not simple
            if (!is_simple_path(current_state.path_state, ObjectId(iter->get_reached_node()))) {
                // If path can be cyclic, return solution only when the new node is the starting node and is also final
                if (CYCLIC && automaton.is_final_state[transition.to]) {
                    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()]
                                                              : start.get_OID();
                    if (ObjectId(iter->get_reached_node()) == start_object_id) {
                        auto node_reached_final = optimal_distances.find(iter->get_reached_node()
                        ); // Only return shortest paths
                        if (node_reached_final != optimal_distances.end()) {
                            if (node_reached_final->second == current_state.distance + 1) {
                                return visited.add(
                                    ObjectId(iter->get_reached_node()),
                                    transition.type_id,
                                    transition.inverse,
                                    current_state.path_state
                                );
                            }
                        } else {
                            optimal_distances.insert({ iter->get_reached_node(), current_state.distance + 1 }
                            );
                            return visited.add(
                                ObjectId(iter->get_reached_node()),
                                transition.type_id,
                                transition.inverse,
                                current_state.path_state
                            );
                        }
                    }
                }
                continue;
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(
                ObjectId(iter->get_reached_node()),
                transition.type_id,
                transition.inverse,
                current_state.path_state
            );
            // Add new state to open
            auto reached_state = &open.emplace(new_visited_ptr, transition.to, current_state.distance + 1);

            // Check if new path is solution
            if (automaton.is_final_state[reached_state->automaton_state]) {
                auto node_reached_final = optimal_distances.find(iter->get_reached_node()
                ); // Only return shortest paths
                if (node_reached_final != optimal_distances.end()) {
                    if (node_reached_final->second == current_state.distance + 1) {
                        return new_visited_ptr;
                    }
                } else {
                    optimal_distances.insert({ iter->get_reached_node(), current_state.distance + 1 });
                    return new_visited_ptr;
                }
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return nullptr;
}

template<bool CYCLIC>
void BFSEnum<CYCLIC>::_reset()
{
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    optimal_distances.clear();
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state, 0);
}

template<bool CYCLIC>
void BFSEnum<CYCLIC>::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        if (stats) {
            os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
               << " reset: " << stat_reset << " results: " << results << " idx_searches: " << idx_searches
               << "]\n";
        }
    }
    os << std::string(indent, ' ') << "Paths::AllShortestSimple::BFSEnum(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}

template class Paths::AllShortestSimple::BFSEnum<true>;
template class Paths::AllShortestSimple::BFSEnum<false>;
