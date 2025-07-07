#include "edge_direction_right.h"

void EdgeDirectionRight::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

bool EdgeDirectionRight::_next()
{
    if (!child_iter->next()) {
        return false;
    }
    parent_binding->add(direction_var, ObjectId(ObjectId::DIRECTION_RIGHT));
    return true;
}

void EdgeDirectionRight::_reset()
{
    child_iter->reset();
}

void EdgeDirectionRight::assign_nulls()
{
    child_iter->assign_nulls();
}

void EdgeDirectionRight::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "EdgeDirectionRight()\n";
    child_iter->print(os, indent + 2, stats);
}
