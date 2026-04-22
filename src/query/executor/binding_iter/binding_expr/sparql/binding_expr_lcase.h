#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "misc/transliterator.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>
#include <string>

namespace SPARQL {
class BindingExprLCase : public BindingExpr {
private:
    static std::string lcase(const std::string& str)
    {
        return Transliterator::lowercase(str);
    }

public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLCase(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::String: {
            std::string str = Conversions::unpack_string(expr_oid);
            return Conversions::pack_string(lcase(str));
        }
        case ObjectSubType::StringXsd: {
            std::string str = Conversions::unpack_string(expr_oid);
            return Conversions::pack_string_xsd(lcase(str));
        }
        case ObjectSubType::StringLang: {
            auto&& [lang, str] = Conversions::unpack_string_lang(expr_oid);
            return Conversions::pack_string_lang(lang, lcase(str));
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
        os << "LCASE(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
