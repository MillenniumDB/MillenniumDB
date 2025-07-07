#include "cross_product.h"

void CrossProduct::_begin(Binding& _parent_binding)
{
    this->parent_binding = &_parent_binding;
    lhs->begin(_parent_binding);
    if (lhs->next()) {
        rhs_iter = rhs.get();
        rhs_iter->begin(_parent_binding);
    } else {
        rhs_iter = nullptr;
    }
}

bool CrossProduct::_next()
{
    while (true) {
        if (rhs_iter != nullptr && rhs_iter->next()) {
            return true;
        } else {
            if (lhs->next()) {
                rhs_iter->reset();
            } else {
                return false;
            }
        }
    }
}

void CrossProduct::_reset()
{
    rhs->reset();
    lhs->reset();

    if (lhs->next()) {
        rhs_iter = rhs.get();
    } else {
        rhs_iter = nullptr;
    }
}

void CrossProduct::assign_nulls()
{
    rhs->assign_nulls();
    lhs->assign_nulls();
}

void CrossProduct::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "CrossProduct()\n";
    lhs->print(os, indent + 2, stats);
    rhs->print(os, indent + 2, stats);
}
