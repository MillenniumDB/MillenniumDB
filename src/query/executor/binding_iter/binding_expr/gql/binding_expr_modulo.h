#pragma once

#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprModulo : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprModulo(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = GQL::Conversions::calculate_optype(lhs_oid, rhs_oid);

        if (rhs == 0) {
                return ObjectId::get_null();
            }

        switch (optype) {
        case GQL::Conversions::OPTYPE_INTEGER: {
            auto rhs = GQL::Conversions::to_integer(rhs_oid);
            auto lhs = GQL::Conversions::to_integer(lhs_oid);
            return GQL::Conversions::pack_int(lhs % rhs);
        }
        case GQL::Conversions::OPTYPE_DECIMAL: {
            auto rhs = GQL::Conversions::to_decimal(rhs_oid);
            auto lhs = GQL::Conversions::to_decimal(lhs_oid);
            return GQL::Conversions::pack_decimal(lhs % rhs);
        }
        case GQL::Conversions::OPTYPE_FLOAT: {
            return ObjectId::get_null();
        }
        case GQL::Conversions::OPTYPE_DOUBLE: {
            return ObjectId::get_null();
        }
        case GQL::Conversions::OPTYPE_INVALID: {
            return ObjectId::get_null();
        }
        default:
            throw LogicException("This should never happen");
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
