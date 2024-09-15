#include "slice.h"

#include "query/parser/op/sparql/op_select.h"

void Slice::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
    count = 0;
    position = 0;
}


void Slice::_reset() {
    child_iter->reset();
    position = 0;
    count = 0;
}


bool Slice::_next() {
    while (position < offset) {
        if (child_iter->next()) {
            position++;
        } else {
            return false;
        }
    }

    if (count < limit && child_iter->next()) {
        count++;
        return true;
    } else {
        return false;
    }
}


void Slice::assign_nulls() {
    child_iter->assign_nulls();
}


void Slice::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
