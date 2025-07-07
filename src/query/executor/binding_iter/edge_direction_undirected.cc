#include "edge_direction_undirected.h"

void EdgeDirectionUndirected::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

bool EdgeDirectionUndirected::_next()
{
    if (!child_iter->next()) {
        return false;
    }
    parent_binding->add(direction_var, ObjectId(ObjectId::DIRECTION_UNDIRECTED));
    return true;
}

void EdgeDirectionUndirected::_reset()
{
    child_iter->reset();
}

void EdgeDirectionUndirected::assign_nulls()
{
    child_iter->assign_nulls();
}

void EdgeDirectionUndirected::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "EdgeDirectionUndirected()\n";
    child_iter->print(os, indent + 2, stats);
}
