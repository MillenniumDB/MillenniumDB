#pragma once

#include <cassert>
#include <memory>
#include <set>
#include <type_traits>

#include "query/query_context.h"
#include "expr_rewrite_rule.h"

namespace SPARQL {
/**
 * According to (Schmidt, 2010) the following rewrite rule is valid:
 * Filter(A1 UNION A2) = Filter(A1) UNION Filter (A2)
 */
class DistributeNot : public ExprRewriteRule {
private:
    bool can_distribute_to_and = false;
    bool can_distribute_to_or = false;
    bool can_distribute_to_not = false;
    bool can_distribute_to_exists = false;
    bool can_distribute_to_not_exists = false;
    void reset() {can_distribute_to_and = can_distribute_to_or = can_distribute_to_not = false;}

public:
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override {
        if (is_possible_to_regroup<ExprAnd>(unknown_expr))
            can_distribute_to_and = true;
        else if (is_possible_to_regroup<ExprOr>(unknown_expr))
            can_distribute_to_or = true;
        else if (is_possible_to_regroup<ExprNot>(unknown_expr))
            can_distribute_to_not = true;
        else if (is_possible_to_regroup<ExprExists>(unknown_expr))
            can_distribute_to_exists = true;
        else if (is_possible_to_regroup<ExprNotExists>(unknown_expr))
            can_distribute_to_not_exists = true;
        return can_distribute_to_and || can_distribute_to_or || can_distribute_to_not;
    }

    template <typename T>
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) {
        if (!is_castable_to<ExprNot>(unknown_expr))
            return false;
        auto not_expr = dynamic_cast<ExprNot*>(unknown_expr.get());
        return is_castable_to<T>(not_expr->expr);
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr> unknown_expr) override {
        if (can_distribute_to_and) {
            reset();
            return de_morgan_regroup<ExprAnd>(std::move(unknown_expr));
        }
        else if (can_distribute_to_or) {
            reset();
            return de_morgan_regroup<ExprOr>(std::move(unknown_expr));
        }
        else if (can_distribute_to_not) {
            reset();
            auto first_not_expr = cast_to<ExprNot>(std::move(unknown_expr));
            auto second_not_expr = cast_to<ExprNot>(std::move(first_not_expr->expr));
            return std::move(second_not_expr->expr);
        }
        else if (can_distribute_to_exists) {
            reset();
            auto first_not_expr = cast_to<ExprNot>(std::move(unknown_expr));
            auto exists_expr = cast_to<ExprExists>(std::move(first_not_expr->expr));
            return std::make_unique<ExprNotExists>(std::move(exists_expr->op));
        }
        else if (can_distribute_to_not_exists) {
            reset();
            auto first_not_expr = cast_to<ExprNot>(std::move(unknown_expr));
            auto exists_expr = cast_to<ExprExists>(std::move(first_not_expr->expr));
            return std::make_unique<ExprExists>(std::move(exists_expr->op));
        }
        else {
            assert(false && "Must be able to distribute to and or to or");
            return {}; // So that compiler wont bother us.
        }
    }

    template <typename T>
    std::unique_ptr<Expr> de_morgan_regroup(std::unique_ptr<Expr> unknown_expr) {
        auto not_expr = cast_to<ExprNot>(std::move(unknown_expr));
        auto t_expr = cast_to<T>(std::move(not_expr->expr));
        if (std::is_same<T, ExprAnd>::value) {
            return std::make_unique<ExprOr>(
                        std::make_unique<ExprNot>(std::move(t_expr->lhs)),
                        std::make_unique<ExprNot>(std::move(t_expr->rhs))
            );
        }
        else if (std::is_same<T, ExprOr>::value) {
            return std::make_unique<ExprAnd>(
                        std::make_unique<ExprNot>(std::move(t_expr->lhs)),
                        std::make_unique<ExprNot>(std::move(t_expr->rhs))
            );
        }
        else {
            assert(false && "T from de_morgan_regroup<T> must be ExprAnd or ExprOr");
            return {}; // So that compiler wont bother us.
        }
    }
};
} // namespace SPARQL
