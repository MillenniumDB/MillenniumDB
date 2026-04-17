#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprMinutes : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprMinutes(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.generic_type()) {
        case ObjectGenType::TemporalLiteral: {
            bool error;
            auto res = DateTime(expr_oid).get_minute(&error);
            if (error) {
                return ObjectId::get_null();
            }
            return Conversions::pack_int(res);
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
        os << "MINUTES(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
