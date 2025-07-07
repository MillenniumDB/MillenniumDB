#include "bind.h"

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
    auto expr_printer = get_query_ctx().create_binding_expr_printer(os);
    expr->accept_visitor(*expr_printer);
    os << ")\n";

    for (size_t i = 0; i < expr_printer->ops.size(); i++) {
        os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
        expr_printer->ops[i]->print(os, indent + 4, stats);
    }

    child_iter->print(os, indent + 2, stats);
}
