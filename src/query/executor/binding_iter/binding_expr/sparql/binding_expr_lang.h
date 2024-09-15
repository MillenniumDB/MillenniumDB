#pragma once

#include <memory>
#include <sstream>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprLang : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLang(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch(RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto&& [lang, str] = Conversions::unpack_string_lang(expr_oid);
            return Conversions::pack_string_simple(lang);
        }
        case RDF_OID::GenericSubType::INTEGER:
        case RDF_OID::GenericSubType::FLOAT:
        case RDF_OID::GenericSubType::DOUBLE:
        case RDF_OID::GenericSubType::DECIMAL:
        case RDF_OID::GenericSubType::DATE:
        case RDF_OID::GenericSubType::BOOL:
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD:
        case RDF_OID::GenericSubType::STRING_DATATYPE: {
            return Conversions::pack_empty_string();
        }
        case RDF_OID::GenericSubType::NULL_ID:
        case RDF_OID::GenericSubType::BLANK:
        case RDF_OID::GenericSubType::IRI:
        case RDF_OID::GenericSubType::PATH:
            return ObjectId::get_null();
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
