#include "let.h"

void Let::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
}

void Let::_reset()
{
    evaluated = false;
}

bool Let::_next()
{
    if (evaluated) {
        return false;
    }

    for (auto& [var, binding_expr] : var_binding_expr) {
        parent_binding->add(var, binding_expr->eval(*parent_binding));
    }

    evaluated = true;

    return true;
}

void Let::assign_nulls()
{
    for (const auto& [var, _] : var_binding_expr) {
        parent_binding->add(var, ObjectId::get_null());
    }
}

void Let::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }

    auto expr_printer = get_query_ctx().create_binding_expr_printer(os);

    os << std::string(indent, ' ') << "Let(";
    os << var_binding_expr[0].first << '=';
    var_binding_expr[0].second->accept_visitor(*expr_printer);

    for (std::size_t i = 0; i < var_binding_expr.size(); ++i) {
        os << ", ";
        os << var_binding_expr[i].first << '=';
        var_binding_expr[i].second->accept_visitor(*expr_printer);
    }
    os << ")\n";
}
