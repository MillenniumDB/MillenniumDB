#pragma once

#include <memory>
#include <set>

#include "query/parser/expr/expr.h"

// IMPORTANT: This is not the aggregate SUM(), its the function mdbfn:sum() for tensors
namespace SPARQL {
class ExprSum : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprSum(std::unique_ptr<Expr> expr) :
        expr(std::move(expr))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprSum>(expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override
    {
        return expr->has_aggregation();
    }
};
} // namespace SPARQL
