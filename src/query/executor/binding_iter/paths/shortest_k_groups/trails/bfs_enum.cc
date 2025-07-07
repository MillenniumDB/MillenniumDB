#include "bfs_enum.h"

#include <cassert>

#include "system/path_manager.h"

using namespace std;
using namespace Paths::ShortestKGroupsTrails;

void BFSEnum::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    expand_first_state();
}

void BFSEnum::_reset()
{
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    solutions.clear();
    pending_finals.clear();

    expand_first_state();
}

void BFSEnum::expand_first_state()
{
    iter = make_unique<NullIndexIterator>();
    reached_final_states = nullptr;

    ObjectId start_oid = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    auto start_node_visited = visited.add(start_oid, ObjectId(), ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state, 0);

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
        SolutionInfo s({ current_state.path_state }, 0, 1);
        solutions.insert({ current_state.path_state->node_id.id, s });
    }
}

bool BFSEnum::_next()
{
    if (reached_final_states != nullptr) {
result_enum:
        auto path_id = path_manager.set_path(*solution_it, path_var);
        parent_binding->add(path_var, path_id);
        parent_binding->add(end, (*solution_it)->node_id);

        solution_it++;
        if (solution_it == reached_final_states->end()) {
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
            solution_it = reached_final_states->begin();
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

        reached_final_states = &sol_it->second.path_states;
        solution_it = reached_final_states->begin();
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
            ObjectId reached_node(iter->get_reached_node());

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
            // Add new state to open
            auto reached_state_distance = current_state.distance + 1;
            open.emplace(new_visited_ptr, transition.to, reached_state_distance);

            // Check if new path is solution
            if (automaton.is_final_state[transition.to]) {
                auto sol_it = solutions.find(reached_node.id);
                if (sol_it != solutions.end()) {
                    if (sol_it->second.num_groups <= K) {
                        if (sol_it->second.last_depth == reached_state_distance) {
                            // same group
                            sol_it->second.path_states.push_back(new_visited_ptr);
                        } else if (sol_it->second.num_groups < K) {
                            // new group
                            sol_it->second.last_depth = reached_state_distance;
                            sol_it->second.num_groups++;
                            sol_it->second.path_states.push_back(new_visited_ptr);
                        } else { //sol_it->second.num_groups == K
                            // create group K+1 but don't add to solutions
                            sol_it->second.num_groups++;
                            return &sol_it->second.path_states;
                        }
                    }
                } else {
                    pending_finals.insert(new_visited_ptr->node_id.id);
                    SolutionInfo s({ new_visited_ptr }, reached_state_distance, 1);
                    solutions.insert({ iter->get_reached_node(), s });
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
    os << std::string(indent, ' ') << "Paths::ShortestKGroupsTrails::BFSEnum(path_var: " << path_var
       << ", start: " << start << ", end: " << end << ")";
}
