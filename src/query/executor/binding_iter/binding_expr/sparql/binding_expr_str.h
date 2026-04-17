#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprStr(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Anon:
        case ObjectSubType::Iri:
        case ObjectSubType::String:
        case ObjectSubType::StringXsd:
        case ObjectSubType::StringLang:
        case ObjectSubType::StringDatatype:
        case ObjectSubType::Int:
        case ObjectSubType::Float:
        case ObjectSubType::Double:
        case ObjectSubType::Decimal:
        case ObjectSubType::Bool:
        case ObjectSubType::TemporalLiteral:
        case ObjectSubType::TensorDouble:
        case ObjectSubType::TensorFloat: {
            auto str = Conversions::to_lexical_str(expr_oid);
            return Conversions::pack_string_simple(str);
        }
        case ObjectSubType::Null:
        case ObjectSubType::Path:
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
        os << "STR(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
