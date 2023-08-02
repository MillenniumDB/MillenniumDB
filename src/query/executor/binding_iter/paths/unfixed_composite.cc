#include "unfixed_composite.h"

#include <cassert>

#include "query/var_id.h"

using namespace Paths;

bool UnfixedComposite::set_next_starting_node() {
    while (true) {
        while (!iter->next()) {
            current_start_transition++;
            if (current_start_transition < start_transitions.size()) {
                auto type_id = start_transitions[current_start_transition].type_id.id;
                auto inverse = start_transitions[current_start_transition].inverse;
                iter = provider->get_iter(type_id, inverse);
                idx_searches++;
            } else {
                return false;
            }
        }
        // we have a possible new start, check it has not visited before
        auto possible_start = iter->get_starting_node();
        if (last_start != possible_start && visited.find(possible_start) == visited.end()) {
            last_start = possible_start;
            visited.emplace(possible_start);
            return true;
        }
    }
}


void UnfixedComposite::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    auto type_id = start_transitions[current_start_transition].type_id.id;
    auto inverse = start_transitions[current_start_transition].inverse;
    iter = provider->get_iter(type_id, inverse);
    idx_searches++;

    if (set_next_starting_node()) {
        parent_binding->add(start, ObjectId(last_start));
    } else {
        parent_binding->add(start, ObjectId::get_null());
    }
    path_enum->begin(_parent_binding);
}


bool UnfixedComposite::next() {
    while (current_start_transition < start_transitions.size()) {
        if (path_enum->next()) {
            results_found++;
            return true;
        } else {
            if (set_next_starting_node()) {
                parent_binding->add(start, ObjectId(last_start));
                path_enum->reset();
            } else {
                return false;
            }
        }
    }
    return false;
}


void UnfixedComposite::reset() {
    visited.clear();

    current_start_transition = 0;
    last_start = ObjectId::get_null().id;

    auto type_id = start_transitions[current_start_transition].type_id.id;
    auto inverse = start_transitions[current_start_transition].inverse;
    iter = provider->get_iter(type_id, inverse);
    idx_searches++;

    if (set_next_starting_node()) {
        parent_binding->add(start, ObjectId(last_start));
    } else {
        parent_binding->add(start, ObjectId::get_null());
    }
    path_enum->reset();
}


void UnfixedComposite::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::UnfixedComposite(idx_searches: " << idx_searches << ", found: " << results_found << ")\n";
    path_enum->analyze(os, indent + 2);
}
