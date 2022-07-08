#pragma once

#include <ostream>
#include <functional>

#include "base/ids/object_id.h"

namespace Paths { namespace AnyShortest {
/*
SearchState is the data structure used for book-keeping in standard graph search
algorithms (BFS, DFS, A*). We use these algorithms to evaluate path queries.
Typically, such an algorithm maintains a queue (BFS), stack (DFS), or priority
queue (A*) of SearchState objects, in order to track which nodes have already
been explored.
*/
struct SearchState {
    // The ID of the node the algorithm has reached
    const ObjectId node_id;

    // Pointer to the previous SearchState that leads to the current one
    // (used to reconstruct paths)
    const SearchState* previous;

    // The type of the traversed edge
    // (used to reconstruct paths)
    const ObjectId type_id;

    // State of the automaton defining the path query
    const uint32_t automaton_state;

    // Indicates in which direction the edge was traversed
    // (the language allows traversing in both directions)
    const bool inverse_direction;

    SearchState(uint32_t           automaton_state,
                ObjectId           node_id,
                const SearchState* previous,
                bool               inverse_direction,
                ObjectId           type_id) :
        node_id           (node_id),
        previous          (previous),
        type_id           (type_id),
        automaton_state   (automaton_state),
        inverse_direction (inverse_direction) {}

    // For ordered set
    bool operator<(const SearchState& other) const {
        if (automaton_state < other.automaton_state) {
            return true;
        } else if (other.automaton_state < automaton_state) {
            return false;
        } else {
            return node_id < other.node_id;
        }
    }

    // For unordered set
    bool operator==(const SearchState& other) const {
        return automaton_state == other.automaton_state && node_id == other.node_id;
    }

    SearchState duplicate() const {
        return SearchState(automaton_state, node_id, previous, inverse_direction, type_id);
    }

    void get_path(std::ostream& os) const;
};

}} // namespace Paths::AnyShortest

// For unordered set
template<>
struct std::hash<Paths::AnyShortest::SearchState> {
    std::size_t operator() (const Paths::AnyShortest::SearchState& lhs) const {
        return lhs.automaton_state ^ lhs.node_id.id;
    }
};
