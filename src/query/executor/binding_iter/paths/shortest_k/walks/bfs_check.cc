
#include "bfs_check.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "system/path_manager.h"

using namespace std;
using namespace Paths::ShortestKWalks;

void BFSCheck::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    expand_first_state();
}

void BFSCheck::_reset()
{
    // empty all structs
    while (!open.empty()) {
        open.pop();
    }
    visited.clear();
    iter_arena.clear();
    path_counts.clear();

    expand_first_state();
}

void BFSCheck::expand_first_state()
{
    // Add starting states to open and visited
    ObjectId start_oid = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();


    iter = make_unique<NullIndexIterator>();
    solution = nullptr;
    returned_solutions = 0;
    first_next = true;

    auto start_iter_transition = iter_arena.add(nullptr, false, ObjectId::get_null());
    auto state_inserted = visited.emplace(start_oid, automaton.start_state, 0, 1, start_iter_transition);
    open.push(state_inserted.first.operator->());

    // Check if first state is final
    auto& current_state = open.front();

    // Return false if node does not exist in the database
    if (!provider->node_exists(current_state->node_id.id)) {
        open.pop();
        return;
    }

    // Starting state is solution
    if (current_state->node_id == end_object_id) {
        if (automaton.is_final_state[automaton.start_state]) {
            auto solution_iter_transition = iter_arena.add(nullptr, false, ObjectId::get_null());
            solution = std::make_unique<Solution>(
                state_inserted.first.operator->(),
                solution_iter_transition,
                1
            );
        }
    }
}

bool BFSCheck::_next()
{
    if (first_next) {
        // prevent enumerating if first state is final
        first_next = false;
    } else if (solution != nullptr) {
        if (returned_solutions == K) {
            return false;
        } else {
enumeration:
            // Timeout
            if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
                throw InterruptedException();
            }
            if (solution->search_state.path_iter.next()) {
                auto path_id = path_manager.set_path(&solution->search_state, path_var);
                parent_binding->add(path_var, path_id);
                returned_solutions++;
                return true;
            } else {
                solution = nullptr;
                return false;
            }
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.front();

        // Enumerate reached solutions
        if (expand_neighbors(*current_state)) {
            solution->search_state.path_iter.start_enumeration();
            goto enumeration;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }

    if (solution != nullptr) {
        solution->search_state.path_iter.start_enumeration();
        goto enumeration;
    }
    return false;
}

bool BFSCheck::expand_neighbors(const SearchState& current_state)
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
            auto reached_node = iter->get_reached_node();
            NodeState current_ns(current_state.node_id.id, transition.from);
            NodeState reached_ns(reached_node, transition.to);

            auto it_paths_count = path_counts.find(reached_ns);

            size_t num_paths = 0;
            if (it_paths_count != path_counts.end()) {
                num_paths = it_paths_count->second;
            }

            if (num_paths >= K) {
                continue;
            }

            SearchState next_state(ObjectId(reached_node), transition.to, current_state.distance + 1);

            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                auto reached_state = visited_search.operator->();
                auto new_iter_transition = iter_arena
                                               .add(&current_state, transition.inverse, transition.type_id);
                reached_state->path_iter.add(new_iter_transition);
                reached_state->path_count += current_state.path_count;

                path_counts[reached_ns] += current_state.path_count;

                // Check if path is solution
                if (automaton.is_final_state[transition.to] && reached_state->node_id == end_object_id) {
                    if (solution != nullptr) {
                        if (solution->count < K) {
                            auto solution_iter_transition = iter_arena.add(
                                &current_state,
                                transition.inverse,
                                transition.type_id
                            );
                            solution->search_state.path_iter.add(solution_iter_transition);

                            solution->count += current_state.path_count;
                            if (solution->count >= K) {
                                return true;
                            }
                        }
                    } else {
                        auto solution_iter_transition = iter_arena.add(
                            &current_state,
                            transition.inverse,
                            transition.type_id
                        );
                        solution = std::make_unique<Solution>(
                            reached_state,
                            solution_iter_transition,
                            current_state.path_count
                        );

                        if (current_state.path_count >= K) {
                            return true;
                        }
                    }
                }
            } else {
                auto new_iter_transition = iter_arena
                                               .add(&current_state, transition.inverse, transition.type_id);
                // Add state to visited and open and keep going unless it's an optimal final state
                auto reached_state = visited
                                         .emplace(
                                             ObjectId(reached_node),
                                             transition.to,
                                             current_state.distance + 1,
                                             current_state.path_count,
                                             new_iter_transition
                                         )
                                         .first.
                                     operator->();
                open.push(reached_state);

                path_counts[reached_ns] += current_state.path_count;

                // Check if path is solution
                if (automaton.is_final_state[transition.to] && reached_state->node_id == end_object_id) {
                    if (solution != nullptr) {
                        if (solution->count < K) {
                            auto solution_iter_transition = iter_arena.add(
                                &current_state,
                                transition.inverse,
                                transition.type_id
                            );
                            solution->search_state.path_iter.add(solution_iter_transition);
                            solution->count += current_state.path_count;
                            if (solution->count >= K) {
                                return true;
                            }
                        }
                    } else {
                        auto solution_iter_transition = iter_arena.add(
                            &current_state,
                            transition.inverse,
                            transition.type_id
                        );
                        solution = std::make_unique<Solution>(
                            reached_state,
                            solution_iter_transition,
                            current_state.path_count
                        );

                        if (current_state.path_count >= K) {
                            return true;
                        }
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

void BFSCheck::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        if (stats) {
            os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
               << " reset: " << stat_reset << " results: " << results << " idx_searches: " << idx_searches
               << "]\n";
        }
    }
    os << std::string(indent, ' ') << "Paths::ShortestKWalks::BFSCheck(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}
