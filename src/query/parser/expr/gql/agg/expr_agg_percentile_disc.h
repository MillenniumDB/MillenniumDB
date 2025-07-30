#pragma once

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprAggPercentileDisc : public Expr {
public:
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Expr> percentile;
    bool distinct;
    VarId var;

    ExprAggPercentileDisc(
        std::unique_ptr<Expr> expr,
        std::unique_ptr<Expr> percentile,
        bool distinct,
        VarId var
    ) :
        expr(std::move(expr)),
        percentile(std::move(percentile)),
        distinct(distinct),
        var(var)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprAggPercentileDisc>(expr->clone(), percentile->clone(), distinct, var);
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
} // namespace GQL
