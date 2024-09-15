#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIsLiteral : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprIsLiteral(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_type(expr_oid)) {
            case RDF_OID::GenericType::STRING:
            case RDF_OID::GenericType::NUMERIC:
            case RDF_OID::GenericType::BOOL:
            case RDF_OID::GenericType::DATE:
                return Conversions::pack_bool(true);
            case RDF_OID::GenericType::NULL_ID:
                return ObjectId::get_null();
            default:
                return Conversions::pack_bool(false);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
