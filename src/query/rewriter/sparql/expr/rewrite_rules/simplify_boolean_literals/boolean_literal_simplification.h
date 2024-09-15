#pragma once

#include <cassert>
#include <memory>
#include <set>
#include <type_traits>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/rdf_object_id.h"
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
        auto expr = dynamic_cast<ExprTerm*>(literal.get());
        if (expr == nullptr) {
            return false;
        }
        return RDF_OID::get_generic_type(expr->term) == RDF_OID::GenericType::BOOL;
    }

    bool get_bool(ObjectId& oid) {
        if (oid.is_true()) {
            return true;
        } else {
            assert(oid.is_false());
            return false;
        }
    }

    std::unique_ptr<ExprTerm> create_expr_object_id(bool value) {
        if (value) {
            return std::make_unique<ExprTerm>(ObjectId(ObjectId::BOOL_TRUE));
        }
        return std::make_unique<ExprTerm>(ObjectId(ObjectId::BOOL_FALSE));
    }

    std::unique_ptr<Expr> transform(std::unique_ptr<ExprAnd>& expr) {
        if (is_bool(expr->lhs)) {
            auto lhs = cast_to<ExprTerm>(std::move(expr->lhs));
            auto value = get_bool(lhs->term);
            if (value)
                return std::move(expr->rhs);
            else
                return create_expr_object_id(false);
        }
        else {
            auto rhs = cast_to<ExprTerm>(std::move(expr->rhs));
            auto value = get_bool(rhs->term);
            if (value)
                return std::move(expr->lhs);
            else
                return create_expr_object_id(false);
        }
    }

    std::unique_ptr<Expr> transform(std::unique_ptr<ExprOr>& expr) {
        if (is_bool(expr->lhs)) {
            auto lhs = cast_to<ExprTerm>(std::move(expr->lhs));
            auto value = get_bool(lhs->term);
            if (!value)
                return std::move(expr->rhs);
            else
                return create_expr_object_id(true);
        }
        else {
            auto rhs = cast_to<ExprTerm>(std::move(expr->rhs));
            auto value = get_bool(rhs->term);
            if (!value)
                return std::move(expr->lhs);
            else
                return create_expr_object_id(true);
        }
    }

    std::unique_ptr<Expr> transform(std::unique_ptr<ExprNot>& expr) {
        auto term = cast_to<ExprTerm>(std::move(expr->expr));
        auto value = get_bool(term->term);
        return create_expr_object_id(!value);
    }
};
} // namespace SPARQL
