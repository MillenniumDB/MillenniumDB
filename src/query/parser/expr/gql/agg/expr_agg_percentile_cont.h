#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace GQL {
class ExprAggPercentileCont : public Expr {
public:
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Expr> percentile;
    bool distinct;

    ExprAggPercentileCont(std::unique_ptr<Expr> expr, std::unique_ptr<Expr> percentile, bool distinct) :
        expr      (std::move(expr)),
        percentile (std::move(percentile)),
        distinct  (distinct) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprAggPercentileCont>(expr->clone(), percentile->clone(), distinct);
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
