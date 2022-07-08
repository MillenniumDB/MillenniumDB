#include "search_state_dijkstra.h"

#include <vector>

#include "query_optimizer/quad_model/quad_model.h"

using namespace Paths::AnyShortest;

void SearchStateDijkstra::get_path(std::ostream& os) const {
    std::vector<ObjectId> nodes;
    std::vector<ObjectId> types;
    std::vector<bool>     directions;

    for (auto* current_state = this; current_state != nullptr; current_state = current_state->previous) {
        nodes.push_back(current_state->node_id);
        types.push_back(current_state->type_id);
        directions.push_back(current_state->inverse_direction);
    }

    os << "(" << quad_model.get_graph_object(nodes[nodes.size() - 1]) << ")";

    for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
        if (directions[i]) {
            os << "<-[:" << quad_model.get_graph_object(types[i]) << "]-";
        } else {
            os << "-[:" << quad_model.get_graph_object(types[i]) << "]->";
        }
        os << "(" << quad_model.get_graph_object(nodes[i]) << ")";
    }
}