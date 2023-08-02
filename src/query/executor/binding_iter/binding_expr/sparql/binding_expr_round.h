#pragma once

#include <cmath>
#include <memory>

#include "query/exceptions.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprRound : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprRound(std::unique_ptr<BindingExpr> expr) : expr(std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        if (!expr_oid.is_numeric()) {
            return ObjectId::get_null();
        }

        switch (expr_oid.get_sub_type()) {
        case ObjectId::MASK_INT: {
            auto n = Conversions::unpack_int(expr_oid);
            return ObjectId(Conversions::pack_int(std::round(n)));
        }
        case ObjectId::MASK_DECIMAL: {
            auto n = Conversions::unpack_decimal(expr_oid);
            return ObjectId(Conversions::pack_decimal(n.round()));
        }
        case ObjectId::MASK_FLOAT: {
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
        case ObjectId::MASK_DOUBLE: {
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
        default:
            // This should never happen.
            throw LogicException("Incorrect type for Round");
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "ROUND(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
