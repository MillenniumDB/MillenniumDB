#pragma once

#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprMultiplication : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprMultiplication(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = GQL::Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case GQL::Conversions::OPTYPE_INTEGER: {
            auto lhs = GQL::Conversions::to_integer(lhs_oid);
            auto rhs = GQL::Conversions::to_integer(rhs_oid);
            return GQL::Conversions::pack_int(lhs * rhs);
        }
        case GQL::Conversions::OPTYPE_DECIMAL: {
            auto lhs = GQL::Conversions::to_decimal(lhs_oid);
            auto rhs = GQL::Conversions::to_decimal(rhs_oid);
            return GQL::Conversions::pack_decimal(lhs * rhs);
        }
        case GQL::Conversions::OPTYPE_FLOAT: {
            auto lhs = GQL::Conversions::to_float(lhs_oid);
            auto rhs = GQL::Conversions::to_float(rhs_oid);
            return GQL::Conversions::pack_float(lhs * rhs);
        }
        case GQL::Conversions::OPTYPE_DOUBLE: {
            auto lhs = GQL::Conversions::to_double(lhs_oid);
            auto rhs = GQL::Conversions::to_double(rhs_oid);
            return GQL::Conversions::pack_double(lhs * rhs);
        }
        case GQL::Conversions::OPTYPE_INVALID: {
            return ObjectId::get_null();
        }
        default: {
            assert(false);
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
