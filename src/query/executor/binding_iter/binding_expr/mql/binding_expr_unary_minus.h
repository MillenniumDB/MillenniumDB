#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprUnaryMinus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryMinus(std::unique_ptr<BindingExpr> expr) : expr(std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::INTEGER: {
            auto i = SPARQL::Conversions::unpack_int(expr_oid);
            return SPARQL::Conversions::pack_int(i*-1);
        }
        case RDF_OID::GenericSubType::DECIMAL: {
            auto d = SPARQL::Conversions::unpack_decimal(expr_oid);
            d.sign = !d.sign;
            return SPARQL::Conversions::pack_decimal(d);
        }
        case RDF_OID::GenericSubType::FLOAT: {
            auto f = SPARQL::Conversions::unpack_float(expr_oid);
            return SPARQL::Conversions::pack_float(f*-1);
        }
        case RDF_OID::GenericSubType::DOUBLE: {
            auto d = SPARQL::Conversions::unpack_double(expr_oid);
            return SPARQL::Conversions::pack_double(d*-1);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace MQL
