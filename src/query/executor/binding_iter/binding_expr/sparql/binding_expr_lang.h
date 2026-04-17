#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprLang : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLang(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::StringLang: {
            auto&& [lang, str] = Conversions::unpack_string_lang(expr_oid);
            return Conversions::pack_string_simple(lang);
        }
        case ObjectSubType::Int:
        case ObjectSubType::Float:
        case ObjectSubType::Double:
        case ObjectSubType::Decimal:
        case ObjectSubType::TemporalLiteral:
        case ObjectSubType::Bool:
        case ObjectSubType::String:
        case ObjectSubType::StringXsd:
        case ObjectSubType::StringDatatype: {
            return Conversions::pack_empty_string();
        }
        case ObjectSubType::Null:
        case ObjectSubType::Anon:
        case ObjectSubType::Iri:
        case ObjectSubType::Path:
        case ObjectSubType::TensorDouble:
        case ObjectSubType::TensorFloat:
            return ObjectId::get_null();
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "LANG(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
