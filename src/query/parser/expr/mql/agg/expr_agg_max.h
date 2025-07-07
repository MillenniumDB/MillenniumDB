#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprAggMax : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprAggMax(std::unique_ptr<Expr> expr) :
        expr(std::move(expr))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprAggMax>(expr->clone());
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
        return true;
    }
};
} // namespace MQL
