#include "bfs_enum.h"

#include <cassert>

#include "base/exceptions.h"
#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllShortest;

template <bool MULTIPLE_FINAL>
BFSEnum<MULTIPLE_FINAL>::BFSEnum(ThreadInfo* thread_info,
                 VarId       path_var,
                 Id          start,
                 VarId       end,
                 RPQ_DFA     automaton,
                 unique_ptr<IndexProvider> provider) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton),
    provider    (move(provider)) { }


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto state_inserted = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(state_inserted.first.operator->());
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
                reached_final.insert({current_state->node_id.id, 0});
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
    if (saved_state_reached != nullptr) {
enumeration:
        // Timeout
        if (__builtin_expect(!!(thread_info->interruption_requested), 0)) {
            throw InterruptedException();
        }
        if (saved_state_reached->path_iter.next()) {
            auto path_id = path_manager.set_path(saved_state_reached, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, saved_state_reached->node_id);
            results_found++;
            return true;
        } else {
            saved_state_reached = nullptr;
        }
    }

    // Enumerate
    while (open.size() > 0) {
        auto current_state = open.front();
        bool reached_final_state = expand_neighbors(current_state);

        // Start enumeration for reached solutions
        if (reached_final_state) {
            saved_state_reached->path_iter.start_enumeration();
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
bool BFSEnum<MULTIPLE_FINAL>::expand_neighbors(const SearchState* current_state) {
    // Check if this is the first time that current_state is explored
    if (iter->at_end()) {
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state->automaton_state].size() == 0) {
            return false;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];

        // Iterate over records until a final state is reached with a shortest path
        while (iter->next()) {
            SearchState next_state(ObjectId(iter->get_node()),
                                   transition.to,
                                   current_state->distance + 1);

            // Check if next state has already been visited
            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                // Consider next_state only if it has an optimal distance
                if (visited_search->distance == next_state.distance) {
                    visited_search->path_iter.add(current_state, transition.inverse, transition.type_id);

                    // Check if path is solution
                    if (automaton.is_final_state[next_state.automaton_state]) {
                        if (MULTIPLE_FINAL) {
                            auto node_reached_final = reached_final.find(next_state.node_id.id);  // must exist
                            if (node_reached_final->second == next_state.distance) {
                                saved_state_reached = visited_search.operator->();
                                return true;
                            }
                        } else {
                            saved_state_reached = visited_search.operator->();
                            return true;
                        }
                    }
                }
            } else {
                // Add state to visited and open and keep going unless it's an optimal final state
                auto reached_state = visited.emplace(ObjectId(iter->get_node()),
                                                     transition.to,
                                                     current_state->distance + 1,
                                                     current_state,
                                                     transition.inverse,
                                                     transition.type_id).first;
                open.push(reached_state.operator->());

                // Check if path is solution
                if (automaton.is_final_state[transition.to]) {
                    if (MULTIPLE_FINAL) {
                        auto node_reached_final = reached_final.find(next_state.node_id.id);
                        if (node_reached_final != reached_final.end()) {
                            if (node_reached_final->second == next_state.distance) {
                                saved_state_reached = reached_state.operator->();
                                return true;
                            }
                        } else {
                            reached_final.insert({next_state.node_id.id, next_state.distance});
                            saved_state_reached = reached_state.operator->();
                            return true;
                        }
                    } else {
                        saved_state_reached = reached_state.operator->();
                        return true;
                    }
                }
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return false;
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::set_iter(const SearchState* current_state) {
    // Get current transition object from automaton
    const auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];

    // Get iterator from custom index
    iter = provider->get_iterator(transition.type_id.id, transition.inverse, current_state->node_id.id);
    idx_searches++;
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    if (MULTIPLE_FINAL) {
        reached_final.clear();
    }
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto state_inserted = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(state_inserted.first.operator->());
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllShortest::BFSEnum(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}


template class Paths::AllShortest::BFSEnum<true>;
template class Paths::AllShortest::BFSEnum<false>;
