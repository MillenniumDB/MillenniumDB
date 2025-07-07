#pragma once

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprIs : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    ExprIs(std::unique_ptr<Expr> lhs_expr, std::unique_ptr<Expr> rhs_expr) :
        lhs(std::move(lhs_expr)),
        rhs(std::move(rhs_expr))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprIs>(lhs->clone(), rhs->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return lhs->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override
    {
        return lhs->get_all_vars();
    }
};
} // namespace GQL
