#include "set_constants.h"

void SetConstants::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    for (auto& [var, oid] : var2oid) {
        parent_binding->add(var, oid);
    }
    child_iter->begin(_parent_binding);
}

bool SetConstants::_next()
{
    return child_iter->next();
}

void SetConstants::_reset()
{
    for (auto& [var, oid] : var2oid) {
        parent_binding->add(var, oid);
    }
    child_iter->reset();
}

void SetConstants::assign_nulls()
{
    child_iter->assign_nulls();
}

void SetConstants::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "SetConstants(";

    bool first = true;
    for (auto& [var, oid] : var2oid) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }
        os << var << " " << oid;
    }

    os << ")\n";
    child_iter->print(os, indent + 2, stats);
}
