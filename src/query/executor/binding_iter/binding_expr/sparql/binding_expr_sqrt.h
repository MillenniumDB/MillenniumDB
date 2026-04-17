#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <cmath>
#include <memory>

namespace SPARQL {
class BindingExprSqrt : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprSqrt(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Int: {
            double f = Conversions::unpack_int(expr_oid);
            return Conversions::pack_float(std::sqrt(f));
        }
        case ObjectSubType::Decimal: {
            auto decimal = Conversions::unpack_decimal(expr_oid);
            auto d = decimal.to_double();
            return Conversions::pack_double(std::sqrt(d));
        }
        case ObjectSubType::Float: {
            auto f = Conversions::unpack_float(expr_oid);
            return Conversions::pack_float(std::sqrt(f));
        }
        case ObjectSubType::Double: {
            auto d = Conversions::to_double(expr_oid);
            return Conversions::pack_double(std::sqrt(d));
        }
        case ObjectSubType::TensorFloat: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.sqrt();
            return Conversions::pack_tensor<float>(tensor);
        }
        case ObjectSubType::TensorDouble: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.sqrt();
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
        os << "mdbfn:sqrt(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
