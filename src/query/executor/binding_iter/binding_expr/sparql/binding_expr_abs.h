#pragma once

#include <memory>

#include "query/exceptions.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprAbs : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprAbs(std::unique_ptr<BindingExpr> expr) : expr(std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        if (!expr_oid.is_numeric()) {
            return ObjectId::get_null();
        }

        switch (expr_oid.get_type()) {
        case ObjectId::MASK_NEGATIVE_INT:
            return ObjectId(((~expr_oid.id) & ObjectId::VALUE_MASK) | ObjectId::MASK_POSITIVE_INT);
        case ObjectId::MASK_POSITIVE_INT:
            return expr_oid;
        case ObjectId::MASK_DECIMAL_INLINED:
            return ObjectId(expr_oid.id & (~Conversions::DECIMAL_SIGN_MASK));
        case ObjectId::MASK_DECIMAL_EXTERN:
        case ObjectId::MASK_DECIMAL_TMP: {
            auto d = Conversions::unpack_decimal(expr_oid);
            d.sign = false;
            return Conversions::pack_decimal(d);
        }
        case ObjectId::MASK_FLOAT:
            return ObjectId(expr_oid.id & (~Conversions::FLOAT_SIGN_MASK));
        case ObjectId::MASK_DOUBLE_EXTERN:
        case ObjectId::MASK_DOUBLE_TMP:
            return Conversions::pack_double(std::abs(Conversions::unpack_double(expr_oid)));
        default:
            // This should never happen.
            throw LogicException("Incorrect type for Abs");
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "ABS(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
