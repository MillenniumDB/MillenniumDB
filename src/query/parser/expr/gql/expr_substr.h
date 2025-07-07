#pragma once

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprSubStr : public Expr {
public:
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Expr> str_len;
    bool left;

    ExprSubStr(std::unique_ptr<Expr> expr, std::unique_ptr<Expr> str_len, bool left) :
        expr(std::move(expr)),
        str_len(std::move(str_len)),
        left(std::move(left))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprSubStr>(expr->clone(), str_len->clone(), left);
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
