#pragma once

#include <memory>

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprUnaryMinus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryMinus(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        auto optype = Conversions::calculate_optype(expr_oid);
        switch (optype) {
        case Conversions::OpType::INTEGER: {
            auto n = Conversions::to_integer(expr_oid);
            return Conversions::pack_int(n * -1);
        }
        case Conversions::OpType::DECIMAL: {
            auto n = Conversions::to_decimal(expr_oid);
            return Conversions::pack_decimal(-n);
        }
        case Conversions::OpType::FLOAT: {
            auto n = Conversions::to_float(expr_oid);
            return Conversions::pack_float(n * -1);
        }
        case Conversions::OpType::DOUBLE: {
            auto n = Conversions::to_double(expr_oid);
            return Conversions::pack_double(n * -1);
        }
        case Conversions::OpType::INVALID: {
            return ObjectId::get_null();
        }
        case Conversions::OpType::TENSOR_FLOAT: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.negate();
            return Conversions::pack_tensor<float>(tensor);
        }
        case Conversions::OpType::TENSOR_DOUBLE: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.negate();
            return Conversions::pack_tensor<double>(tensor);
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
} // namespace MQL
