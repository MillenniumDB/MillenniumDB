#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprNormalize : public Expr {
public:
    std::unique_ptr<Expr> expr;
    std::string form;

    ExprNormalize(std::unique_ptr<Expr> expr, std::string form) :
        expr(std::move(expr)),
        form(std::move(form))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprNormalize>(expr->clone(), form);
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
