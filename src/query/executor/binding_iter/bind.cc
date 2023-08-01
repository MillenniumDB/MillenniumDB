#include "bind.h"

void Bind::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
}

void Bind::reset() {
    this->child_iter->reset();
}

bool Bind::next() {
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

void Bind::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Bind(";
    os << '?' << get_query_ctx().get_var_name(var);
    os << '=' << *expr;
    os << ")\n";
    child_iter->analyze(os, indent + 2);
}
