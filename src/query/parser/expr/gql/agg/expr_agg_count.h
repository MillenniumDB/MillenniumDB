#pragma once

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprAggCount : public Expr {
public:
    std::unique_ptr<Expr> expr;
    bool distinct;

    ExprAggCount(std::unique_ptr<Expr> expr, bool distinct) :
        expr(std::move(expr)),
        distinct(distinct)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprAggCount>(expr->clone(), distinct);
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
