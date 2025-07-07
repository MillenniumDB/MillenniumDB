#pragma once

#include <memory>

#include "graph_models/gql/conversions.h"
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

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        if (rhs == 0) {
            return ObjectId::get_null();
        }

        switch (optype) {
        case Conversions::OpType::INTEGER: {
            auto rhs = Conversions::to_integer(rhs_oid);
            auto lhs = Conversions::to_integer(lhs_oid);
            return Conversions::pack_int(lhs % rhs);
        }
        case Conversions::OpType::DECIMAL:
        case Conversions::OpType::FLOAT:
        case Conversions::OpType::DOUBLE:
        case Conversions::OpType::INVALID:
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
