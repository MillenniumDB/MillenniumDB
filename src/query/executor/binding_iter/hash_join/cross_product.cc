#include "cross_product.h"

#include <cassert>

void CrossProduct::begin(Binding& _parent_binding) {
    this->parent_binding = &_parent_binding;
    lhs->begin(_parent_binding);
    if (lhs->next()) {
        rhs_iter = rhs.get();
        rhs_iter->begin(_parent_binding);
    } else {
        rhs_iter = nullptr;
    }
}


bool CrossProduct::next() {
    while (true) {
        if (rhs_iter != nullptr && rhs_iter->next()) {
            found++;
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


void CrossProduct::reset() {
    rhs->reset();
    lhs->reset();

    if (lhs->next()) {
        rhs_iter = rhs.get();
    } else {
        rhs_iter = nullptr;
    }
}


void CrossProduct::assign_nulls() {
    rhs->assign_nulls();
    lhs->assign_nulls();
}


void CrossProduct::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ') << "CrossProduct(Found: " << found << ")\n";
    lhs->analyze(os, indent + 2);
    rhs->analyze(os, indent + 2);
}
