#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/query_context.h"
#include "query/rewriter/sparql/expr/expr_rewrite_rule_visitor.h"
#include "query/rewriter/sparql/expr/rewrite_rules/distribute_or_into_and.h"
#include "query/rewriter/sparql/expr/rewrite_rules/distribute_not.h"
#include "query/rewriter/sparql/expr/rewrite_rules/simplify_boolean_literals/boolean_literal_simplification.h"
#include "query/rewriter/sparql/expr/rewrite_rules/simplify_boolean_literals/literal_simplification.h"
#include "query/rewriter/sparql/expr/rewrite_rules/remove_optionals_from_exists.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "query/rewriter/sparql/op/rewrite_rules/rewrite_rule.h"

namespace SPARQL {
/**
  * This class implements the rules:
  * Or Distributivity:
  * E1 Or (E2 And E3) = (E1 Or E2) And (E1 Or E3)
  *
  * Not Distributivity:
  * Not (E1 Or E2) = Not E1 And Not E2
  * Not (E1 And E2) = Not E1 Or Not E2
  * Not Not E = E
  * Not (Exists op) = NotExists var
  * Not (NotExists op) = Exists op
  *
  * Simplifications of all queries that don't have any variables
  *
  * [not] Exists (Op1 OPT Op2) = [not] Exists (Op1)
 */
class ExprIntoNormalForm : public RewriteRule {
private:
    ExprRewriteRuleVisitor expr_visitor;

public:
    ExprIntoNormalForm() {
        expr_visitor.add_rule<RemoveOptionalsFromExists>();
        expr_visitor.add_rule<DistributeOrIntoAnd>();
        expr_visitor.add_rule<DistributeNot>();
        expr_visitor.add_rule<LiteralSimplification>();
        expr_visitor.add_rule<BooleanLiteralSimplification<ExprAnd>>();
        expr_visitor.add_rule<BooleanLiteralSimplification<ExprOr>>();
        expr_visitor.add_rule<BooleanLiteralSimplification<ExprNot>>();
    }

    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        bool has_rewritten = false;

        if (is_castable_to<OpFilter>(unknown_op)) {
            while (true) {
                auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
                for (auto& expr : op_filter->filters) {
                    expr_visitor.start_visit(expr);
                }
                if (expr_visitor.reset_and_check_if_has_rewritten_a_rule()) {
                    has_rewritten = true;
                } else {
                    break;
                }
            }
        } else if (is_castable_to<OpProcedure>(unknown_op)) {
            while (true) {
                auto op_procedure = dynamic_cast<OpProcedure*>(unknown_op.get());
                for (auto& expr : op_procedure->argument_exprs) {
                    expr_visitor.start_visit(expr);
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

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        // All work is done in is_possible_to_regroup, this is because
        // is_possible_to_regroup marks whether a change will be made
        // here. This is the only way to use the visitor interface for
        // expr's.
        return unknown_op;
    }

};
} // namespace SPARQL
