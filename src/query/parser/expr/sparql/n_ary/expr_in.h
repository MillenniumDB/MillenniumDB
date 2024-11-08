#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprIn : public Expr {
public:
    std::unique_ptr<Expr> lhs_expr;
    std::vector<std::unique_ptr<Expr>> exprs;

    ExprIn(std::unique_ptr<Expr> lhs_expr, std::vector<std::unique_ptr<Expr>> exprs) :
        lhs_expr  (std::move(lhs_expr)),
        exprs     (std::move(exprs)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        std::vector<std::unique_ptr<Expr>> exprs_clone;
        exprs_clone.reserve(exprs.size());
        for (auto& child_expr : exprs) {
            exprs_clone.push_back(child_expr->clone());
        }
        return std::make_unique<ExprIn>(lhs_expr->clone(), std::move(exprs_clone));
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = lhs_expr->get_all_vars();

        for (auto& expr : exprs) {
            auto expr_vars = expr->get_all_vars();
            res.insert(expr_vars.begin(), expr_vars.end());
        }
        return res;
    }

    bool has_aggregation() const override {
        if (lhs_expr->has_aggregation()) {
            return true;
        }
        for (auto& expr : exprs) {
            if (expr->has_aggregation()) {
                return true;
            }
        }
        return false;
    }
};
} // namespace SPARQL
