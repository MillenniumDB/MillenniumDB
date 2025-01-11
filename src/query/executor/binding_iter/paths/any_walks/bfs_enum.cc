#include "bfs_enum.h"

#include "system/path_manager.h"

using namespace std;
using namespace Paths::Any;

template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    // first_next = true;

    // Add starting states to open and visited


    // MATI TODO What is this: parent_biding[start.get_var]? Why do we need to get objectId from starting vertex?
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto state_inserted = visited.emplace(automaton.start_state,
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::_reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    if (MULTIPLE_FINAL) {
        reached_final.clear();
    }
    first_next = true;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto state_inserted = visited.emplace(automaton.start_state,
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}


template <bool MULTIPLE_FINAL>
bool BFSEnum<MULTIPLE_FINAL>::_next() {
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
            auto reached_state = SearchState(automaton.start_state,
                                             current_state->node_id,
                                             nullptr,
                                             true,
                                             ObjectId::get_null());
            if (MULTIPLE_FINAL) {
                reached_final.insert(current_state->node_id.id);
            }
            auto path_id = path_manager.set_path(visited.insert(reached_state).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            return true;
        }
    }

    while (open.size() > 0) {
        auto current_state = open.front();
        auto reached_final_state = expand_neighbors(*current_state);

        // Enumerate reached solutions
        if (reached_final_state != nullptr) {
            auto path_id = path_manager.set_path(reached_final_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, reached_final_state->node_id);
            return true;
        } else {
            // Pop and visit next state
            open.pop();
        }
    }
    return false;
}


template <bool MULTIPLE_FINAL>
const SearchState* BFSEnum<MULTIPLE_FINAL>::expand_neighbors(const SearchState& current_state) {
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
            SearchState next_state(transition.to,
                                   ObjectId(iter->get_reached_node()),
                                   &current_state,
                                   transition.inverse,
                                   transition.type_id);
            auto visited_state = visited.insert(next_state);
            // next state = current_state tak naprawdę.
            // visited_state = next_state = current_state tak naprawdę.

            // If next state was visited for the first time
            if (visited_state.second) {
                auto reached_state = visited_state.first;
                // reached state = visited_state

                // dodajemy current_state do open
                open.push(reached_state.operator->());

                // Check if new path is solution
                if (automaton.is_final_state[reached_state->automaton_state]) {
                    if (MULTIPLE_FINAL) {
                        auto node_reached_final = reached_final.find(reached_state->node_id.id);
                        if (node_reached_final == reached_final.end()) {
                            reached_final.insert(reached_state->node_id.id);
                            return reached_state.operator->();
                        }
                    } else {
                        return reached_state.operator->();
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
    return nullptr;
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


template class Paths::Any::BFSEnum<true>;
template class Paths::Any::BFSEnum<false>;
