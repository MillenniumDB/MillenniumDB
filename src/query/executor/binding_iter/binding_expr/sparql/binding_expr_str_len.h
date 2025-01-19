#pragma once

#include <memory>

#include <unicode/unistr.h>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprStrLen : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprStrLen(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto oid = expr->eval(binding);
        size_t len;

        switch (RDF_OID::get_generic_sub_type(oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD: {
            auto str = Conversions::to_lexical_str(oid);
            len = get_string_length(str);
            break;
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto [lang_id, str] = Conversions::unpack_string_lang(oid);
            len = get_string_length(str);
            break;
        }
        default:
            return ObjectId::get_null();
        }

        return Conversions::pack_int(len);
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }

private:
    int get_string_length(const std::string& str) {
        const icu::UnicodeString icu_str(str.c_str(), "UTF-8");
        return icu_str.countChar32();
    }
};
} // namespace SPARQL
