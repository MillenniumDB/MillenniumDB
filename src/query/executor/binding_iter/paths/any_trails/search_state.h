#pragma once

#include <ostream>
#include <vector>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"

namespace Paths { namespace AnyTrails {

// Represents a path in a recursive manner (prev_state points to previous path state)
struct PathState {
    ObjectId node_id;
    ObjectId edge_id;
    ObjectId type_id;
    bool inverse_dir;
    const PathState* prev_state;

    PathState() = default;

    PathState(ObjectId node_id,
              ObjectId edge_id,
              ObjectId type_id,
              bool     inverse_dir,
              const PathState* prev_state) :
        node_id     (node_id),
        edge_id     (edge_id),
        type_id     (type_id),
        inverse_dir (inverse_dir),
        prev_state  (prev_state) { }

    void print(std::ostream& os,
               void (*print_node) (std::ostream&, ObjectId),
               void (*print_edge) (std::ostream&, ObjectId, bool inverse),
               bool begin_at_left) const;
};

// Represents a search state to expand
struct SearchState {
    const PathState* path_state;
    uint32_t automaton_state;

    SearchState(const PathState* path_state,
                uint32_t         automaton_state) :
        path_state      (path_state),
        automaton_state (automaton_state) { }
};

// Structure that manages access & storage for visited path states
class Visited {
public:
    Visited() {
        blocks.push_back(new PathState[4096]);
    }

    ~Visited() {
        clear();
    }

    PathState* add(ObjectId node_id, ObjectId edge_id, ObjectId type_id, bool inverse_dir, const PathState* prev_state) {
        if (current_index >= 4096) {
            blocks.push_back(new PathState[4096]);
            current_index = 0;
        }

        auto& res = blocks.back()[current_index];
        res = PathState(node_id, edge_id, type_id, inverse_dir, prev_state);
        current_index++;
        return &res;
    }

    void clear() {
        for (auto block : blocks) {
            delete[](block);
        }
        blocks.clear();
    }

private:
    std::vector<PathState*> blocks;
    size_t current_index = 0;
};

// Represents a search state to expand with DFS
struct SearchStateDFS {
    uint32_t automaton_state;
    uint32_t transition;  // Current transition for the automaton state
    std::unique_ptr<EdgeIter> iter;
    ObjectId node_id;
    ObjectId edge_id;
    ObjectId type_id;
    bool inverse_dir;
    const SearchStateDFS* prev_state;

    SearchStateDFS(uint32_t                       automaton_state,
                   uint32_t                       transition,
                   std::unique_ptr<EdgeIter>      iter,
                   ObjectId                       node_id,
                   ObjectId                       edge_id,
                   ObjectId                       type_id,
                   bool                           inverse_dir,
                   const SearchStateDFS*          prev_state) :
        automaton_state (automaton_state),
        transition      (transition),
        iter            (std::move(iter)),
        node_id         (node_id),
        edge_id         (edge_id),
        type_id         (type_id),
        inverse_dir     (inverse_dir),
        prev_state      (prev_state) { }

    void print(std::ostream& os,
               void (*print_node) (std::ostream&, ObjectId),
               void (*print_edge) (std::ostream&, ObjectId, bool inverse),
               bool begin_at_left) const;
};

inline bool is_trail(const PathState* path_state, ObjectId new_edge) {
    // Iterate over path backwards
    auto current = path_state;
    while (current != nullptr) {
        // Path is not a trail (two edges are equal)
        if (current->edge_id == new_edge) {
            return false;
        }
        current = current->prev_state;
    }
    return true;
}

inline bool is_trail(const SearchStateDFS& path_state, ObjectId new_edge) {
    // Iterate over path backwards
    auto current = &path_state;
    while (current != nullptr) {
        // Path is not a trail (two edges are equal)
        if (current->edge_id == new_edge) {
            return false;
        }
        current = current->prev_state;
    }
    return true;
}

}} // namespace Paths::AnyTrails
