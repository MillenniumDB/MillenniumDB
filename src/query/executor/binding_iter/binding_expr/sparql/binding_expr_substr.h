#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "misc/transliterator.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprSubStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr_str;
    std::unique_ptr<BindingExpr> expr_start;
    // may be nullptr
    std::unique_ptr<BindingExpr> expr_length;

    BindingExprSubStr(std::unique_ptr<BindingExpr> expr_str, std::unique_ptr<BindingExpr> expr_start) :
        expr_str(std::move(expr_str)),
        expr_start(std::move(expr_start))
    { }

    BindingExprSubStr(
        std::unique_ptr<BindingExpr> expr_str,
        std::unique_ptr<BindingExpr> expr_start,
        std::unique_ptr<BindingExpr> expr_length
    ) :
        expr_str(std::move(expr_str)),
        expr_start(std::move(expr_start)),
        expr_length(std::move(expr_length))
    { }

    ObjectId eval(const Binding& binding) override
    {
        int start = 0;
        auto length = std::string::npos;

        // This function could support any numeric type for start and length
        // but this implementation only supports positive integers by now
        // Sources:
        // 1. https://www.w3.org/TR/sparql11-query/#func-substr
        // 2. https://www.w3.org/TR/xpath-functions/#func-substring

        // Start must be a positive integer
        auto start_oid = expr_start->eval(binding);
        if (start_oid.subtype() != ObjectSubType::Int) {
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
            if (length_oid.subtype() != ObjectSubType::Int) {
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
        switch (str_oid.subtype()) {
        case ObjectSubType::String: {
            std::string str = Conversions::unpack_string(str_oid);
            auto s = Transliterator::substr(str, start, length);
            return Conversions::pack_string(s);
        }
        case ObjectSubType::StringXsd: {
            std::string str = Conversions::unpack_string(str_oid);
            auto s = Transliterator::substr(str, start, length);
            return Conversions::pack_string_xsd(s);
        }
        case ObjectSubType::StringLang: {
            auto [lang, str] = Conversions::unpack_string_lang(str_oid);
            auto s = Transliterator::substr(str, start, length);
            return Conversions::pack_string_lang(lang, s);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "SUBSTR(";
        expr_str->print(os, ops);
        os << ", ";
        expr_start->print(os, ops);
        if (expr_length != nullptr) {
            os << ", ";
            expr_length->print(os, ops);
        }
        os << ')';
    }
};
} // namespace SPARQL
