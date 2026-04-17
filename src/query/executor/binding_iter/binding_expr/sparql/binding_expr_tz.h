#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprTZ : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprTZ(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);
        switch (expr_oid.generic_type()) {
        case ObjectGenType::TemporalLiteral: {
            auto str = DateTime(expr_oid).get_tz();
            return Conversions::pack_string_simple(str);
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
        os << "TZ(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
