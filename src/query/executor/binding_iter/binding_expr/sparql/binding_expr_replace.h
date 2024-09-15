#pragma once

#include <codecvt>
#include <memory>
#include <regex>

#include "graph_models/rdf_model/conversions.h"
#include "misc/locale.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprReplace : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr1;
    std::unique_ptr<BindingExpr> expr2;
    std::unique_ptr<BindingExpr> expr3;
    std::unique_ptr<BindingExpr> expr4;
private:
    std::wstring_convert<std::codecvt_utf8<wchar_t>> str_conv;

public:
    BindingExprReplace(std::unique_ptr<BindingExpr> expr1,
                       std::unique_ptr<BindingExpr> expr2,
                       std::unique_ptr<BindingExpr> expr3,
                       std::unique_ptr<BindingExpr> expr4) :
        expr1(std::move(expr1)),
        expr2(std::move(expr2)),
        expr3(std::move(expr3)),
        expr4(std::move(expr4))
    {
        auto locale = misc::get_locale();
    }

    ObjectId eval(const Binding& binding) override {
        auto expr1_oid = expr1->eval(binding);
        auto expr2_oid = expr2->eval(binding);
        auto expr3_oid = expr3->eval(binding);

        ObjectId expr4_oid;
        if (expr4) {
            expr4_oid = expr4->eval(binding);
        }

        std::string expr2_str;
        std::string expr3_str;
        std::string expr4_str;

        if (RDF_OID::get_generic_sub_type(expr2_oid) != RDF_OID::GenericSubType::STRING_SIMPLE ||
            RDF_OID::get_generic_sub_type(expr3_oid) != RDF_OID::GenericSubType::STRING_SIMPLE)
        {
            return ObjectId::get_null();
        }
        if (expr4 != nullptr &&
            RDF_OID::get_generic_sub_type(expr4_oid) != RDF_OID::GenericSubType::STRING_SIMPLE)
        {
            return ObjectId::get_null();
        }

        auto flags = std::regex::ECMAScript;
        if (expr4) {
            expr4_str = Conversions::unpack_string(expr4_oid);
            for (auto& f : expr4_str) {
                // TODO: implement all flags
                switch (f) {
                case 'i': { flags |= std::regex::icase; break; }
                // TODO: this should be in C++17 but does not compile
                // case 'm': { flags |= std::regex::multiline ; break; }
                default: {
                    std::stringstream ss;
                    ss << "Unsupported REGEX flag: \"" << f << '"';
                    throw QuerySemanticException(ss.str());
                }
                }
            }
        }

        expr2_str = Conversions::unpack_string(expr2_oid);
        expr3_str = Conversions::unpack_string(expr3_oid);

        // TODO: avoid expensive regex compilation.
        // - maybe add caching for regexes.
        // - if the regex is a simple string and not a variable recompiling for each eval is not necessary.
        // TODO: make sure regex behaves as in the standard.
        std::wstring key = std_string_to_wstring(expr2_str);
        std::wregex reg(key, flags);

        switch (RDF_OID::get_generic_sub_type(expr1_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD: {
            auto expr1_str = Conversions::unpack_string(expr1_oid);
            auto res = replace(expr1_str, reg, expr3_str);
            return Conversions::pack_string_simple(res);
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto&& [lang, expr1_str] = Conversions::unpack_string_lang(expr1_oid);
            auto res = replace(expr1_str, reg, expr3_str);
            return Conversions::pack_string_lang(lang, res);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }

private:
    std::wstring std_string_to_wstring(std::string& str) {
        return str_conv.from_bytes(str);
    }

    std::string wstring_to_std_string(std::wstring& wstr) {
        return str_conv.to_bytes(wstr);
    }

    std::string replace(std::string data_str,
                        std::wregex reg,
                        std::string str_replacement)
    {
        auto data_wstr = std_string_to_wstring(data_str);
        auto wstr_replacement = std_string_to_wstring(str_replacement);
        auto wres = std::regex_replace(data_wstr, reg, wstr_replacement);
        return wstring_to_std_string(wres);
    }
};
} // namespace SPARQL
