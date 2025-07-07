#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprManhattanDistance : public Expr {
public:
    std::unique_ptr<Expr> expr1;
    std::unique_ptr<Expr> expr2;

    ExprManhattanDistance(std::unique_ptr<Expr> expr1, std::unique_ptr<Expr> expr2) :
        expr1(std::move(expr1)),
        expr2(std::move(expr2))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprManhattanDistance>(expr1->clone(), expr2->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = expr1->get_all_vars();
        const auto expr2_vars = expr2->get_all_vars();
        res.insert(expr2_vars.begin(), expr2_vars.end());
        return res;
    }

    bool has_aggregation() const override
    {
        return expr1->has_aggregation() || expr2->has_aggregation();
    }
};
} // namespace MQL
