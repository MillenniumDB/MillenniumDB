#pragma once

#include <memory>
#include <vector>

#include "graph_models/rdf_model/comparisons.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIn : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs_expr;

    std::vector<std::unique_ptr<BindingExpr>> exprs;

    BindingExprIn(std::unique_ptr<BindingExpr> lhs_expr, std::vector<std::unique_ptr<BindingExpr>> exprs) :
        lhs_expr(std::move(lhs_expr)),
        exprs(std::move(exprs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        bool error = false;

        auto lhs_oid = lhs_expr->eval(binding);

        if (lhs_oid.is_null()) {
            return lhs_oid;
        }

        for (auto& expr : exprs) {
            auto expr_oid = expr->eval(binding);
            if (expr_oid.is_null()) {
                error = true;
            } else if (SPARQL::Comparisons::compare(lhs_oid, expr_oid) == 0) {
                return Conversions::pack_bool(true);
            }
        }
        if (error) {
            return ObjectId::get_null();
        } else {
            return Conversions::pack_bool(false);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << '(';
        lhs_expr->print(os, ops);
        os << " IN (";
        auto first = true;
        for (auto& e : exprs) {
            if (first)
                first = false;
            else
                os << ", ";
            e->print(os, ops);
        }
        os << "))";
    }
};
} // namespace SPARQL
