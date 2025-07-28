#pragma once

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"

class BindingExprTerm : public BindingExpr {
public:
    ObjectId object_id;

    BindingExprTerm(ObjectId object_id) :
        object_id(object_id)
    { }

    ObjectId eval(const Binding&) override
    {
        return object_id;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << object_id;
    }
};
