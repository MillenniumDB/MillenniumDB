#include "check_repeated_variable.h"

CheckRepeatedVariable::CheckRepeatedVariable(std::unique_ptr<PathBindingIter> iter, VarId repeated_var) :
    child_iter(std::move(iter)),
    repeated_var(repeated_var)
{ }

void CheckRepeatedVariable::begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

void CheckRepeatedVariable::set_left_boundary(ObjectId boundary)
{
    ObjectId repeated_value = (*parent_binding)[repeated_var];
    vars_are_good = boundary == repeated_value;
    if (vars_are_good) {
        child_iter->set_left_boundary(boundary);
    }
}

void CheckRepeatedVariable::set_right_boundary(ObjectId boundary)
{
    ObjectId new_value = (*parent_binding)[repeated_var];
    vars_are_good = boundary == new_value;
    if (vars_are_good) {
        child_iter->set_right_boundary(boundary);
    }
}

std::unique_ptr<PathBinding> CheckRepeatedVariable::next()
{
    if (vars_are_good) {
        return child_iter->next();
    }
    return nullptr;
}

void CheckRepeatedVariable::assign_nulls()
{
    child_iter->assign_nulls();
}

void CheckRepeatedVariable::assign_empty()
{
    child_iter->assign_empty();
}

void CheckRepeatedVariable::reset()
{
    child_iter->reset();
}

void CheckRepeatedVariable::print(std::ostream& os, int indent, bool stats)
{
    os << std::string(indent, ' ') << "CheckRepeatedVariable(repeated: " << repeated_var << ")\n";
    child_iter->print(os, indent + 2, stats);
}
