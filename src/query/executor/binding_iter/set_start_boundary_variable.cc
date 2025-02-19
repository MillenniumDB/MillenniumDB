#include "set_start_boundary_variable.h"

void SetStartBoundaryVariable::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    bool found;
    VarId end_var = get_query_ctx().get_var(".end", &found);
    ObjectId start_value = (*parent_binding)[end_var];
    parent_binding->add(start_var_to_set, start_value);

    child_iter->begin(_parent_binding);
}

bool SetStartBoundaryVariable::_next()
{
    return child_iter->next();
}

void SetStartBoundaryVariable::_reset()
{
    bool found;
    VarId end_var = get_query_ctx().get_var(".end", &found);
    ObjectId start_value = (*parent_binding)[end_var];
    parent_binding->add(start_var_to_set, start_value);

    child_iter->reset();
    parent_binding->add(start_var_to_set, ObjectId::get_null());
}

void SetStartBoundaryVariable::assign_nulls()
{
    parent_binding->add(start_var_to_set, ObjectId::get_null());
    child_iter->assign_nulls();
}

void SetStartBoundaryVariable::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}
