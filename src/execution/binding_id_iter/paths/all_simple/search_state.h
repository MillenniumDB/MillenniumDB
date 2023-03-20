#pragma once

#include <ostream>
#include <vector>

#include "base/ids/object_id.h"
#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"

namespace Paths { namespace AllSimple {

// Represents a path in a recursive manner (prev_state points to previous path state)
struct PathState {
    ObjectId node_id;
    ObjectId type_id;
    bool inverse_dir;
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

    void get_path(std::ostream& os) const;
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

    PathState* add(ObjectId node_id, ObjectId type_id, bool inverse_dir, const PathState* prev_state) {
        if (current_index >= 4096) {
            blocks.push_back(new PathState[4096]);
            current_index = 0;
        }

        auto& res = blocks.back()[current_index];
        res = PathState(node_id, type_id, inverse_dir, prev_state);
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
    std::unique_ptr<IndexIterator> iter;  // Iterator
    ObjectId node_id;
    ObjectId type_id;
    bool inverse_dir;
    const SearchStateDFS* prev_state;

    SearchStateDFS(uint32_t                       automaton_state,
                   uint32_t                       transition,
                   std::unique_ptr<IndexIterator> iter,
                   ObjectId                       node_id,
                   ObjectId                       type_id,
                   bool                           inverse_dir,
                   const SearchStateDFS*          prev_state) :
        automaton_state (automaton_state),
        transition      (transition),
        iter            (std::move(iter)),
        node_id         (node_id),
        type_id         (type_id),
        inverse_dir     (inverse_dir),
        prev_state      (prev_state) { }

    void get_path(std::ostream& os) const;
};
}} // namespace Paths::AllSimple
