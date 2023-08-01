#pragma once

#include <cassert>
#include <memory>
#include <set>
#include <type_traits>

#include "query/query_context.h"
#include "expr_rewrite_rule.h"
#include "query/rewriter/sparql/op/rewrite_rules/remove_optional_rhs.h"
#include "query/rewriter/sparql/op/rewrite_rule_visitor.h"

namespace SPARQL {
/**
  * Optionals do not add or remove matches, so it is possible to remove
  * the first optionals from exists and not exist scopes. This rewrite rule
  * implements this.
  *
  * Note that if the optional is nested, it might be possible that it cannot
  * be removed, for example, (Op1 OPTIONAL OP2) MINUS OP3 Where minus has
  * a variable in common with op2 only.
  *
 */
class RemoveOptionalsFromExists : public ExprRewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override {
        return remove_optionals_from<ExprExists>(unknown_expr)
            || remove_optionals_from<ExprNotExists>(unknown_expr);
    }

    template <typename T>
    bool remove_optionals_from(std::unique_ptr<Expr>& unknown_expr) {
        auto expr_exists = dynamic_cast<T*>(unknown_expr.get());
        if (expr_exists == nullptr) {
            return false;
        }
        if (is_castable_to<OpOptional>(expr_exists->op)) {
            auto optional_op = cast_to<OpOptional>(std::move(expr_exists->op));
            expr_exists->op = std::move(optional_op->lhs);
            return true;
        }
        return false;
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr> unknown_expr) override {
        // All the work is done in remove_optionals_from.
        return unknown_expr;
    }

};
} // namespace SPARQL
