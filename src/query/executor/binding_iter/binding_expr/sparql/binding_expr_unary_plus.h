#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprUnaryPlus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryPlus(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_type(expr_oid)) {
            case RDF_OID::GenericType::NUMERIC:
            case RDF_OID::GenericType::TENSOR:
                return expr_oid;
            default:
                return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
