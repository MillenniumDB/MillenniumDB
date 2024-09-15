#include "sub_select.h"

void SubSelect::_begin(Binding& parent_binding_) {
    parent_binding = &parent_binding_;

    child_binding = std::make_unique<Binding>(parent_binding_.size);

    for (auto var : safe_assigned_vars) {
        (*child_binding).add(var, (*parent_binding)[var]);
    }

    child_iter->begin(*child_binding);
}


void SubSelect::_reset() {
    for (auto var : safe_assigned_vars) {
        (*child_binding).add(var, (*parent_binding)[var]);
    }
    child_iter->reset();
}


bool SubSelect::_next() {
    if (child_iter->next()) {
        for (auto var : projection_vars) {
            (*parent_binding).add(var, (*child_binding)[var]);
        }
        return true;
    } else {
        return false;
    }
}


void SubSelect::assign_nulls() {
    child_iter->assign_nulls();

    for (auto var : projection_vars) {
        (*parent_binding).add(var, (*child_binding)[var]);
    }
}


void SubSelect::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
