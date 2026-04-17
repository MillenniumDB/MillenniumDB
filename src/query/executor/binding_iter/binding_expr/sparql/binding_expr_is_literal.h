#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprIsLiteral : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprIsLiteral(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.generic_type()) {
        case ObjectGenType::String:
        case ObjectGenType::Numeric:
        case ObjectGenType::Bool:
        case ObjectGenType::TemporalLiteral:
        case ObjectGenType::Tensor:
            return Conversions::pack_bool(true);
        case ObjectGenType::Null:
            return ObjectId::get_null();
        default:
            return Conversions::pack_bool(false);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "isLITERAL(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
