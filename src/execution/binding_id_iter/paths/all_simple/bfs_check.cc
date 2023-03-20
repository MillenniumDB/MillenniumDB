#include "bfs_check.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllSimple;

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


bool BFSCheck::is_simple_path(const PathState* path_state, ObjectId new_node) {
    // Iterate over path backwards
    auto current = path_state;
    while (current != nullptr) {
        // Path is not simple (two nodes are equal)
        if (current->node_id == new_node) {
            return false;
        }
        current = current->prev_state;
    }
    return true;
}


void BFSCheck::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Store ID for end object
    end_object_id = std::holds_alternative<ObjectId>(end) ? std::get<ObjectId>(end)
                                                          : (*parent_binding)[std::get<VarId>(end)];
}


bool BFSCheck::next() {
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
        if (automaton.is_final_state[automaton.start_state] && current_state.path_state->node_id == end_object_id) {
            auto path_id = path_manager.set_path(current_state.path_state, path_var);
            parent_binding->add(path_var, path_id);
            results_found++;
            return true;
        }
    }

    // Enumerate
    while (open.size() > 0) {
        auto& current_state = open.front();

        // Discard states that contain the end node, since no more simple paths can be reached by expanding them
        if (current_state.path_state->node_id == end_object_id) {
            open.pop();
            continue;
        }

        // Expand to neighbors
        auto reached_final_state = expand_neighbors(current_state);

        // Enumerate reached solutions
        if (reached_final_state != nullptr) {
            auto path_id = path_manager.set_path(reached_final_state->path_state, path_var);
            parent_binding->add(path_var, path_id);
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


const SearchState* BFSCheck::expand_neighbors(const SearchState& current_state) {
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
            if (!is_simple_path(current_state.path_state, ObjectId(iter->get_node()))) {
                continue;
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(ObjectId(iter->get_node()),
                                               transition.type_id,
                                               transition.inverse,
                                               current_state.path_state);
            // Add new state to open
            auto reached_state = &open.emplace(new_visited_ptr, transition.to);

            // Check if new path is solution
            if (automaton.is_final_state[reached_state->automaton_state] &&
                reached_state->path_state->node_id == end_object_id)
            {
                return reached_state;
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


void BFSCheck::set_iter(const SearchState& current_state) {
    // Get current transition object from automaton
    const auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

    // Get iterator from custom index
    iter = provider->get_iterator(transition.type_id.id, transition.inverse, current_state.path_state->node_id.id);
    idx_searches++;
}


void BFSCheck::reset() {
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Store ID for end object
    end_object_id = std::holds_alternative<ObjectId>(end) ? std::get<ObjectId>(end)
                                                          : (*parent_binding)[std::get<VarId>(end)];
}


void BFSCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AllSimple::BFSCheck(idx_searches: " << idx_searches << ", found: " << results_found << ")";
}
