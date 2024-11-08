#include "union.h"

void Union::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    iters[0]->begin(_parent_binding);
}


bool Union::_next() {
    while (current_iter < iters.size()) {
        if (iters[current_iter]->next()) {
            return true;
        } else {
            iters[current_iter]->assign_nulls();
            ++current_iter;
            if (current_iter < iters.size()) {
                iters[current_iter]->begin(*parent_binding);
            }
        }
    }
    return false;
}


void Union::_reset() {
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
    current_iter = 0;
    for (auto& iter : iters) {
        iter->reset();
    }
}


void Union::assign_nulls() {
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
}


void Union::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
