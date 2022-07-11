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
struct SearchStateSMT {
    // The ID of the node the algorithm has reached
    const ObjectId node_id;

    // Pointer to the previous SearchState that leads to the current one
    // (used to reconstruct paths)
    const SearchStateSMT* previous;

    // The type of the traversed edge
    // (used to reconstruct paths)
    const ObjectId type_id;

    // State of the automaton defining the path query
    const uint32_t automaton_state;

    // Indicates in which direction the edge was traversed
    // (the language allows traversing in both directions)
    const bool inverse_direction;

    // TODO: add formulas
    // std::vector<std::unique_ptr<Formula>> formulas;

    // TODO: receive and assign formulas
    SearchStateSMT(uint32_t              automaton_state,
                   ObjectId              node_id,
                   const SearchStateSMT* previous,
                   bool                  inverse_direction,
                   ObjectId              type_id
                   //, std::vector<std::unique_ptr<Formula>> formulas
                   ) :
        node_id           (node_id),
        previous          (previous),
        type_id           (type_id),
        automaton_state   (automaton_state),
        inverse_direction (inverse_direction)
        // , formulas (std::move(formulas))
        { }

    // For unordered set
    bool operator==(const SearchStateSMT& other) const {
        // TODO: must compare formulas as well
        return automaton_state == other.automaton_state && node_id == other.node_id;
    }

    SearchStateSMT duplicate() const {
        return SearchStateSMT(automaton_state, node_id, previous, inverse_direction, type_id);
    }

    void get_path(std::ostream& os) const;
};

}} // namespace Paths::AnyShortest

// For unordered set
template<>
struct std::hash<Paths::AnyShortest::SearchStateSMT> {
    std::size_t operator() (const Paths::AnyShortest::SearchStateSMT& lhs) const {
        return lhs.automaton_state ^ lhs.node_id.id;
    }
};
