#include "search_state.h"

#include <cassert>

using namespace Paths::AllShortest;

void SearchState::add_transition(TransitionLinkedList* iter_transition) const
{
    assert(begin != nullptr);
    end->next = iter_transition;
    end = iter_transition;
}

bool SearchState::next() const
{
    if (begin == nullptr) {
        return false;
    }
    if (current_transition == nullptr) {
        // iteration is initializing
        current_transition = begin;
        current_transition->previous->next(); // initialize recursively
        return true;
    } else {
        if (current_transition->previous->next()) {
            return true;
        } else {
            if (current_transition->next == nullptr) {
                current_transition = nullptr;
                return false;
            } else {
                current_transition = current_transition->next;
                current_transition->previous->next();
                return true;
            }
        }
    }
}

void SearchState::start_enumeration() const
{
    assert(begin != nullptr);
    assert(end != nullptr);
    current_transition = end;

    // prepares the first result
    current_transition->previous->next();
}

void SearchState::print(
    std::ostream& os,
    std::function<void(std::ostream& os, ObjectId)> print_node,
    std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
    bool begin_at_left
) const
{
    if (begin_at_left) {
        // the path need to be reconstructed in the reverse direction (last seen goes first)
        std::vector<ObjectId> nodes;
        std::vector<ObjectId> edges;
        std::vector<bool> inverse_directions;

        auto current_state = this;
        for (; current_state->current_transition != nullptr;
             current_state = current_state->current_transition->previous)
        {
            nodes.push_back(current_state->node_id);
            edges.push_back(current_state->current_transition->type_id);
            inverse_directions.push_back(current_state->current_transition->inverse_direction);
        }
        nodes.push_back(current_state->node_id);

        print_node(os, nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            print_edge(os, edges[i], inverse_directions[i]);
            print_node(os, nodes[i]);
        }
    } else {
        auto current_state = this;
        for (; current_state->current_transition != nullptr;
             current_state = current_state->current_transition->previous)
        {
            print_node(os, current_state->node_id);
            print_edge(
                os,
                current_state->current_transition->type_id,
                !current_state->current_transition->inverse_direction
            );
        }
        print_node(os, current_state->node_id);
    }
}
