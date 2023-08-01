#include "bfs_check.h"

#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnyShortest;

void BFSCheck::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    // first_next = true;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_state = visited.emplace(automaton.start_state,
                                       start_object_id,
                                       nullptr,
                                       true,
                                       ObjectId::get_null());
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();
}


void BFSCheck::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();

    first_next = true;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_state = visited.emplace(automaton.start_state,
                                       start_object_id,
                                       nullptr,
                                       true,
                                       ObjectId::get_null());
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();
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
            auto path_id = path_manager.set_path(current_state, path_var);
            parent_binding->add(path_var, path_id);
            queue<const SearchState*> empty;
            open.swap(empty);
            results_found++;
            return true;
        }
    }

    while (open.size() > 0) {
        auto current_state = open.front();

        // Expand state
        for (const auto& transition : automaton.from_to_connections[current_state->automaton_state]) {
            set_iter(transition, *current_state);

            // Explore matching states
            while (iter->next()) {
                auto next_state = SearchState(
                    transition.to,
                    ObjectId(iter->get_reached_node()),
                    current_state,
                    transition.inverse,
                    transition.type_id);

                auto next_state_pointer = visited.insert(next_state);

                // Check if next_state was added to visited
                if (next_state_pointer.second) {
                    open.push(next_state_pointer.first.operator->());

                    // Check if next_state is final
                    if (automaton.is_final_state[next_state.automaton_state]
                        && next_state.node_id == end_object_id)
                    {
                        auto path_id = path_manager.set_path(next_state_pointer.first.operator->(),
                                                             path_var);
                        parent_binding->add(path_var, path_id);
                        queue<const SearchState*> empty;
                        open.swap(empty);
                        results_found++;
                        return true;
                    }
                }
            }
        }

        // Pop to visit next state
        open.pop();
    }
    return false;
}


void BFSCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::Any::BFSCheck(idx_searches: " << idx_searches << ", found: " << results_found <<")";
}
