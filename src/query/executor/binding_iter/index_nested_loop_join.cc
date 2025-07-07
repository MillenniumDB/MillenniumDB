#include "index_nested_loop_join.h"

void IndexNestedLoopJoin::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    lhs->begin(parent_binding);
    if (lhs->next()) {
        rhs = original_rhs.get();
    } else {
        rhs = &empty_iter;
    }
    original_rhs->begin(parent_binding);
}

bool IndexNestedLoopJoin::_next()
{
    while (true) {
        if (rhs->next()) {
            return true;
        } else {
            if (lhs->next())
                rhs->reset();
            else
                return false;
        }
    }
}

void IndexNestedLoopJoin::_reset()
{
    lhs->reset();
    if (lhs->next()) {
        rhs = original_rhs.get();
        rhs->reset();
    } else {
        rhs = &empty_iter;
    }
}

void IndexNestedLoopJoin::assign_nulls()
{
    lhs->assign_nulls();
    original_rhs->assign_nulls();
}

void IndexNestedLoopJoin::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "IndexNestedLoopJoin()\n";
    lhs->print(os, indent + 2, stats);
    original_rhs->print(os, indent + 2, stats);
}
