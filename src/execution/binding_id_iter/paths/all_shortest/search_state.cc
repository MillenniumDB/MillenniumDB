#include "search_state.h"

#include <cassert>

#include "query_optimizer/quad_model/quad_model.h"

using namespace Paths::AllShortest;


void PathIter::add(const SearchState* previous, bool direction, ObjectId type_id) {
    assert(begin != nullptr);
    end->next = std::make_unique<IterTransition>(previous, direction, type_id);
    end = end->next.get();
}


bool PathIter::next() {
    if (begin == nullptr) {
        return false;
    }
    if (current == nullptr) {
        // iteration is initializing
        current = begin.get();
        current->previous->path_iter.next(); // initialize recursively
        return true;
    } else {
        if (current->previous->path_iter.next()) {
            return true;
        } else {
            if (current->next == nullptr) {
                current = nullptr;
                return false;
            } else {
                current = current->next.get();
                current->previous->path_iter.next();
                return true;
            }
        }
    }
}


void PathIter::start_enumeration() {
    assert(begin != nullptr);
    if (begin->previous->path_iter.begin != nullptr) {
        current = end;
    }
}


void SearchState::get_path(std::ostream& os) const {
    std::vector<ObjectId> nodes;
    std::vector<ObjectId> types;
    std::vector<bool> directions;

    auto* current_state = this;
    while (current_state->path_iter.current != nullptr) {
        nodes.push_back(current_state->node_id);
        types.push_back(current_state->path_iter.current->type_id);
        directions.push_back(current_state->path_iter.current->inverse_direction);
        current_state = current_state->path_iter.current->previous;
    }
    nodes.push_back(current_state->node_id);  // First node in the path has no valid previous node

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
