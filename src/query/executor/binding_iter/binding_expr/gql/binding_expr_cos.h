#pragma once

#include <cassert>
#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprCos : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprCos(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        auto expr_subtype = GQL_OID::get_generic_sub_type(expr_oid);

        auto expr_generic_type = GQL_OID::get_generic_type(expr_oid);

        if (expr_generic_type == GQL_OID::GenericType::NUMERIC) {
        switch (expr_subtype) {
        case GQL_OID::GenericSubType::INTEGER: {
            auto expr = GQL::Conversions::to_integer(expr_oid);
            return GQL::Conversions::pack_float(cos(expr));
        }
        case GQL_OID::GenericSubType::DECIMAL: {
            auto expr = GQL::Conversions::to_decimal(expr_oid);
            return GQL::Conversions::pack_decimal(expr.cos());
        }
        case GQL_OID::GenericSubType::FLOAT: {
            auto expr = GQL::Conversions::to_float(expr_oid);
            return GQL::Conversions::pack_float(cos(expr));
        }
        case GQL_OID::GenericSubType::DOUBLE: {
            auto expr = GQL::Conversions::to_double(expr_oid);
            return GQL::Conversions::pack_double(cos(expr));
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
};
} // namespace GQL