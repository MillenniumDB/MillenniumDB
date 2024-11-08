#pragma once

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class BindingExprTerm : public BindingExpr {
public:
    ObjectId object_id;

    BindingExprTerm(ObjectId object_id) :
        object_id (object_id) { }

    ObjectId eval(const Binding&) override {
        return object_id;
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
