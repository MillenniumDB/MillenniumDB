#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprUnaryMinus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryMinus(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Int: {
            auto i = Conversions::unpack_int(expr_oid);
            return Conversions::pack_int(i * -1);
        }
        case ObjectSubType::Decimal: {
            auto d = Conversions::unpack_decimal(expr_oid);
            return Conversions::pack_decimal(-d);
        }
        case ObjectSubType::Float: {
            auto f = Conversions::unpack_float(expr_oid);
            return Conversions::pack_float(f * -1);
        }
        case ObjectSubType::Double: {
            auto d = Conversions::unpack_double(expr_oid);
            return Conversions::pack_double(d * -1);
        }
        case ObjectSubType::TensorFloat: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.negate();
            return Conversions::pack_tensor<float>(tensor);
        }
        case ObjectSubType::TensorDouble: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.negate();
            return Conversions::pack_tensor<double>(tensor);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "-(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
