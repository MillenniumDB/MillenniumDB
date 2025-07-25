#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "graph_models/object_id.h"
#include "query/optimizer/property_graph_model/expr_to_binding_expr.h"
#include "query/parser/expr/gql/expr_term.h"
#include "query/rewriter/gql/expr/rewrite_rules/expr_rewrite_rule.h"

namespace GQL {

class LiteralSimplification : public ExprRewriteRule {
private:
    Binding binding;
    ObjectId resulting_object_id;

public:
    bool is_possible_to_regroup(std::unique_ptr<Expr>& unknown_expr) override
    {
        if (unknown_expr->get_all_vars().size() == 0 && !unknown_expr->has_aggregation()) {
            // Already simplified.
            if (is_castable_to<ExprTerm>(unknown_expr))
                return false;

            ExprToBindingExpr expr_to_binding_expr;
            unknown_expr->accept_visitor(expr_to_binding_expr);
            auto binding_expr = std::move(expr_to_binding_expr.tmp);
            resulting_object_id = binding_expr->eval(binding);
            return true;
        }
        return false;
    }

    std::unique_ptr<Expr> regroup(std::unique_ptr<Expr>) override
    {
        return std::make_unique<ExprTerm>(resulting_object_id);
    }
};

} // namespace GQL
