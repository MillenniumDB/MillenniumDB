#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprNot : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprNot(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto oid = expr->eval(binding);

        if (oid == ObjectId(ObjectId::BOOL_FALSE)) {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else if (oid == ObjectId(ObjectId::BOOL_TRUE)) {
            return ObjectId(ObjectId::BOOL_FALSE);
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "!";
        expr->print(os, ops);
    }
};
} // namespace MQL
