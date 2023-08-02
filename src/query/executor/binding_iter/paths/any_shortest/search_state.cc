#include "search_state.h"

using namespace Paths::AnyShortest;

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

        for (auto current_state = this; current_state != nullptr; current_state = current_state->previous) {
            nodes.push_back(current_state->node_id);
            edges.push_back(current_state->type_id);
            inverse_directions.push_back(current_state->inverse_direction);
        }

        print_node(os, nodes[nodes.size() - 1]);
        for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
            print_edge(os, edges[i], inverse_directions[i]);
            print_node(os, nodes[i]);
        }
    } else {
        auto current_state = this;
        print_node(os, current_state->node_id);

        while (current_state->previous != nullptr) {
            print_edge(os, current_state->type_id, current_state->inverse_direction);
            current_state = current_state->previous;
            print_node(os, current_state->node_id);
        }
    }
}
