#include "unfixed_composite.h"

#include <cassert>

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


void UnfixedComposite::_begin(Binding& _parent_binding) {
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
    child_iter->begin(_parent_binding);
}


bool UnfixedComposite::_next() {
    while (current_start_transition < start_transitions.size()) {
        if (child_iter->next()) {
            return true;
        } else {
            if (set_next_starting_node()) {
                parent_binding->add(start, ObjectId(last_start));
                child_iter->reset();
            } else {
                return false;
            }
        }
    }
    return false;
}


void UnfixedComposite::_reset() {
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
    child_iter->reset();
}


void UnfixedComposite::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
