#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <cassert>
#include <memory>

namespace GQL {
class BindingExprFloor : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprFloor(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        auto expr_subtype = expr_oid.subtype();
        auto expr_generic_type = expr_oid.generic_type();

        if (expr_generic_type == ObjectGenType::Numeric) {
            switch (expr_subtype) {
            case ObjectSubType::Int: {
                auto expr = GQL::Conversions::to_integer(expr_oid);
                return GQL::Conversions::pack_int(floor(expr));
            }
            case ObjectSubType::Decimal: {
                auto expr = GQL::Conversions::to_decimal(expr_oid);
                return GQL::Conversions::pack_decimal(expr.floor());
            }
            case ObjectSubType::Float: {
                auto expr = GQL::Conversions::to_float(expr_oid);
                return GQL::Conversions::pack_int(floor(expr));
            }
            case ObjectSubType::Double: {
                auto expr = GQL::Conversions::to_double(expr_oid);
                return GQL::Conversions::pack_int(floor(expr));
            }
            default: {
                assert(false);
                return ObjectId::get_null();
            }
            }
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "FLOOR(";
        expr->print(os, ops);
        os << ")";
    }
};
} // namespace GQL
