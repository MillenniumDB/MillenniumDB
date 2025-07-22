#include "bind.h"

#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

void Bind::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
}

void Bind::_reset()
{
    this->child_iter->reset();
}

bool Bind::_next()
{
    if (child_iter->next()) {
        parent_binding->add(var, expr->eval(*parent_binding));
        return true;
    }
    return false;
}

void Bind::assign_nulls()
{
    parent_binding->add(var, ObjectId::get_null());
    this->child_iter->assign_nulls();
}

void Bind::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }

    os << std::string(indent, ' ') << "Bind(" << var << '=';
    {  // scope to control printer destruction (before child_iter->print)
        BindingExprPrinter printer(os, indent, stats);
        printer.print(*expr);
        os << ")\n";
    }

    child_iter->print(os, indent + 2, stats);
}
