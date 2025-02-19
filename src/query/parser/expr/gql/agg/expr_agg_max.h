#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprAggMax : public Expr {
public:
    std::unique_ptr<Expr> expr;
    bool distinct;

    ExprAggMax(std::unique_ptr<Expr> expr, bool distinct) :
        expr     (std::move(expr)),
        distinct (distinct) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggMax>(expr->clone(), distinct);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return true;
    }
};
} // namespace GQL
