#include "set_end_boundary_variable.h"

void SetEndBoundaryVariable::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

bool SetEndBoundaryVariable::_next()
{
    if (child_iter->next()) {
        bool found;
        VarId end_var = get_query_ctx().get_var(".end", &found);
        ObjectId end_value = (*parent_binding)[end_var_to_set];
        parent_binding->add(end_var, end_value);
        return true;
    }
    return false;
}

void SetEndBoundaryVariable::_reset()
{
    child_iter->reset();
}

void SetEndBoundaryVariable::assign_nulls()
{
    child_iter->assign_nulls();
}

void SetEndBoundaryVariable::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "SetEndBoundaryVariable(end: " << end_var_to_set << ")\n";
    child_iter->print(os, indent + 2, stats);
}
