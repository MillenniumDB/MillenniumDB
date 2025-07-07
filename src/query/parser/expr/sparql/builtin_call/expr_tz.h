#pragma once

#include <memory>

#include "query/parser/expr/sparql/expr.h"

namespace SPARQL {
class ExprTZ : public Expr {
public:
    std::unique_ptr<Expr> expr;

    ExprTZ(std::unique_ptr<Expr> expr) :
        expr (std::move(expr)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprTZ>(expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return expr->has_aggregation();
    }
};
} // namespace SPARQL
