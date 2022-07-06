#include "search_state.h"

#include <cassert>

#include "query_optimizer/quad_model/quad_model.h"

using namespace Paths::AllShortest;

void PathIter::add(const SearchState* previous, bool direction, ObjectId type_id) {
    assert(begin != nullptr);
    end->next = std::make_unique<IterTransition>(previous, direction, type_id);
    end       = end->next.get();
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
    if (begin->previous->path_iter.begin != nullptr ) {
        current = end;
    }
}


void PathIter::get_path(ObjectId node_id, std::ostream& os) const {
    if (begin == nullptr) {
        // don't have a previous transition
        os << "(" << quad_model.get_graph_object(node_id) << ")";
        return;
    }
    assert(current != nullptr);
    if (current->previous != nullptr) {
        os << "(" << quad_model.get_graph_object(node_id) << ")";
        if (current->inverse_direction) {
            os << "-[:" << quad_model.get_graph_object(current->type_id) << "]->";
        } else {
            os << "<-[:" << quad_model.get_graph_object(current->type_id) << "]-";
        }
        current->previous->path_iter.get_path(current->previous->node_id, os);
    }
}
