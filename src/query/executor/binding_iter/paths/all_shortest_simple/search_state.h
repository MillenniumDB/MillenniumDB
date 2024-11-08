#pragma once

#include <functional>
#include <ostream>
#include <vector>

#include "graph_models/object_id.h"

namespace Paths { namespace AllShortestSimple {

// Represents a path in a recursive manner
struct PathState {
    ObjectId         node_id;
    ObjectId         type_id;
    bool             inverse_dir;
    const PathState* prev_state;

    PathState() = default;

    PathState(ObjectId         node_id,
              ObjectId         type_id,
              bool             inverse_dir,
              const PathState* prev_state) :
        node_id     (node_id),
        type_id     (type_id),
        inverse_dir (inverse_dir),
        prev_state  (prev_state) { }

    void print(std::ostream& os,
               std::function<void(std::ostream& os, ObjectId)> print_node,
               std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
               bool begin_at_left) const;

};

// Represents a search state to expand
struct SearchState {
    const PathState* path_state;
    uint32_t         automaton_state;
    uint32_t         distance;

    SearchState(const PathState* path_state,
                uint32_t         automaton_state,
                uint32_t         distance) :
        path_state      (path_state),
        automaton_state (automaton_state),
        distance        (distance) { }
};

inline bool is_simple_path(const PathState* path_state, ObjectId new_node) {
    // Iterate over path backwards
    auto current = path_state;
    while (current != nullptr) {
        // Path is not simple (two nodes are equal)
        if (current->node_id == new_node) {
            return false;
        }
        current = current->prev_state;
    }
    return true;
}

}} // namespace Paths::AllShortestSimple
