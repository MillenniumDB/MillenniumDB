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

void SearchState::for_each(
    std::function<void(ObjectId)> node_func,
    std::function<void(ObjectId, bool)> edge_func,
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

        node_func(nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            edge_func(edges[i], inverse_directions[i]);
            node_func(nodes[i]);
        }
    } else {
        auto current_state = this;
        for (; current_state->current_transition != nullptr;
             current_state = current_state->current_transition->previous)
        {
            node_func(current_state->node_id);
            edge_func(
                current_state->current_transition->type_id,
                !current_state->current_transition->inverse_direction
            );
        }
        node_func(current_state->node_id);
    }
}
