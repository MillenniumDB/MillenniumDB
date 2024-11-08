#include "bind.h"

void Bind::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
}

void Bind::_reset() {
    this->child_iter->reset();
}

bool Bind::_next() {
    if (child_iter->next()) {
        parent_binding->add(var, expr->eval(*parent_binding));
        return true;
    }
    return false;
}

void Bind::assign_nulls() {
    parent_binding->add(var, ObjectId::get_null());
    this->child_iter->assign_nulls();
}

void Bind::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
