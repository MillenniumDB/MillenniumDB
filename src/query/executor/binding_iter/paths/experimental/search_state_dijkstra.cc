#include "search_state_dijkstra.h"

#include <vector>

using namespace Paths::Any;
using namespace std;

void SearchStateDijkstra::for_each(
    std::function<void(ObjectId)> node_func,
    std::function<void(ObjectId, bool)> edge_func,
    bool begin_at_left
) const
{
    if (begin_at_left) {
        // the path need to be reconstructed in the reverse direction (last seen goes first)
        vector<ObjectId> nodes;
        vector<ObjectId> edges;
        vector<bool>     inverse_directions;

        for (auto current_state = this; current_state != nullptr; current_state = current_state->previous) {
            nodes.push_back(current_state->node_id);
            edges.push_back(current_state->type_id);
            inverse_directions.push_back(current_state->inverse_direction);
        }

        node_func(nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            edge_func(edges[i], inverse_directions[i]);
            node_func(nodes[i]);
        }
    } else {
        auto current_state = this;
        node_func(current_state->node_id);

        while (current_state->previous != nullptr) {
            edge_func(current_state->type_id, !current_state->inverse_direction);
            current_state = current_state->previous;
            node_func(current_state->node_id);
        }
    }
}
