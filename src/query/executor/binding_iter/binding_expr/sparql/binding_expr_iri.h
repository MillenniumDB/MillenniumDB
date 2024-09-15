#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIRI : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    std::string base_iri;

    BindingExprIRI(std::unique_ptr<BindingExpr> expr, std::string base_iri) :
        expr(std::move(expr)), base_iri(base_iri) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch(RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::IRI: {
            // IRIs remain unchanged
            return expr_oid;
        }
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            // Strings are converted to IRIs
            std::string str = Conversions::unpack_string(expr_oid);
            if (str.find(':') == std::string::npos) {
                // IRI is not absolute
                if (base_iri.empty())
                    return ObjectId::get_null();
                else
                    return Conversions::pack_iri(base_iri + str);
            } else {
                // IRI is absolute
                return Conversions::pack_iri(str);
            }
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
