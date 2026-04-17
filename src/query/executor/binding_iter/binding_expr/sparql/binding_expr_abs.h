#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprAbs : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprAbs(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Int: {
            auto i = Conversions::unpack_int(expr_oid);
            if (i >= 0) {
                return expr_oid;
            } else {
                return Conversions::pack_int(-i);
            }
        }
        case ObjectSubType::Decimal: {
            auto d = Conversions::unpack_decimal(expr_oid);
            return Conversions::pack_decimal(d.abs());
        }
        case ObjectSubType::Float: {
            auto f = Conversions::unpack_float(expr_oid);
            if (f >= 0) {
                return expr_oid;
            } else {
                return Conversions::pack_float(-f);
            }
        }
        case ObjectSubType::Double: {
            auto d = Conversions::unpack_double(expr_oid);
            if (d >= 0) {
                return expr_oid;
            } else {
                return Conversions::pack_double(-d);
            }
        }
        case ObjectSubType::TensorFloat: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.abs();
            return Conversions::pack_tensor<float>(tensor);
        }
        case ObjectSubType::TensorDouble: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.abs();
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
        os << "ABS(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
