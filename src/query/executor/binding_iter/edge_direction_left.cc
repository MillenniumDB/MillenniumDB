#include "edge_direction_left.h"

void EdgeDirectionLeft::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

bool EdgeDirectionLeft::_next()
{
    if (!child_iter->next()) {
        return false;
    }
    parent_binding->add(direction_var, ObjectId(ObjectId::DIRECTION_LEFT));
    return true;
}

void EdgeDirectionLeft::_reset()
{
    child_iter->reset();
}

void EdgeDirectionLeft::assign_nulls()
{
    child_iter->assign_nulls();
}

void EdgeDirectionLeft::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "EdgeDirectionLeft()\n";
    child_iter->print(os, indent + 2, stats);
}
