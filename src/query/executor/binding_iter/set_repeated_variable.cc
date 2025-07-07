#include "set_repeated_variable.h"

void SetRepeatedVariable::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    ObjectId start_value = (*parent_binding)[start_var];

    bool found;
    VarId end_var = get_query_ctx().get_var(".end", &found);
    ObjectId end_value = _parent_binding[end_var];

    if (start_value.is_null()) {
        parent_binding->add(start_var, end_value);
        written = true;
        vars_are_good = true;
    } else {
        vars_are_good = end_value == start_value;
    }

    if (vars_are_good) {
        child_iter->begin(_parent_binding);
    }
}

bool SetRepeatedVariable::_next()
{
    if (vars_are_good) {
        return child_iter->next();
    }
    return false;
}

void SetRepeatedVariable::_reset()
{
    if (vars_are_good) {
        child_iter->reset();
    }
}

void SetRepeatedVariable::assign_nulls()
{
    if (written) {
        parent_binding->add(start_var, ObjectId::get_null());
    }
    if (vars_are_good) {
        child_iter->assign_nulls();
    }
}

void SetRepeatedVariable::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "SetRepeatedVariable(";
    os << "start: " << start_var;
    os << ")\n";
    child_iter->print(os, indent + 2, stats);
}
