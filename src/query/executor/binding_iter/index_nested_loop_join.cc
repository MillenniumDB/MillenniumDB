#include "index_nested_loop_join.h"

#include <algorithm>

#include "query/var_id.h"
#include "query/executor/binding_iter/empty_binding_iter.h"


void IndexNestedLoopJoin::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;

    lhs->begin(parent_binding);
    if (lhs->next()) {
        rhs = original_rhs.get();
    } else {
        rhs = &empty_iter;
    }
    original_rhs->begin(parent_binding);
}

bool IndexNestedLoopJoin::_next() {
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

void IndexNestedLoopJoin::_reset() {
    lhs->reset();
    if (lhs->next()) {
        rhs = original_rhs.get();
        rhs->reset();
    } else {
        rhs = &empty_iter;
    }
}


void IndexNestedLoopJoin::assign_nulls() {
    lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void IndexNestedLoopJoin::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


template class std::unique_ptr<IndexNestedLoopJoin>;
