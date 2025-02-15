#pragma once

#include <memory>

#include "query/id.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace Any {
struct MultiSourceSearchState {
    // The ID of the node the algorithm has reached
    const ObjectId node_id;


    // Pointer to the previous SearchState that leads to the current one
    // (used to reconstruct paths)
    const MultiSourceSearchState* previous;

    // The type of the traversed edge
    // (used to reconstruct paths)
    const ObjectId type_id;

    // State of the automaton defining the path query
    const uint32_t automaton_state;

    // Indicates in which direction the edge was traversed
    // (the language allows traversing in both directions)
    const bool inverse_direction;
    
    
    const ObjectId bfs_id;

    MultiSourceSearchState(uint32_t           automaton_state,
                ObjectId           node_id,
                const MultiSourceSearchState* previous,
                bool               inverse_direction,
                ObjectId           type_id,
                ObjectId                bfs_id) :
        node_id           (node_id),
        previous          (previous),
        type_id           (type_id),
        automaton_state   (automaton_state),
        inverse_direction (inverse_direction),
        bfs_id(bfs_id) {}

    // For ordered set
    bool operator<(const MultiSourceSearchState& other) const {
        if (automaton_state < other.automaton_state) {
            return true;
        } else if (other.automaton_state < automaton_state) {
            return false;
        } else {
            return node_id < other.node_id;
        }
    }
   
    // Overloading the ostream operator<<
    friend std::ostream& operator<<(std::ostream& os, const MultiSourceSearchState& state) {
        os << "MultiSourceSearchState: bfs_id(" << state.bfs_id << "), type_id(" << state.type_id << "), automaton_state(" << state.automaton_state << "), node_id(" << state.node_id << ")";
        return os;
    } 
    // For unordered set
    bool operator==(const MultiSourceSearchState& other) const {
        return automaton_state == other.automaton_state && node_id == other.node_id && bfs_id == other.bfs_id;
    }

    // TODO analyze
    MultiSourceSearchState clone() const {
        return MultiSourceSearchState(automaton_state, node_id, previous, inverse_direction, type_id, bfs_id);
    }

    void print(std::ostream& os,
               std::function<void(std::ostream& os, ObjectId)> print_node,
               std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
               bool begin_at_left) const;

};
}} // namespace Paths::Any

// For unordered set
template<>
struct std::hash<Paths::Any::MultiSourceSearchState> {
    std::size_t operator() (const Paths::Any::MultiSourceSearchState& lhs) const {
        return lhs.automaton_state ^ lhs.node_id.id;
    }
};

