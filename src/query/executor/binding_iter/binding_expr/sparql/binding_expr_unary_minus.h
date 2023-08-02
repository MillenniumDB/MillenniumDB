#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprUnaryMinus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryMinus(std::unique_ptr<BindingExpr> expr) : expr(std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.get_type()) {
        case ObjectId::MASK_NEGATIVE_INT:
            return ObjectId(((~expr_oid.id) & ObjectId::VALUE_MASK) | ObjectId::MASK_POSITIVE_INT);
        case ObjectId::MASK_POSITIVE_INT:
            return ObjectId(((~expr_oid.id) & ObjectId::VALUE_MASK) | ObjectId::MASK_NEGATIVE_INT);
        case ObjectId::MASK_FLOAT:
            return ObjectId(expr_oid.id ^ Conversions::FLOAT_SIGN_MASK);
        case ObjectId::MASK_DOUBLE_EXTERN:
        case ObjectId::MASK_DOUBLE_TMP:
            return Conversions::pack_double(-Conversions::unpack_double(expr_oid));
        case ObjectId::MASK_DECIMAL_INLINED:
            return ObjectId(expr_oid.id ^ Conversions::DECIMAL_SIGN_MASK);
        case ObjectId::MASK_DECIMAL_EXTERN:
        case ObjectId::MASK_DECIMAL_TMP: {
            auto d = Conversions::unpack_decimal_extern_tmp(expr_oid);
            return Conversions::pack_decimal(-d);
        }
        default:
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '-' << *expr;
        return os;
    }
};
} // namespace SPARQL
