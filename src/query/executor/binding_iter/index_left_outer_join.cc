#include "index_left_outer_join.h"

#include <algorithm>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/empty_binding_iter.h"

using namespace std;

void IndexLeftOuterJoin::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;

    lhs->begin(parent_binding);
    if (lhs->next()) {
        must_return_null = true;
        rhs = original_rhs.get();
    } else {
        must_return_null = false;
        rhs = &empty_iter;
    }
    original_rhs->begin(parent_binding);
}


bool IndexLeftOuterJoin::_next() {
    while (true) {
        if (rhs->next()) {
            must_return_null = false;
            return true;
        } else {
            if (must_return_null) {
                for (auto var : rhs_only_vars) {
                    parent_binding->add(var, ObjectId::get_null());
                }
                must_return_null = false;
                return true;
            } else {
                if (lhs->next()) {
                    must_return_null = true;
                    rhs->reset();
                } else {
                    must_return_null = false;
                    return false;
                }
            }
        }
    }
}


void IndexLeftOuterJoin::_reset() {
    lhs->reset();
    if (lhs->next()) {
        must_return_null = true;
        rhs = original_rhs.get();
        rhs->reset();
    } else {
        must_return_null = false;
        rhs = &empty_iter;
    }
}


void IndexLeftOuterJoin::assign_nulls() {
    lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void IndexLeftOuterJoin::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
