#include "bfs_check.h"

#include "system/path_manager.h"

using namespace std;
using namespace Paths::Any;

void BFSCheck::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    // Construct automaton reverse transitions to allow backwards traversal
    automaton.add_reverse_connections();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Initial state: Forward traversal
    auto start_state = visited.emplace(automaton.start_state,
                                       start_object_id,
                                       nullptr,
                                       false,
                                       ObjectId::get_null(),
                                       true);
    open.push(start_state.first.operator->());
    forward_states++;

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();

    // Add final states to open and visited
    for (size_t state = 0; state < automaton.is_final_state.size(); state++) {
        if (automaton.is_final_state[state]) {
            // Final state: Backwards traversal
            auto final_state = visited.emplace(state,
                                               end_object_id,
                                               nullptr,
                                               true,
                                               ObjectId::get_null(),
                                               false);
            open.push(final_state.first.operator->());
            backward_states++;
        }
    }
}


void BFSCheck::_reset() {
    // Empty open and visited
    queue<DirectionalSearchState*> empty;
    open.swap(empty);
    forward_states = 0;
    backward_states = 0;
    visited.clear();

    first_next = true;

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();

    // Initial state: Forward traversal
    auto start_state = visited.emplace(automaton.start_state,
                                       start_object_id,
                                       nullptr,
                                       false,
                                       ObjectId::get_null(),
                                       true);
    open.push(start_state.first.operator->());
    forward_states++;

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
    iter = make_unique<NullIndexIterator>();

    // Add final states to open and visited
    for (size_t state = 0; state < automaton.is_final_state.size(); state++) {
        if (automaton.is_final_state[state]) {
            // Final state: Backwards traversal
            auto final_state = visited.emplace(state,
                                               end_object_id,
                                               nullptr,
                                               true,
                                               ObjectId::get_null(),
                                               false);
            open.push(final_state.first.operator->());
            backward_states++;
        }
    }
}


bool BFSCheck::_next() {
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state->node_id.id)) {
            queue<DirectionalSearchState*> empty;
            open.swap(empty);
            forward_states = 0;
            backward_states = 0;
            return false;
        }

        // Starting state is solution
        if (automaton.is_final_state[automaton.start_state] && current_state->node_id == end_object_id) {
            auto path_id = path_manager.set_path(current_state, path_var);
            parent_binding->add(path_var, path_id);
            queue<DirectionalSearchState*> empty;
            open.swap(empty);
            forward_states = 0;
            backward_states = 0;
            return true;
        }
    }

    while (forward_states > 0 && backward_states > 0) {  // No solutions if any direction has no more states to expand
        auto current_state = open.front();

        // Expand state in a specific direction
        const auto& connections = current_state->forward
                                ? automaton.from_to_connections
                                : automaton.reverse_connections;
        for (const auto& transition : connections[current_state->automaton_state]) {
            set_iter(transition, *current_state);

            // Explore matching states
            while (iter->next()) {
                auto next_state = DirectionalSearchState(transition.to,
                                                         ObjectId(iter->get_reached_node()),
                                                         current_state,
                                                         transition.inverse,
                                                         transition.type_id,
                                                         current_state->forward);

                auto next_state_pointer = visited.insert(next_state);

                // Check if next_state was added to visited
                if (next_state_pointer.second) {  // New state was inserted
                    open.push(next_state_pointer.first.operator->());

                    // Increase state count for current direction
                    if (current_state->forward) {
                        forward_states++;
                    } else {
                        backward_states++;
                    }
                } else {  // State already visited
                    if (next_state_pointer.first->forward != current_state->forward) {  // Both directions converge: found solution
                        // Create convergent path
                        auto fw_state = current_state;
                        auto bw_state = next_state_pointer.first.operator->();
                        if (!current_state->forward) {
                            fw_state = next_state_pointer.first.operator->();
                            bw_state = current_state;
                        }
                        auto full_path = merge_directions(fw_state, bw_state, current_state->forward == transition.inverse, transition.type_id);

                        // Return solution
                        auto path_id = path_manager.set_path(full_path, path_var);
                        parent_binding->add(path_var, path_id);
                        queue<DirectionalSearchState*> empty;
                        open.swap(empty);
                        forward_states = 0;
                        backward_states = 0;
                        return true;
                    }
                }
            }
        }

        // Decrease state count for current direction
        if (current_state->forward) {
            forward_states--;
        } else {
            backward_states--;
        }

        // Pop to visit next state
        open.pop();
    }
    return false;
}


DirectionalSearchState* BFSCheck::merge_directions(DirectionalSearchState* forward_state,
                                                   DirectionalSearchState* backward_state,
                                                   bool merge_inverse, ObjectId merge_type)
{
    auto prev = forward_state;
    auto current = backward_state;
    auto next = backward_state;
    auto prev_type = merge_type;
    auto prev_inverse = merge_inverse;
    auto current_type = current->type_id;
    auto current_inverse = current->inverse_direction;
    while (current != nullptr) {
        next = current->previous;
        current_type = current->type_id;
        current_inverse = current->inverse_direction;
        current->previous = prev;
        current->type_id = prev_type;
        current->inverse_direction = prev_inverse;
        prev = current;
        prev_type = current_type;
        prev_inverse = !current_inverse;  // Invert since the traversal was backwards
        current = next;
    }
    return prev;
}


void BFSCheck::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
