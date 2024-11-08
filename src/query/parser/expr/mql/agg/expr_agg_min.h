#pragma once

#include "query/parser/expr/expr.h"

namespace MQL {

class ExprAggMin : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprAggMin(std::unique_ptr<Expr> expr) :
        expr (std::move(expr)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggMin>(expr->clone());
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
} // namespace MQL
