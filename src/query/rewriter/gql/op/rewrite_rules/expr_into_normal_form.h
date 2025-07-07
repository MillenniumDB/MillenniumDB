#pragma once

#include "query/parser/expr/gql/expr_and.h"
#include "query/parser/op/gql/graph_pattern/op_filter.h"
#include "query/parser/op/gql/op_let.h"
#include "query/rewriter/gql/expr/expr_rewrite_rule_visitor.h"
#include "query/rewriter/gql/expr/rewrite_rules/flatten_labels.h"
#include "query/rewriter/gql/expr/rewrite_rules/literal_simplification.h"
#include "rewrite_rule.h"

namespace GQL {
class ExprIntoNormalForm : public RewriteRule {
    ExprRewriteRuleVisitor expr_visitor;

public:
    ExprIntoNormalForm()
    {
        expr_visitor.add_rule<FlattenLogicalExprs<ExprAnd>>();
        expr_visitor.add_rule<FlattenLogicalExprs<ExprOr>>();
        expr_visitor.add_rule<LiteralSimplification>();
    }

    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override
    {
        bool is_filter = false;
        bool is_let = false;

        if (is_castable_to<OpFilter>(unknown_op)) {
            is_filter = true;
        } else if (is_castable_to<OpLet>(unknown_op)) {
            is_let = true;
        } else {
            return false;
        }

        bool has_rewritten = false;
        while (true) {
            if (is_filter) {
                auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
                for (auto& expr : op_filter->exprs) {
                    expr_visitor.start_visit(expr);
                }
                if (expr_visitor.reset_and_check_if_has_rewritten_a_rule()) {
                    has_rewritten = true;
                } else {
                    break;
                }
            } else if (is_let) {
                auto op_let = dynamic_cast<OpLet*>(unknown_op.get());
                for (auto& item : op_let->items) {
                    expr_visitor.start_visit(item.expr);
                }
                if (expr_visitor.reset_and_check_if_has_rewritten_a_rule()) {
                    has_rewritten = true;
                } else {
                    break;
                }
            }
        }
        return has_rewritten;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override
    {
        return unknown_op;
    }
};
} // namespace GQL
