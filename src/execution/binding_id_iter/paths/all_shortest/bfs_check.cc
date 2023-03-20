#include "bfs_check.h"

#include <cassert>

#include "base/exceptions.h"
#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllShortest;

BFSCheck::BFSCheck(ThreadInfo* thread_info,
                   VarId       path_var,
                   Id          start,
                   Id          end,
                   RPQ_DFA     automaton,
                   unique_ptr<IndexProvider> provider) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton),
    provider    (move(provider)) { }


void BFSCheck::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto start_state = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = std::holds_alternative<ObjectId>(end) ? std::get<ObjectId>(end)
                                                          : (*parent_binding)[std::get<VarId>(end)];
    // Set optimal distance as MAX by default
    optimal_distance = UINT64_MAX;
}


bool BFSCheck::next() {
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
        if (automaton.is_final_state[automaton.start_state] && current_state->node_id == end_object_id) {
            auto new_state = visited.emplace(current_state->node_id, automaton.start_state, 0);
            auto path_id = path_manager.set_path(new_state.first.operator->(), path_var);
            parent_binding->add(path_var, path_id);

            // Empty queue because the only state with 0 distance is the first state
            open.pop();
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


bool BFSCheck::expand_neighbors(const SearchState* current_state) {
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
                    if (next_state.node_id == end_object_id &&
                        automaton.is_final_state[next_state.automaton_state])
                    {
                        if (optimal_distance == next_state.distance) {
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
                if (reached_state->node_id == end_object_id
                    && automaton.is_final_state[transition.to])
                {
                    if (optimal_distance != UINT64_MAX) {
                        if (optimal_distance == next_state.distance) {
                            saved_state_reached = reached_state.operator->();
                            return true;
                        }
                    } else {
                        optimal_distance = next_state.distance;
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


void BFSCheck::set_iter(const SearchState* current_state) {
    // Get current transition object from automaton
    const auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];

    // Get iterator from custom index
    iter = provider->get_iterator(transition.type_id.id, transition.inverse, current_state->node_id.id);
    idx_searches++;
}


void BFSCheck::reset() {
    // Empty structures
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting state to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto start_state = visited.emplace(start_object_id, automaton.start_state, 0);
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = std::holds_alternative<ObjectId>(end) ? std::get<ObjectId>(end)
                                                          : (*parent_binding)[std::get<VarId>(end)];
    // Set optimal distance as MAX by default
    optimal_distance = UINT64_MAX;
}


void BFSCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllShortest::BFSCheck(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}
