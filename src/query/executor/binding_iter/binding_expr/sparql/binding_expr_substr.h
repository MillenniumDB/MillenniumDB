#pragma once

#include <memory>

#include "misc/transliterator.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprSubStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr_str;
    std::unique_ptr<BindingExpr> expr_start;
    // may be nullptr
    std::unique_ptr<BindingExpr> expr_length;

    BindingExprSubStr(std::unique_ptr<BindingExpr> expr_str,
                        std::unique_ptr<BindingExpr> expr_start) :
        expr_str   (std::move(expr_str)),
        expr_start (std::move(expr_start)) { }

    BindingExprSubStr(std::unique_ptr<BindingExpr> expr_str,
                        std::unique_ptr<BindingExpr> expr_start,
                        std::unique_ptr<BindingExpr> expr_length) :
        expr_str    (std::move(expr_str)),
        expr_start  (std::move(expr_start)),
        expr_length (std::move(expr_length)) { }

    ObjectId eval(const Binding& binding) override {
        int start  = 0;
        auto length = std::string::npos;

        // This function could support any numeric type for start and length
        // but this implementation only supports positive integers by now
        // Sources:
        // 1. https://www.w3.org/TR/sparql11-query/#func-substr
        // 2. https://www.w3.org/TR/xpath-functions/#func-substring

        // Start must be a positive integer
        auto start_oid = expr_start->eval(binding);
        if (RDF_OID::get_generic_sub_type(start_oid) != RDF_OID::GenericSubType::INTEGER) {
            return ObjectId::get_null();
        }
        start = Conversions::unpack_int(start_oid);
        if (start < 0) {
            return ObjectId::get_null();
        }

        // Length must be a positive integer or undefined
        ObjectId length_oid = ObjectId::get_null();
        if (expr_length != nullptr) {
            length_oid = expr_length->eval(binding);
            if (RDF_OID::get_generic_sub_type(length_oid) != RDF_OID::GenericSubType::INTEGER) {
                return ObjectId::get_null();
            }
            // dont't use directly length because is unsigned
            auto length_ = Conversions::unpack_int(length_oid);
            if (length_ < 0) {
                return ObjectId::get_null();
            } else {
                length = length_;
            }
        }

        // Fix offset (according to sparql the index of the first character in a strings is 1.)
        if (start != 0)
            start--;
        else
            length--;

        auto str_oid = expr_str->eval(binding);
        switch (RDF_OID::get_generic_sub_type(str_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            std::string str = Conversions::unpack_string(str_oid);
            auto s = Transliterator::substr(str, start, length);
            return Conversions::pack_string_simple(s);
        }
        case RDF_OID::GenericSubType::STRING_XSD: {
            std::string str = Conversions::unpack_string(str_oid);
            auto s = Transliterator::substr(str, start, length);
            return Conversions::pack_string_xsd(s);
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto [lang, str] = Conversions::unpack_string_lang(str_oid);
            auto s = Transliterator::substr(str, start, length);
            return Conversions::pack_string_lang(lang, s);
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
