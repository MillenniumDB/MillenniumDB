#pragma once

#include <cassert>
#include <cmath>
#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

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

        auto expr_subtype = GQL_OID::get_generic_sub_type(expr_oid);

        switch (expr_subtype) {
        case GQL_OID::GenericSubType::INTEGER:
        case GQL_OID::GenericSubType::DECIMAL:
        case GQL_OID::GenericSubType::FLOAT: {
            auto expr = GQL::Conversions::to_float(expr_oid);
            return GQL::Conversions::pack_float(cos(expr) / sin(expr));
        }
        case GQL_OID::GenericSubType::DOUBLE: {
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
};
} // namespace GQL
