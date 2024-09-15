#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprBNode : public Expr {
public:
    // may be nullptr
    std::unique_ptr<Expr> expr;

    ExprBNode(std::unique_ptr<Expr> expr) :
        expr (std::move(expr)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprBNode>(expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        if (expr)
            return expr->get_all_vars();
        return {};
    }

    bool has_aggregation() const override {
        if (expr) { return expr->has_aggregation(); }
        else { return false; }
    }
};
} // namespace SPARQL
