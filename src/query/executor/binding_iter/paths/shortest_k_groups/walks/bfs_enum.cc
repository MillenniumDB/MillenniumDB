
#include "bfs_enum.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "system/path_manager.h"

using namespace std;
using namespace Paths::ShortestKGroupsWalks;

void BFSEnum::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    expand_first_state();
}

void BFSEnum::_reset()
{
    // empty all structs
    while (!open.empty()) {
        open.pop();
    }
    visited.clear();
    iter_arena.clear();

    solutions.clear();
    groups_counts.clear();
    pending_finals.clear();

    expand_first_state();
}

void BFSEnum::expand_first_state()
{
    // Add starting states to open and visited
    ObjectId start_oid = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    iter = make_unique<NullIndexIterator>();
    current_solution = nullptr;

    auto start_iter_transition = iter_arena.add(nullptr, false, ObjectId::get_null());
    auto state_inserted = visited.emplace(start_oid, automaton.start_state, 0, start_iter_transition);
    open.push(state_inserted.first.operator->());

    // Check if first state is final
    auto& current_state = open.front();

    // Return false if node does not exist in the database
    if (!provider->node_exists(current_state->node_id.id)) {
        open.pop();
        return;
    }

    // Starting state is solution
    if (automaton.is_final_state[automaton.start_state]) {
        auto solution_iter_transition = iter_arena.add(nullptr, false, ObjectId::get_null());
        pending_finals.insert(current_state->node_id.id);
        solutions.insert({ current_state->node_id.id,
                           Solution(
                               state_inserted.first.operator->(),
                               solution_iter_transition,
                               1, // num_groups
                               0 // last_group_len
                           ) });
    }
}

bool BFSEnum::_next()
{
    if (current_solution != nullptr) {
enumeration:
        // Timeout
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        if (current_solution->search_state.path_iter.next()) {
            auto path_id = path_manager.set_path(&current_solution->search_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_solution->search_state.node_id);
            return true;
        } else {
            pending_finals.erase(current_solution->search_state.node_id.id);
            current_solution = nullptr;
            // continues to while
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.front();
        current_solution = expand_neighbors(*current_state);

        // Enumerate reached solutions
        if (current_solution != nullptr) {
            current_solution->search_state.path_iter.start_enumeration();
            goto enumeration;
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

        current_solution = &sol_it->second;
        current_solution->search_state.path_iter.start_enumeration();
        goto enumeration;
    }
    return false;
}

Solution* BFSEnum::expand_neighbors(const SearchState& current_state)
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

        // Iterate over records until a final state is reached with a shortest path
        while (iter->next()) {
            auto reached_node = iter->get_reached_node();
            NodeState current_ns(current_state.node_id.id, transition.from);
            NodeState reached_ns(reached_node, transition.to);

            SearchState next_state(ObjectId(reached_node), transition.to, current_state.distance + 1);

            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                auto reached_state = visited_search.operator->();
                auto new_iter_transition = iter_arena
                                               .add(&current_state, transition.inverse, transition.type_id);
                reached_state->path_iter.add(new_iter_transition);

                // Check if path is solution
                if (automaton.is_final_state[transition.to]) {
                    // solutions cannot be returned since we are adding paths to an existing group
                    auto sol_it = solutions.find(reached_node);
                    if (sol_it != solutions.end()) {
                        auto& solution_found = sol_it->second;
                        if (solution_found.num_groups > K) {
                            continue;
                        }

                        if (solution_found.num_groups <= K) {
                            auto solution_iter_transition = iter_arena.add(
                                &current_state,
                                transition.inverse,
                                transition.type_id
                            );
                            solution_found.search_state.path_iter.add(solution_iter_transition);
                        }
                    } else {
                        assert(false);
                    }
                    // we assume K > 1, so we need to find more solutions before returning
                }
            } else {
                auto it_groups_counts = groups_counts.find(reached_ns);
                if (it_groups_counts != groups_counts.end()) {
                    auto& groups_info = it_groups_counts->second;
                    if (groups_info.num_groups >= K) {
                        continue;
                    }
                } else {
                    // groups count will be updated below
                    groups_counts.insert({ reached_ns, GroupsInfo(0, 0) });
                }

                auto new_iter_transition = iter_arena
                                               .add(&current_state, transition.inverse, transition.type_id);
                // Add state to visited and open and keep going unless it's an optimal final state
                auto reached_state = visited
                                         .emplace(
                                             ObjectId(reached_node),
                                             transition.to,
                                             current_state.distance + 1,
                                             new_iter_transition
                                         )
                                         .first.
                                     operator->();
                open.push(reached_state);

                // update groups info
                groups_counts[reached_ns].num_groups++;
                groups_counts[reached_ns].last_group_len = reached_state->distance;

                // Check if path is solution
                if (automaton.is_final_state[transition.to]) {
                    auto sol_it = solutions.find(reached_node);
                    if (sol_it != solutions.end()) {
                        auto& solution_found = sol_it->second;
                        if (solution_found.num_groups > K) {
                            continue;
                        }

                        if (solution_found.last_group_len == reached_state->distance) {
                            // case 1: same group seen before
                            auto solution_iter_transition = iter_arena.add(
                                &current_state,
                                transition.inverse,
                                transition.type_id
                            );
                            pending_finals.insert(reached_state->node_id.id);
                            solution_found.search_state.path_iter.add(solution_iter_transition);
                        } else {
                            // case 2:
                            solution_found.num_groups++;
                            solution_found.last_group_len = reached_state->distance;
                            if (solution_found.num_groups == K + 1) {
                                return &solution_found;
                            }
                            auto solution_iter_transition = iter_arena.add(
                                &current_state,
                                transition.inverse,
                                transition.type_id
                            );
                            solution_found.search_state.path_iter.add(solution_iter_transition);
                        }
                    } else {
                        // Case 3: first time seeing reached node as solution
                        auto solution_iter_transition = iter_arena.add(
                            &current_state,
                            transition.inverse,
                            transition.type_id
                        );
                        pending_finals.insert(reached_state->node_id.id);
                        solutions.insert(
                            { reached_node,
                              Solution(reached_state, solution_iter_transition, 1, reached_state->distance) }
                        );
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

void BFSEnum::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        if (stats) {
            os << std::string(indent, ' ') << "[begin: " << stat_begin << " next: " << stat_next
               << " reset: " << stat_reset << " results: " << results << " idx_searches: " << idx_searches
               << "]\n";
        }
    }
    os << std::string(indent, ' ') << "Paths::ShortestKGroupsWalks::BFSEnum(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}
