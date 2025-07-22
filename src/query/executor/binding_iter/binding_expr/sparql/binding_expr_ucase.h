#pragma once

#include <memory>
#include <string>

#include "graph_models/rdf_model/conversions.h"
#include "misc/transliterator.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprUCase : public BindingExpr {
private:
    static std::string ucase(const std::string& str)
    {
        return Transliterator::uppercase(str);
    }

public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUCase(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (RDF_OID::get_generic_sub_type(expr_oid)) {
        case RDF_OID::GenericSubType::STRING_SIMPLE: {
            std::string str = Conversions::unpack_string(expr_oid);
            return Conversions::pack_string_simple(ucase(str));
        }
        case RDF_OID::GenericSubType::STRING_XSD: {
            std::string str = Conversions::unpack_string(expr_oid);
            return Conversions::pack_string_xsd(ucase(str));
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto&& [lang, str] = Conversions::unpack_string_lang(expr_oid);
            return Conversions::pack_string_lang(lang, ucase(str));
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "UCASE(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
