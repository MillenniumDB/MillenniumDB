#include "bfs_enum.h"

#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllShortestCount;

template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    // first_next = true;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_state = visited.emplace(start_object_id, automaton.start_state, 0, 1);
    open.push(start_state.first.operator->());
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

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto state_inserted = visited.emplace(start_object_id, automaton.start_state, 0, 1);
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
        if (MULTIPLE_FINAL) {
            if (automaton.is_final_state[automaton.start_state]) {
                reached_final.insert({current_state->node_id.id, ResultInfo(1, 0)});
            }
        }
    }

    while (open.size() > 0) {
        auto current_state = open.front();
        explore_neighbors(*current_state);

        // Check if current paths are solution
        if (automaton.is_final_state[current_state->automaton_state]) {
            if (MULTIPLE_FINAL) {
                auto found_reached_final = reached_final.find(current_state->node_id.id);
                if (found_reached_final != reached_final.end()) {
                    parent_binding->add(path_var, ObjectId(ObjectId::MASK_POSITIVE_INT | found_reached_final->second.count));
                    parent_binding->add(end, current_state->node_id);
                    open.pop();
                    results_found += found_reached_final->second.count;
                    reached_final.erase(found_reached_final);
                    return true;
                }
            } else {
                parent_binding->add(path_var, ObjectId(ObjectId::MASK_POSITIVE_INT | current_state->count));
                parent_binding->add(end, current_state->node_id);
                open.pop();
                results_found += current_state->count;
                return true;
            }
        }
        open.pop();
    }
    return false;
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::explore_neighbors(const SearchState& current_state) {
    for (size_t current_transition = 0;
         current_transition < automaton.from_to_connections[current_state.automaton_state].size();
         current_transition++)
    {
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Get iterator from custom index
        auto iter = provider->get_iter(transition.type_id.id, transition.inverse, current_state.node_id.id);
        idx_searches++;

        // Iterate over records while counting paths, until all neighbors of the current state are visited
        while (iter->next()) {
            SearchState next_state(ObjectId(iter->get_reached_node()),
                                   transition.to,
                                   current_state.distance + 1,
                                   current_state.count);

            // Check if next state has already been visited
            auto visited_search = visited.find(next_state);
            if (visited_search != visited.end()) {
                // Consider next_state only if it has an optimal distance
                if (visited_search->distance == next_state.distance) {
                    visited_search->count += current_state.count;

                    // Check if current paths are solution, and add them if true
                    if (MULTIPLE_FINAL) {
                        if (automaton.is_final_state[next_state.automaton_state]) {
                            auto node_reached_final = reached_final.find(next_state.node_id.id);  // must exist
                            if (node_reached_final->second.distance == next_state.distance) {
                                node_reached_final->second.count += next_state.count;
                            }
                        }
                    }
                }
            } else {
                // Add state to visited and open
                auto new_visited_state = visited.emplace(ObjectId(iter->get_reached_node()),
                                                         transition.to,
                                                         current_state.distance + 1,
                                                         current_state.count).first;
                open.push(new_visited_state.operator->());

                // Check if current paths are solution, and add them if true
                if (MULTIPLE_FINAL) {
                    if (automaton.is_final_state[transition.to]) {
                        auto node_reached_final = reached_final.find(next_state.node_id.id);
                        if (node_reached_final != reached_final.end()) {
                            if (node_reached_final->second.distance == next_state.distance) {
                                node_reached_final->second.count += next_state.count;
                            }
                        } else {
                            reached_final.insert({next_state.node_id.id, ResultInfo(next_state.count, next_state.distance)});
                        }
                    }
                }
            }
        }
    }
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllShortestCount::BFSEnum(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}


template class Paths::AllShortestCount::BFSEnum<true>;
template class Paths::AllShortestCount::BFSEnum<false>;
