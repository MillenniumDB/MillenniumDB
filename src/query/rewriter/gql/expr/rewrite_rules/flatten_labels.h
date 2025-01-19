#pragma once

#include "expr_rewrite_rule.h"

namespace GQL {

template<typename LogicalExpr>
class FlattenLogicalExprs : public ExprRewriteRule {
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override
    {
        auto expr = dynamic_cast<LogicalExpr*>(unknown_expr.get());
        if (expr == nullptr) {
            return false;
        }

        for (auto& expr : expr->exprs) {
            if (is_castable_to<LogicalExpr>(expr)) {
                return true;
            }
        }

        return false;
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr> unknown_expr) override
    {
        std::vector<std::unique_ptr<Expr>> new_exprs;

        auto expr = cast_to<LogicalExpr>(std::move(unknown_expr));

        for (auto& sub_expr : expr->exprs) {
            if (is_castable_to<LogicalExpr>(sub_expr)) {
                auto sub_expr_cast = cast_to<LogicalExpr>(std::move(sub_expr));

                for (auto& sub_sub_expr : sub_expr_cast->exprs) {
                    new_exprs.push_back(std::move(sub_sub_expr));
                }
            } else {
                new_exprs.push_back(std::move(sub_expr));
            }
        }
        return std::make_unique<LogicalExpr>(std::move(new_exprs));
    }
};

} // namespace GQL
