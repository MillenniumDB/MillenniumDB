#pragma once

#include "query/query_context.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class BindingExprTerm : public BindingExpr {
public:
    ObjectId object_id;

    BindingExprTerm(ObjectId object_id) :
        object_id(object_id) { }

    ObjectId eval(const Binding&) override {
        return object_id;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        return os << object_id;
    }
};
