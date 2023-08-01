#include "search_state.h"

#include <cassert>

using namespace Paths::AllShortest;


void PathIter::add(const SearchState* previous, bool direction, ObjectId type_id) {
    assert(begin != nullptr);
    end->next = std::make_unique<IterTransition>(previous, direction, type_id);
    end = end->next.get();
}


bool PathIter::next() {
    if (begin == nullptr) {
        return false;
    }
    if (current == nullptr) {
        // iteration is initializing
        current = begin.get();
        current->previous->path_iter.next(); // initialize recursively
        return true;
    } else {
        if (current->previous->path_iter.next()) {
            return true;
        } else {
            if (current->next == nullptr) {
                current = nullptr;
                return false;
            } else {
                current = current->next.get();
                current->previous->path_iter.next();
                return true;
            }
        }
    }
}


void PathIter::start_enumeration() {
    assert(begin != nullptr);
    if (begin->previous->path_iter.begin != nullptr) {
        current = end;
    }
}


void SearchState::print(std::ostream& os,
                        void (*print_node) (std::ostream&, ObjectId),
                        void (*print_edge) (std::ostream&, ObjectId, bool inverse),
                        bool begin_at_left) const
{
    if (begin_at_left) {
        // the path need to be reconstructed in the reverse direction (last seen goes first)
        std::vector<ObjectId> nodes;
        std::vector<ObjectId> edges;
        std::vector<bool>     inverse_directions;

        auto current_state = this;
        for (;
             current_state->path_iter.current != nullptr;
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
            print_edge(os,
                       current_state->path_iter.current->type_id,
                       current_state->path_iter.current->inverse_direction);
            current_state = current_state->path_iter.current->previous;
            print_node(os, current_state->node_id);
        }
    }
}
