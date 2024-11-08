#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprStrLen : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprStrLen(std::unique_ptr<Expr> expr) :
        expr (std::move(expr)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprStrLen>(expr->clone());
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return expr->has_aggregation();
    }
};
} // namespace SPARQL
