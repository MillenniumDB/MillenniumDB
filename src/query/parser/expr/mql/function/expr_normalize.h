#pragma once

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprNormalize : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprNormalize(std::unique_ptr<Expr> expr) :
        expr(std::move(expr))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprNormalize>(expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return expr->get_all_vars();
    }

    std::set<VarId> get_input_vars() const override
    {
        return expr->get_input_vars();
    }

    bool has_aggregation() const override
    {
        return expr->has_aggregation();
    }

    void print(std::ostream& os) const override
    {
        os << "NORMALIZE(" << *expr << ')';
    }
};
} // namespace MQL
