#pragma once

#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprNot : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprNot(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        ObjectId expr_bool = GQL::Conversions::to_boolean(expr_oid);

        if (expr_bool == GQL::Conversions::pack_bool(true)) {
            return GQL::Conversions::pack_bool(false);
        } else if (expr_bool == GQL::Conversions::pack_bool(false)) {
            return GQL::Conversions::pack_bool(true);
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
        os << "NOT(";
        expr->print(os, ops);
        os << ")";
    }
};
} // namespace GQL
