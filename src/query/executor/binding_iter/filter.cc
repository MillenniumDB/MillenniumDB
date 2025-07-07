#include "filter.h"

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

    auto printer = get_query_ctx().create_binding_expr_printer(os);

    for (auto& filter : filters) {
        os << std::string(indent + 2, ' ');
        filter->accept_visitor(*printer);
        os << "\n";
    }

    for (size_t i = 0; i < printer->ops.size(); i++) {
        os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
        printer->ops[i]->print(os, indent + 4, stats);
    }
    os << std::string(indent, ' ') << ")\n";
    child_iter->print(os, indent + 2, stats);
}
