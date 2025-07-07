#pragma once

#include <memory>

#include "graph_models/quad_model/conversions.h"
#include "misc/tensor_oid_op.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprSubtraction : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprSubtraction(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case Conversions::OpType::INTEGER: {
            auto lhs = Conversions::to_integer(lhs_oid);
            auto rhs = Conversions::to_integer(rhs_oid);
            return Conversions::pack_int(lhs - rhs);
        }
        case Conversions::OpType::DECIMAL: {
            auto lhs = Conversions::to_decimal(lhs_oid);
            auto rhs = Conversions::to_decimal(rhs_oid);
            return Conversions::pack_decimal(lhs - rhs);
        }
        case Conversions::OpType::FLOAT: {
            auto lhs = Conversions::to_float(lhs_oid);
            auto rhs = Conversions::to_float(rhs_oid);
            return Conversions::pack_float(lhs - rhs);
        }
        case Conversions::OpType::DOUBLE: {
            auto lhs = Conversions::to_double(lhs_oid);
            auto rhs = Conversions::to_double(rhs_oid);
            return Conversions::pack_double(lhs - rhs);
        }
        case Conversions::OpType::TENSOR_FLOAT: {
            return misc::sub_oid_tensor<float>(lhs_oid, rhs_oid, Conversions::to_float);
        }
        case Conversions::OpType::TENSOR_DOUBLE: {
            return misc::sub_oid_tensor<double>(lhs_oid, rhs_oid, Conversions::to_double);
        }
        case Conversions::OpType::INVALID: {
            return ObjectId::get_null();
        }
        default:
            throw LogicException("This should never happen");
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace MQL
