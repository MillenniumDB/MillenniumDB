#include "search_state.h"

#include <cassert>
#include <vector>

using namespace Paths::ShortestKGroupsWalks;

void PathIter::add(IterTransition* iter_transition)
{
    assert(begin != nullptr);
    end->next = iter_transition;
    end = iter_transition;

}

bool PathIter::next()
{
    assert(begin != nullptr);

    if (current == nullptr) {
        // iteration is initializing on root
        current = begin;
        if (current->previous) {
            // initialize recursively
            current->previous->path_iter.reset();
        }
        return true;
    }

    if (current->previous && current->previous->path_iter.next()) {
        // this iter does not advance, but some previous did
        return true;
    }

    current = current->next;
    if (current) {
        if (current->previous) {
            // initialize recursively
            current->previous->path_iter.reset();
        }
        return true;
    }
    return false;
}

void PathIter::reset()
{
    assert(begin != nullptr);
    current = begin;
    if (current->previous) {
        current->previous->path_iter.reset();
    }
}

void PathIter::start_enumeration()
{
    current = nullptr;
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
        for (; current_state->path_iter.current->previous != nullptr;
             current_state = current_state->path_iter.current->previous)
        {
            nodes.push_back(current_state->node_id);
            edges.push_back(current_state->path_iter.current->type_id);
            inverse_directions.push_back(current_state->path_iter.current->inverse_direction);
        }
        nodes.push_back(current_state->node_id);

        node_func(nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            edge_func(edges[i], inverse_directions[i]);
            node_func(nodes[i]);
        }
    } else {
        auto current_state = this;
        for (; current_state->path_iter.current->previous != nullptr;
            current_state = current_state->path_iter.current->previous)
        {
            node_func(current_state->node_id);
            edge_func(
                current_state->path_iter.current->type_id,
                !current_state->path_iter.current->inverse_direction
            );
        }
        node_func(current_state->node_id);
    }
}
