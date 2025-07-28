#include "let.h"

#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

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

    {   // scope to control printer destruction (before child_iter->print)
        BindingExprPrinter printer(os, indent, stats);

        os << std::string(indent, ' ') << "Let(";
        os << var_binding_expr[0].first << '=';
        printer.print(*var_binding_expr[0].second);

        for (std::size_t i = 0; i < var_binding_expr.size(); ++i) {
            os << ", ";
            os << var_binding_expr[i].first << '=';
            printer.print(*var_binding_expr[i].second);
        }
    }
    os << ")\n";
}
