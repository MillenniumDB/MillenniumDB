#pragma once

#include <cassert>
#include <memory>
#include <set>
#include <type_traits>

#include "graph_models/object_id.h"
#include "query/query_context.h"
#include "query/rewriter/sparql/expr/rewrite_rules/expr_rewrite_rule.h"
namespace SPARQL {
/**
 * Simplifies all numeric operations that have a lhs and a rhs.
 */
template <typename ExprType>
class BooleanLiteralSimplification : public ExprRewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override {
        auto expr = dynamic_cast<ExprType*>(unknown_expr.get());
        if (expr == nullptr) {
            return false;
        }
        if constexpr (std::is_same<ExprType, ExprNot>::value) {
            return is_bool(expr->expr);
        }
        else {
            return (is_bool(expr->lhs)) || is_bool(expr->rhs);
        }
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr> unknown_expr) override {
        auto expr = cast_to<ExprType>(std::move(unknown_expr));
        return transform(expr);
    }

private:
    bool is_bool(std::unique_ptr<Expr>& literal) {
        auto expr = dynamic_cast<ExprObjectId*>(literal.get());
        if (expr == nullptr) {
            return false;
        }
        return expr->object_id.get_generic_type() == ObjectId::MASK_BOOL;
    }

    bool get_bool(ObjectId& oid) {
        if (oid.is_true()) {
            return true;
        } else {
            assert(oid.is_false());
            return false;
        }
    }

    std::unique_ptr<ExprObjectId> create_expr_object_id(bool value) {
        if (value) {
            return std::make_unique<ExprObjectId>(ObjectId(ObjectId::BOOL_TRUE));
        }
        return std::make_unique<ExprObjectId>(ObjectId(ObjectId::BOOL_FALSE));
    }

    std::unique_ptr<Expr> transform(std::unique_ptr<ExprAnd>& expr) {
        if (is_bool(expr->lhs)) {
            auto lhs = cast_to<ExprObjectId>(std::move(expr->lhs));
            auto value = get_bool(lhs->object_id);
            if (value)
                return std::move(expr->rhs);
            else
                return create_expr_object_id(false);
        }
        else {
            auto rhs = cast_to<ExprObjectId>(std::move(expr->rhs));
            auto value = get_bool(rhs->object_id);
            if (value)
                return std::move(expr->lhs);
            else
                return create_expr_object_id(false);
        }
    }

    std::unique_ptr<Expr> transform(std::unique_ptr<ExprOr>& expr) {
        if (is_bool(expr->lhs)) {
            auto lhs = cast_to<ExprObjectId>(std::move(expr->lhs));
            auto value = get_bool(lhs->object_id);
            if (!value)
                return std::move(expr->rhs);
            else
                return create_expr_object_id(true);
        }
        else {
            auto rhs = cast_to<ExprObjectId>(std::move(expr->rhs));
            auto value = get_bool(rhs->object_id);
            if (!value)
                return std::move(expr->lhs);
            else
                return create_expr_object_id(true);
        }
    }

    std::unique_ptr<Expr> transform(std::unique_ptr<ExprNot>& expr) {
        auto term = cast_to<ExprObjectId>(std::move(expr->expr));
        auto value = get_bool(term->object_id);
        return create_expr_object_id(!value);
    }
};
} // namespace SPARQL
