#include "group_expressions.h"

using namespace std;

void GroupExpressions::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
}

void GroupExpressions::reset() {
    this->child_iter->reset();
}

bool GroupExpressions::next() {
    if (child_iter->next()) {
        for (auto& [var_id, expr] : expressions) {
            auto res = expr->eval(*parent_binding);
            parent_binding->add(var_id, res);
        }
        return true;
    }
    return false;
}

void GroupExpressions::assign_nulls() {
    for (auto& [var_id, expr] : expressions) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
    this->child_iter->assign_nulls();
}

void GroupExpressions::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "GroupExpressions(";

    for (size_t i = 0; i < expressions.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        const auto& [var, expr] = expressions[i];
        os << '?' << get_query_ctx().get_var_name(var);
        if (expr) {
            os << '=' << *expr ;
        }
    }

    os << ")\n";
    child_iter->analyze(os, indent+2);
}
