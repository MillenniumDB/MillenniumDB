#pragma once

#include <functional>
#include <ostream>

#include "graph_models/object_id.h"

namespace Paths { namespace Any {
/*
SearchStateDijkstra is used to represent states with an additional cost parameter,
allowing the use of algorithms such as Dijkstra.
*/
struct SearchStateDijkstra {
    // The ID of the node the algorithm has reached
    const ObjectId node_id;

    // Pointer to the previous search state that leads to the current one
    // (used to reconstruct paths)
    mutable const SearchStateDijkstra* previous;

    // The type of the traversed edge
    // (used to reconstruct paths)
    mutable ObjectId type_id;

    // Total cost of this path
    mutable uint64_t cost;

    // State of the automaton defining the path query
    const uint32_t automaton_state;

    // TODO: Relocate to somewhere else to avoid repeating
    // Whether the path is inverted
    const bool inverted_path;

    // Indicates in which direction the edge was traversed
    // (the language allows traversing in both directions)
    mutable bool inverse_direction;

    SearchStateDijkstra(uint32_t                   automaton_state,
                        ObjectId                   node_id,
                        const SearchStateDijkstra* previous,
                        bool                       inverse_direction,
                        ObjectId                   type_id,
                        uint64_t                   cost = 0,
                        bool                       inverted_path = false) :
        node_id           (node_id),
        previous          (previous),
        type_id           (type_id),
        cost              (cost),
        automaton_state   (automaton_state),
        inverted_path     (inverted_path),
        inverse_direction (inverse_direction) { }

    // For unordered set
    bool operator==(const SearchStateDijkstra& other) const {
        return automaton_state == other.automaton_state && node_id == other.node_id;
    }

    SearchStateDijkstra clone() const {
        return SearchStateDijkstra(automaton_state, node_id, previous, inverse_direction, type_id, cost, inverted_path);
    }

    void print(std::ostream& os,
               std::function<void(std::ostream& os, ObjectId)> print_node,
               std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
               bool begin_at_left) const;
};

// Represents a priority queue element for Dijkstra's algorithm
struct DijkstraQueueState {
    const SearchStateDijkstra* state;
    uint64_t cost;

    DijkstraQueueState(const SearchStateDijkstra* state,
                       uint64_t cost) :
        state (state),
        cost  (cost) {}
};

// Priority queue comparison
struct DijkstraQueueStateComp {
    bool operator() (const DijkstraQueueState& a, const DijkstraQueueState& b) {
        return b.cost < a.cost;
    }
};

}} // namespace Paths::Any

// For unordered set
template<>
struct std::hash<Paths::Any::SearchStateDijkstra> {
    std::size_t operator() (const Paths::Any::SearchStateDijkstra& lhs) const {
        return lhs.automaton_state ^ lhs.node_id.id;
    }
};

