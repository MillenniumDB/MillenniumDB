#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <cmath>
#include <memory>

namespace SPARQL {
class BindingExprRound : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprRound(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Int: {
            auto n = Conversions::unpack_int(expr_oid);
            return ObjectId(Conversions::pack_int(std::round(n)));
        }
        case ObjectSubType::Decimal: {
            auto n = Conversions::unpack_decimal(expr_oid);
            return ObjectId(Conversions::pack_decimal(n.round()));
        }
        case ObjectSubType::Float: {
            auto n = Conversions::unpack_float(expr_oid);
            float whole;
            auto frac = std::modf(n, &whole);
            if (frac == -0.5f) {
                // Special case where SPARQL-1.1 ROUND != std::round
                // SPARQL ROUND(-0.5f) == 0
                // std::round(-0.5f) == -1
                n = whole;
            } else {
                n = std::round(n);
            }

            return ObjectId(Conversions::pack_float(n));
        }
        case ObjectSubType::Double: {
            auto n = Conversions::unpack_double(expr_oid);
            double whole;
            auto frac = std::modf(n, &whole);
            if (frac == -0.5) {
                // Special case where SPARQL-1.1 ROUND != std::round
                // SPARQL ROUND(-0.5f) == 0
                // std::round(-0.5f) == -1
                n = whole;
            } else {
                n = std::round(n);
            }

            return ObjectId(Conversions::pack_double(n));
        }
        case ObjectSubType::TensorFloat: {
            auto tensor = Conversions::unpack_tensor<float>(expr_oid);
            tensor.sparql11_round();
            return Conversions::pack_tensor<float>(tensor);
        }
        case ObjectSubType::TensorDouble: {
            auto tensor = Conversions::unpack_tensor<double>(expr_oid);
            tensor.sparql11_round();
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
        os << "ROUND(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
