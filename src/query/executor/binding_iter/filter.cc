#include "filter.h"

#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

void Filter::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

bool Filter::_next()
{
    while (child_iter->next()) {
        bool pass_filters = true;
        for (auto& filter : filters) {
            auto evaluation = filter->eval(*parent_binding);
            if (!to_boolean(evaluation).is_true()) {
                pass_filters = false;
                break;
            }
        }
        if (pass_filters) {
            return true;
        }
    }
    return false;
}

void Filter::_reset()
{
    child_iter->reset();
}

void Filter::assign_nulls()
{
    child_iter->assign_nulls();
}

void Filter::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "Filter(\n";

    { // scope to control printer destruction (before child_iter->print)
        BindingExprPrinter printer(os, indent, stats);

        for (auto& filter : filters) {
            os << std::string(indent + 2, ' ');
            printer.print(*filter);
            os << "\n";
        }
    }

    os << std::string(indent, ' ') << ")\n";
    child_iter->print(os, indent + 2, stats);
}
