#pragma once

#include <codecvt>
#include <memory>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprSubStr : public BindingExpr {
private:
    static std::string substr(const std::string& str, size_t start, size_t length) {
        // Fix offset
        if (start != 0)
            start--;
        else
            length--;
        // Fast path for empty strings
        if (str.empty() || start > str.size() || length == 0)
            return "";
        // Convert to wstring and get the substring
        // This considers the unicode multi-byte characters
        std::wstring_convert<std::codecvt_utf8<wchar_t>> str_conv;

        auto wstr = str_conv.from_bytes(str);
        auto sub_wstr = wstr.substr(start, length);
        return str_conv.to_bytes(sub_wstr);
    }

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
        size_t start  = 0;
        size_t length = std::wstring::npos;

        // This function could support any numeric type for start and length
        // but this implementation only supports positive integers by now
        // Sources:
        // 1. https://www.w3.org/TR/sparql11-query/#func-substr
        // 2. https://www.w3.org/TR/xpath-functions/#func-substring

        // Start must be a positive integer
        auto start_oid = expr_start->eval(binding);
        if (start_oid.get_type() != ObjectId::MASK_POSITIVE_INT)
            return ObjectId::get_null();
        else
            start = Conversions::unpack_positive_int(start_oid);

        // Length must be a positive integer or undefined
        auto length_oid = ObjectId::get_null();
        if (expr_length != nullptr) {
            length_oid = expr_length->eval(binding);
            if (length_oid.get_type() != ObjectId::MASK_POSITIVE_INT)
                return ObjectId::get_null();
            else
                length = Conversions::unpack_positive_int(length_oid);
        }

        auto str_oid = expr_str->eval(binding);
        switch (str_oid.get_sub_type()) {
        case ObjectId::MASK_STRING_SIMPLE: {
            std::string str = Conversions::unpack_string_simple(str_oid);
            auto s = substr(str, start, length);
            return Conversions::pack_string_simple(std::move(s));
        }
        case ObjectId::MASK_STRING_XSD: {
            std::string str = Conversions::unpack_string_xsd(str_oid);
            auto s = substr(str, start, length);
            return Conversions::pack_string_xsd(std::move(s));
        }
        case ObjectId::MASK_STRING_LANG: {
            auto [lang, str] = Conversions::unpack_string_lang(str_oid);
            auto s = substr(str, start, length);
            return Conversions::pack_string_lang(lang, std::move(s));
        }
        default:
            auto typ = str_oid.get_type();
            std::stringstream ss;
            ss << std::hex << typ;
            auto s = ss.str();
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "strAFTER(" << *expr_str << ", " << *expr_start;
        if (expr_length) {
            os << ", " << *expr_length;
        }
        os << ")";
        return os;
    }
};
} // namespace SPARQL
