#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprRadians : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprRadians(std::unique_ptr<Expr> expr) :
        expr(std::move(expr))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprRadians>(expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return expr->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override
    {
        return expr->get_all_vars();
    }
};
} // namespace GQL
