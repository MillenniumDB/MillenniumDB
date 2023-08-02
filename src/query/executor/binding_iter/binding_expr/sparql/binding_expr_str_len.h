#pragma once

#include <codecvt>
#include <locale>
#include <memory>

#include "graph_models/object_id.h"
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
        auto oid_sub = oid.get_sub_type();

        size_t len;

        switch (oid_sub) {
        case ObjectId::MASK_STRING_SIMPLE:
        case ObjectId::MASK_STRING_XSD: {
            auto str = Conversions::to_lexical_str(oid);
            len = get_string_length(str);
            break;
        }
        case ObjectId::MASK_STRING_LANG: {
            auto [lang_id, str] = Conversions::unpack_string_lang(oid);
            len = get_string_length(str);
            break;
        }
        default:
            return ObjectId::get_null();
        }

        return Conversions::pack_int(len);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "strLEN(" << *expr << ")";
        return os;
    }

private:
    int get_string_length(const std::string& str) {
        std::locale locale("en_US.UTF-8"); // TODO: local computer may not have this locale
        std::wstring_convert<std::codecvt_utf8<wchar_t>> str_conv;
        auto wstr = str_conv.from_bytes(str);
        return wstr.length();
    }
};
} // namespace SPARQL
