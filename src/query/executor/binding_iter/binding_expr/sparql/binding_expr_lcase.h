#pragma once

#include <codecvt>
#include <locale>
#include <memory>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprLCase : public BindingExpr {
private:
    static std::string lcase(const std::string& str) {
        std::locale locale("en_US.UTF-8"); // TODO: local computer may not have this locale
        std::wstring_convert<std::codecvt_utf8<wchar_t>> str_conv;
        auto wstr = str_conv.from_bytes(str);
        std::ctype<wchar_t> const &ctype_facet = std::use_facet<std::ctype<wchar_t> >(locale);
        for (auto& c : wstr) {
            c = ctype_facet.tolower(c);
        }
        return str_conv.to_bytes(wstr);
    }

public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLCase(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.get_sub_type()) {
        case ObjectId::MASK_STRING_SIMPLE: {
            std::string str = Conversions::unpack_string_simple(expr_oid);
            return Conversions::pack_string_simple(lcase(str));
        }
        case ObjectId::MASK_STRING_XSD: {
            std::string str = Conversions::unpack_string_xsd(expr_oid);
            return Conversions::pack_string_xsd(lcase(str));
        }
        case ObjectId::MASK_STRING_LANG: {
            auto [lang, str] = Conversions::unpack_string_lang(expr_oid);
            return Conversions::pack_string_lang(lang, lcase(str));
        }
        default:
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "LCASE(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
