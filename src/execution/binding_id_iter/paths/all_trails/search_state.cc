#include "search_state.h"

#include "query_optimizer/quad_model/quad_model.h"

using namespace Paths::AllTrails;


void PathState::get_path(std::ostream& os) const {
    std::vector<ObjectId> nodes;
    std::vector<ObjectId> types;
    std::vector<ObjectId> edges;
    std::vector<bool> directions;

    for (auto* current_state = this; current_state != nullptr; current_state = current_state->prev_state) {
        nodes.push_back(current_state->node_id);
        types.push_back(current_state->type_id);
        edges.push_back(current_state->edge_id);
        directions.push_back(current_state->inverse_dir);
    }

    os << "(" << quad_model.get_graph_object(nodes[nodes.size() - 1]) << ")";

    for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
        if (directions[i]) {
            os << "<-[" << quad_model.get_graph_object(edges[i]) << " :" << quad_model.get_graph_object(types[i]) << "]-";
        } else {
            os << "-[" << quad_model.get_graph_object(edges[i]) << " :" << quad_model.get_graph_object(types[i]) << "]->";
        }
        os << "(" << quad_model.get_graph_object(nodes[i]) << ")";
    }
}


void SearchStateDFS::get_path(std::ostream& os) const {
    std::vector<ObjectId> nodes;
    std::vector<ObjectId> types;
    std::vector<ObjectId> edges;
    std::vector<bool> directions;

    for (auto* current_state = this; current_state != nullptr; current_state = current_state->prev_state) {
        nodes.push_back(current_state->node_id);
        types.push_back(current_state->type_id);
        edges.push_back(current_state->edge_id);
        directions.push_back(current_state->inverse_dir);
    }

    os << "(" << quad_model.get_graph_object(nodes[nodes.size() - 1]) << ")";

    for (int_fast32_t i = nodes.size() - 2; i >= 0; i--) { // don't use unsigned i, will overflow
        if (directions[i]) {
            os << "<-[" << quad_model.get_graph_object(edges[i]) << " :" << quad_model.get_graph_object(types[i]) << "]-";
        } else {
            os << "-[" << quad_model.get_graph_object(edges[i]) << " :" << quad_model.get_graph_object(types[i]) << "]->";
        }
        os << "(" << quad_model.get_graph_object(nodes[i]) << ")";
    }
}
