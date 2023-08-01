#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/query_context.h"
#include "expr_rewrite_rule.h"

namespace SPARQL {
/**
 * E1 OR (E2 AND E3) = (E1 OR E2) AND (E1 OR E3)
 */
class DistributeOrIntoAnd : public ExprRewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override {
        if (!is_castable_to<ExprOr>(unknown_expr)) {
            return false;
        }
        auto or_expr = dynamic_cast<ExprOr*>(unknown_expr.get());
        return (is_castable_to<ExprAnd>(or_expr->rhs));
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr> unknown_expr) override {
        auto or_expr = cast_to<ExprOr>(std::move(unknown_expr));
        auto and_expr = cast_to<ExprAnd>(std::move(or_expr->rhs));
        return std::make_unique<ExprAnd>(
                    std::make_unique<ExprOr>(
                        or_expr->lhs->clone(),
                        std::move(and_expr->lhs)
                    ),
                    std::make_unique<ExprOr>(
                        or_expr->lhs->clone(), // less error prone to clone
                        std::move(and_expr->rhs)
                        )
                );
    }
};
} // namespace SPARQL
