#include "bfs_check.h"

#include "system/path_manager.h"

using namespace std;
using namespace Paths::AllShortestSimple;

template<bool CYCLIC>
void BFSCheck<CYCLIC>::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    // setted at object initialization:
    // first_next = true;
    // optimal_distance = UINT64_MAX;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state, 0);

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();
}

template<bool CYCLIC>
void BFSCheck<CYCLIC>::_reset()
{
    // Empty BFS structures
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();

    first_next = true;
    optimal_distance = UINT64_MAX;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_state = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_state, automaton.start_state, 0);

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();
}

template<bool CYCLIC>
bool BFSCheck<CYCLIC>::_next()
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
        if (current_state.path_state->node_id == end_object_id) {
            if (automaton.is_final_state[automaton.start_state]) {
                auto path_id = path_manager.set_path(current_state.path_state, path_var);
                parent_binding->add(path_var, path_id);
                queue<SearchState>
                    empty; // Empty queue because the only state with 0 distance is the first state
                open.swap(empty);
                return true;
            } else if (!CYCLIC) { // Acyclic can't have any more solutions when start node = end node
                queue<SearchState> empty;
                open.swap(empty);
                return false;
            }
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.front();
        auto reached_final_state = expand_neighbors(current_state);

        if (reached_final_state != nullptr) {
            auto path_id = path_manager.set_path(reached_final_state, path_var);
            parent_binding->add(path_var, path_id);
            return true;
        } else {
            open.pop(); // Pop and visit next state
        }
    }
    return false;
}

template<bool CYCLIC>
const PathState* BFSCheck<CYCLIC>::expand_neighbors(const SearchState& current_state)
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
                    // This case only happens if the starting node and end node are the same
                    if (start_object_id == end_object_id
                        && ObjectId(iter->get_reached_node()) == start_object_id)
                    {
                        if (optimal_distance != UINT64_MAX) { // Only return shortest paths
                            if (optimal_distance == current_state.distance + 1) {
                                return visited.add(
                                    ObjectId(iter->get_reached_node()),
                                    transition.type_id,
                                    transition.inverse,
                                    current_state.path_state
                                );
                            }
                        } else {
                            optimal_distance = current_state.distance + 1;
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

            // Special Cases: End node has been reached
            if (ObjectId(iter->get_reached_node()) == end_object_id) {
                // Return only if it's a solution, never expand
                if (automaton.is_final_state[transition.to]) {
                    if (optimal_distance != UINT64_MAX) { // Only return shortest paths
                        if (optimal_distance == current_state.distance + 1) {
                            return visited.add(
                                ObjectId(iter->get_reached_node()),
                                transition.type_id,
                                transition.inverse,
                                current_state.path_state
                            );
                        } // Else: continue
                    } else {
                        optimal_distance = current_state.distance + 1;
                        return visited.add(
                            ObjectId(iter->get_reached_node()),
                            transition.type_id,
                            transition.inverse,
                            current_state.path_state
                        );
                    }
                } // Else: continue
                continue;
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(
                ObjectId(iter->get_reached_node()),
                transition.type_id,
                transition.inverse,
                current_state.path_state
            );

            // Add new state to open if it's a potential optimal path
            // When the first optimal solution is reached, all other paths to potential solutions are already in open
            if (optimal_distance == UINT64_MAX) {
                open.emplace(new_visited_ptr, transition.to, current_state.distance + 1);
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
void BFSCheck<CYCLIC>::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        if (stats) {
            os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
               << " reset: " << stat_reset << " results: " << results << " idx_searches: " << idx_searches
               << "]\n";
        }
    }
    os << std::string(indent, ' ') << "Paths::AllShortestSimple::BFSCheck(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}

template class Paths::AllShortestSimple::BFSCheck<true>;
template class Paths::AllShortestSimple::BFSCheck<false>;
