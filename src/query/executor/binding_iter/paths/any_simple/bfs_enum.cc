#include "bfs_enum.h"

#include <cassert>

#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnySimple;

template <bool CYCLIC>
void BFSEnum<CYCLIC>::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);
}


template <bool CYCLIC>
bool BFSEnum<CYCLIC>::next() {
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
            reached_final.insert(current_state.path_state->node_id.id);  // Return a single path per reached node
            auto path_id = path_manager.set_path(current_state.path_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state.path_state->node_id);
            results_found++;
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
            results_found++;
            return true;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }
    return false;
}


template <bool CYCLIC>
const PathState* BFSEnum<CYCLIC>::expand_neighbors(const SearchState& current_state) {
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
                    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
                    if (ObjectId(iter->get_reached_node()) == start_object_id) {
                        auto node_reached_final = reached_final.find(iter->get_reached_node());
                        if (node_reached_final == reached_final.end()) {
                            reached_final.insert(iter->get_reached_node());
                            return visited.add(ObjectId(iter->get_reached_node()),
                                               transition.type_id,
                                               transition.inverse,
                                               current_state.path_state);
                        }
                    }
                }
                continue;
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(ObjectId(iter->get_reached_node()),
                                               transition.type_id,
                                               transition.inverse,
                                               current_state.path_state);
            // Add new state to open
            auto reached_state = &open.emplace(new_visited_ptr, transition.to);

            // Check if new path is solution
            if (automaton.is_final_state[reached_state->automaton_state]) {
                auto node_reached_final = reached_final.find(reached_state->path_state->node_id.id);
                if (node_reached_final == reached_final.end()) {  // Return a single path per reached node
                    reached_final.insert(reached_state->path_state->node_id.id);
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


template <bool CYCLIC>
void BFSEnum<CYCLIC>::reset() {
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    reached_final.clear();
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);
}


template <bool CYCLIC>
void BFSEnum<CYCLIC>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    if (CYCLIC) {
        os << "Paths::AnySimple::BFSEnum<SIMPLE>(idx_searches: " << idx_searches << ", found: " << results_found << ")";
    } else {
        os << "Paths::AnySimple::BFSEnum<ACYCLIC>(idx_searches: " << idx_searches << ", found: " << results_found << ")";
    }
}


template class Paths::AnySimple::BFSEnum<true>;
template class Paths::AnySimple::BFSEnum<false>;
