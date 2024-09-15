#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprStrAfter : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprStrAfter(std::unique_ptr<BindingExpr> lhs,
                        std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_sub = RDF_OID::get_generic_sub_type(lhs_oid);
        auto rhs_sub = RDF_OID::get_generic_sub_type(rhs_oid);

        if (lhs_sub != RDF_OID::GenericSubType::STRING_SIMPLE &&
            lhs_sub != RDF_OID::GenericSubType::STRING_XSD &&
            lhs_sub != RDF_OID::GenericSubType::STRING_LANG)
        {
            return ObjectId::get_null();
        }

        if (rhs_sub != RDF_OID::GenericSubType::STRING_SIMPLE &&
            rhs_sub != RDF_OID::GenericSubType::STRING_XSD &&
            rhs_sub != RDF_OID::GenericSubType::STRING_LANG)
        {
            return ObjectId::get_null();
        }

        if (lhs_sub != RDF_OID::GenericSubType::STRING_LANG &&
            rhs_sub == RDF_OID::GenericSubType::STRING_LANG)
        {
            return ObjectId::get_null();
        }

        std::string lhs_str;
        std::string rhs_str;

        bool return_lang = false;
        std::string lhs_lang;

        if (lhs_sub == RDF_OID::GenericSubType::STRING_LANG &&
            rhs_sub == RDF_OID::GenericSubType::STRING_LANG)
        {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            auto [rhs_l, rhs_s] = Conversions::unpack_string_lang(rhs_oid);
            if (lhs_l != rhs_l) {
                return ObjectId::get_null();
            }
            lhs_lang = lhs_l;
            return_lang = true;
            lhs_str = std::move(lhs_s);
            rhs_str = std::move(rhs_s);
        }
        else if (lhs_sub == RDF_OID::GenericSubType::STRING_SIMPLE ||
                 lhs_sub == RDF_OID::GenericSubType::STRING_XSD)
        {
            lhs_str = Conversions::to_lexical_str(lhs_oid);
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }
        else {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            lhs_str = lhs_s;
            lhs_lang = lhs_l;
            return_lang = true;
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }

        if (rhs_str.size() == 0) {
            return lhs_oid;
        }
        if (lhs_str.size() == 0) {
            return Conversions::pack_string_simple("");
        }

        auto it = lhs_str.find(rhs_str);
        if (it != std::string::npos) {
            auto substr = lhs_str.substr(it+rhs_str.size());
            if (return_lang) {
                return Conversions::pack_string_lang(lhs_lang, substr);
            } else if (lhs_sub == RDF_OID::GenericSubType::STRING_XSD) {
                return Conversions::pack_string_xsd(substr);
            } else {
                return Conversions::pack_string_simple(substr);
            }
        }
        return Conversions::pack_string_simple("");
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
