#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"

namespace SPARQL {
class ExprConcat : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> exprs;

    ExprConcat(std::vector<std::unique_ptr<Expr>> exprs) :
        exprs (std::move(exprs)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    virtual std::unique_ptr<Expr> clone() const override {
        std::vector<std::unique_ptr<Expr>> exprs_clone;
        exprs_clone.reserve(exprs.size());
        for (auto& child_expr : exprs) {
            exprs_clone.push_back(child_expr->clone());
        }
        return std::make_unique<ExprConcat>(std::move(exprs_clone));
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;

        for (auto& expr : exprs) {
            auto expr_vars = expr->get_all_vars();
            res.insert(expr_vars.begin(), expr_vars.end());
        }
        return res;
    }

    bool has_aggregation() const override {
        for (auto& expr : exprs) {
            if (expr->has_aggregation()) {
                return true;
            }
        }
        return false;
    }
};
} // namespace SPARQL
