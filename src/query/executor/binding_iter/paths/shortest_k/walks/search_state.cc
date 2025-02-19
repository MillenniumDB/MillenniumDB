#include "search_state.h"

#include <cassert>
#include <vector>

using namespace Paths::ShortestKWalks;

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
        for (; current_state->path_iter.current->previous != nullptr;
             current_state = current_state->path_iter.current->previous)
        {
            nodes.push_back(current_state->node_id);
            edges.push_back(current_state->path_iter.current->type_id);
            inverse_directions.push_back(current_state->path_iter.current->inverse_direction);
        }
        nodes.push_back(current_state->node_id);

        print_node(os, nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            print_edge(os, edges[i], inverse_directions[i]);
            print_node(os, nodes[i]);
        }
    } else {
        auto current_state = this;
        print_node(os, current_state->node_id);

        while (current_state->path_iter.current != nullptr) {
            print_edge(
                os,
                current_state->path_iter.current->type_id,
                !current_state->path_iter.current->inverse_direction
            );
            current_state = current_state->path_iter.current->previous;
            print_node(os, current_state->node_id);
        }
    }
}
