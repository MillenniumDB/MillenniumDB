#include "search_state.h"

using namespace Paths::ShortestKGroupsTrails;

void PathState::for_each(
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

        for (auto current_state = this; current_state != nullptr; current_state = current_state->prev_state) {
            nodes.push_back(current_state->node_id);
            edges.push_back(current_state->type_id);
            inverse_directions.push_back(current_state->inverse_dir);
        }

        node_func(nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            edge_func(edges[i], inverse_directions[i]);
            node_func(nodes[i]);
        }
    } else {
        auto current_state = this;
        node_func(current_state->node_id);

        while (current_state->prev_state != nullptr) {
            edge_func(current_state->type_id, !current_state->inverse_dir);
            current_state = current_state->prev_state;
            node_func(current_state->node_id);
        }
    }
}
