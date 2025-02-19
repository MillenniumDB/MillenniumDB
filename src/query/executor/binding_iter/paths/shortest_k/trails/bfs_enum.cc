#include "bfs_enum.h"

#include <cassert>

#include "system/path_manager.h"

using namespace std;
using namespace Paths::ShortestKTrails;

void BFSEnum::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    iter = make_unique<NullIndexIterator>();
    reached_final_states = nullptr;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Check if first state is final
    auto& current_state = open.front();

    // Return false if node does not exist in the database
    if (!provider->node_exists(current_state.path_state->node_id.id)) {
        open.pop();
        return;
    }

    // Starting state is solution
    if (automaton.is_final_state[automaton.start_state]) {
        pending_finals.insert(current_state.path_state->node_id.id);
        solutions.insert({ current_state.path_state->node_id.id, { current_state.path_state } });
    }
}

void BFSEnum::_reset()
{
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    iter = make_unique<NullIndexIterator>();
    reached_final_states = nullptr;
    solutions.clear();
    pending_finals.clear();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Check if first state is final
    auto& current_state = open.front();

    // Return false if node does not exist in the database
    if (!provider->node_exists(current_state.path_state->node_id.id)) {
        open.pop();
        return;
    }

    // Starting state is solution
    if (automaton.is_final_state[automaton.start_state]) {
        pending_finals.insert(current_state.path_state->node_id.id);
        solutions.insert({ current_state.path_state->node_id.id, { current_state.path_state } });
    }
}

bool BFSEnum::_next()
{
    if (reached_final_states != nullptr) {
result_enum:
        auto path_id = path_manager.set_path((*reached_final_states)[enumerating_result_i], path_var);
        parent_binding->add(path_var, path_id);
        parent_binding->add(end, (*reached_final_states)[enumerating_result_i]->node_id);

        enumerating_result_i++;
        if (enumerating_result_i == enumerating_result_last) {
            // selecting back(), but any element of the vector should have the same value
            pending_finals.erase(reached_final_states->back()->node_id.id);
            reached_final_states = nullptr;
        }
        return true;
    }

    while (open.size() > 0) {
        auto& current_state = open.front();
        reached_final_states = expand_neighbors(current_state);

        // Enumerate reached solutions
        if (reached_final_states != nullptr) {
            enumerating_result_i = 0;
            enumerating_result_last = K;
            goto result_enum;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }

    if (!pending_finals.empty()) {
        auto pending_final = pending_finals.begin();

        auto sol_it = solutions.find(*pending_final);
        assert(sol_it != solutions.end());

        reached_final_states = &sol_it->second;
        enumerating_result_i = 0;
        enumerating_result_last = reached_final_states->size();
        goto result_enum;
    }
    return false;
}

std::vector<const PathState*>* BFSEnum::expand_neighbors(const SearchState& current_state)
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

            open.emplace(new_visited_ptr, transition.to);

            // Check if new path is solution
            if (automaton.is_final_state[transition.to]) {
                auto sol_it = solutions.find(reached_node.id);
                if (sol_it != solutions.end()) {
                    if (sol_it->second.size() < K) {
                        sol_it->second.push_back(new_visited_ptr);
                        if (sol_it->second.size() == K) {
                            return &sol_it->second;
                        }
                    }
                } else {
                    pending_finals.insert(new_visited_ptr->node_id.id);
                    solutions.insert({ reached_node.id, { new_visited_ptr } });
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

void BFSEnum::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}
