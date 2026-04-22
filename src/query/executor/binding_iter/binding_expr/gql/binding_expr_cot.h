#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <cassert>
#include <cmath>
#include <memory>

namespace GQL {
class BindingExprCot : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprCot(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Int:
        case ObjectSubType::Decimal:
        case ObjectSubType::Float: {
            auto expr = GQL::Conversions::to_float(expr_oid);
            return GQL::Conversions::pack_float(cos(expr) / sin(expr));
        }
        case ObjectSubType::Double: {
            auto expr = GQL::Conversions::to_double(expr_oid);
            return GQL::Conversions::pack_double(cos(expr) / sin(expr));
        }
        default: {
            return ObjectId::get_null();
        }
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "COT(";
        expr->print(os, ops);
        os << ")";
    }
};
} // namespace GQL
