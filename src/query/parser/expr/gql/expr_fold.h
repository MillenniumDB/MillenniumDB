#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprFold : public Expr {
public:
    std::unique_ptr<Expr> expr;
    bool upper;

    ExprFold(std::unique_ptr<Expr> expr, bool upper) :
        expr(std::move(expr)),
        upper(std::move(upper))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprFold>(expr->clone(), upper);
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
