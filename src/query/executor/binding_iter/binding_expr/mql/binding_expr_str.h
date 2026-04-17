#pragma once

#include <memory>

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprStr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprStr(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const auto expr_oid = expr->eval(binding);

        const auto subtype = expr_oid.subtype();

        switch (subtype) {
        case ObjectSubType::String:
        case ObjectSubType::Anon:
        case ObjectSubType::NamedNode:
        case ObjectSubType::Int:
        case ObjectSubType::Decimal:
        case ObjectSubType::Float:
        case ObjectSubType::TemporalLiteral:
        case ObjectSubType::Bool:
        case ObjectSubType::Edge:
        case ObjectSubType::TensorDouble:
        case ObjectSubType::TensorFloat: {
            const auto str = Conversions::to_lexical_str(expr_oid);
            return Conversions::pack_string(str);
        }
        case ObjectSubType::Path:
        case ObjectSubType::Null:
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
} // namespace MQL
