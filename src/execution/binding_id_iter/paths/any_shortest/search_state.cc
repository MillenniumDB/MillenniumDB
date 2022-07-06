#include "search_state.h"

#include <sstream>
#include <vector>

#include "query_optimizer/quad_model/quad_model.h"

using namespace Paths::AnyShortest;

void SearchState::get_path(std::ostream& os) const {
    std::vector<std::string> left_path_string;
    std::vector<bool> left_directions;
    std::vector<ObjectId> left_labels;

    std::vector<std::string> right_path_string;
    std::vector<bool> right_directions;
    std::vector<ObjectId> right_labels;

    SearchState* current_state = const_cast<SearchState*>(this);

    while (current_state != nullptr) {
        std::stringstream ss;
        ss << quad_model.get_graph_object(current_state->node_id);
        if (current_state->inverted_path) {
            left_path_string.push_back(ss.str());
            left_directions.push_back(current_state->inverse_direction);
            left_labels.push_back(current_state->type_id);
        } else {
            right_path_string.push_back(ss.str());
            right_directions.push_back(current_state->inverse_direction);
            right_labels.push_back(current_state->type_id);

        }
        current_state = const_cast<SearchState*>(current_state->previous);
    }

    // Se imprime el camino left, que es el que viene invertido
    if (left_path_string.size() > 0) {
        os << "(" << left_path_string[0] << ")";
        if (left_directions[0]) {
            os << "<-[:" << quad_model.get_graph_object(left_labels[0]) << "]-";
        } else {
            os << "-[:" << quad_model.get_graph_object(left_labels[0]) << "]->";
        }
    }

    for (size_t i = 1; i < left_path_string.size(); i++) {
        os << "(" << left_path_string[i] << ")";
        if (left_directions[i]) {
            os << "<-[:" << quad_model.get_graph_object(left_labels[i]) << "]-";
        } else {
            os << "-[:" << quad_model.get_graph_object(left_labels[i]) << "]->";
        }

    }
    // Se imprime el camino que no esta invertido

    if (right_path_string.size() > 0) {
        os << "(" << right_path_string[right_path_string.size() - 1] << ")";
    }

    for (int i = right_path_string.size() - 2; i >= 0; i--) {
        if (right_directions[i]) {
            os << "<-[:" << quad_model.get_graph_object(right_labels[i]) << "]-";
        } else {
            os << "-[:" << quad_model.get_graph_object(right_labels[i]) << "]->";
        }
        os << "(" << right_path_string[i] << ")";
    }
}