#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprTimezone : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprTimezone(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch(RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::DATE: {
            bool error;
            auto str = DateTime(expr_oid).get_timezone(&error);
            if (error) {
                return ObjectId::get_null();
            }
            std::string datatype = "http://www.w3.org/2001/XMLSchema#dayTimeDuration";
            return Conversions::pack_string_datatype(datatype, str);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
