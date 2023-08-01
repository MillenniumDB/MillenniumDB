#pragma once

#include <cassert>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>

#include "graph_models/object_id.h"
#include "query/optimizer/rdf_model/expr_to_binding_expr.h"
#include "query/query_context.h"
#include "query/rewriter/sparql/expr/rewrite_rules/expr_rewrite_rule.h"

namespace SPARQL {
/**
 * Simplifies all numeric operations that have a lhs and a rhs.
 */
class LiteralSimplification : public ExprRewriteRule {
private:
    Binding binding;
    ObjectId resulting_object_id;

public:
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override {
        if (unknown_expr->get_all_vars().size() == 0) {
            if (is_castable_to<ExprObjectId>(unknown_expr))
                return false; // Already simplified.

            std::set<VarId> safe_assigned_vars;
            std::set<VarId> possible_assigned_vars;

            ExprToBindingExpr expr_to_binding_expr(
                safe_assigned_vars,
                possible_assigned_vars
            );
            unknown_expr->accept_visitor(expr_to_binding_expr);
            auto binding_expr = std::move(expr_to_binding_expr.current_binding_expr);
            resulting_object_id = binding_expr->eval(binding);
            return true;
        }
        return false;
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr>) override {
        return std::make_unique<ExprObjectId>(resulting_object_id);
    }
};

} // namespace SPARQL
