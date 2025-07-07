#pragma once

#include <memory>
#include <set>

#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "query/parser/expr/sparql/exprs.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to (Schmidt, 2010) the following rewrite rules are valid:
 * Filter_Bound(?x)(A1) = A1 if ?x in safeVars(A1)                             BndI
 * Filter_Bound(?x)(A1) = null if ?x not in vars(A1)                           BndII
 * Not(Filter_Bound(?x))(A1) = null if ?x in safeVars(A1)                      BndIII
 * Not(Filter_Bound(?x))(A1) = A1 if ?x not in vars(A1)                        BndIV
 * Filter_Bound(?x)(A1 OPT A2) = A1 JOIN A2 if ?x in (safeVars(A2) \ vars(A1)) BndV
 */
class SimplifyBound : public RewriteRule {
private:
    bool has_regrouped = false;
    bool return_empty_op = false;
    bool changed_to_join = false;
    void reset() { has_regrouped = return_empty_op = changed_to_join = false; }

public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        reset();
        if (!is_castable_to<OpFilter>(unknown_op)) {
            return false;
        }
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));

        //std::cout << "Op:" << std::endl;
        //op_filter->print_to_ostream(std::cout, 0);

        auto safe_vars = op_filter->op->get_safe_vars();
        auto vars = op_filter->op->get_all_vars();
        auto opt_vars = get_vars_that_would_produce_a_join_change(op_filter->op);

        //std::cout << "safe_vars: ";
        //for (auto& var : safe_vars) { std::cout << var << ' ';}; std::cout << std::endl;
        //std::cout << "vars: ";
        //for (auto& var : vars) { std::cout << var << ' ';}; std::cout << std::endl;
        //std::cout << "opt_vars: ";
        //for (auto& var : opt_vars) { std::cout << var << ' ';}; std::cout << std::endl;


        std::vector<std::unique_ptr<Expr>> new_filters;
        for (auto& expr : op_filter->filters) {
            // BndI
            if (expr_is_bound_and_is_in_vars(expr, safe_vars)) {
                //std::cout << "BndI" << std::endl;
                has_regrouped = true; // expr is not included
            }
            // BndII
            else if (is_castable_to<ExprBound>(expr) &&
                     !expr_is_bound_and_is_in_vars(expr, vars)) {
                //std::cout << "BndII" << std::endl;
                return_empty_op = true;
                has_regrouped = true;
            }
            // BndIII
            else if (expr_not_has_bound_expr_and_is_in_vars(expr, safe_vars)) {
                //std::cout << "BndIII" << std::endl;
                return_empty_op = true;
                has_regrouped = true;
            }
            // BndIV
            else if (expr_is_not_and_has_bound_expr(expr) &&
                    !expr_not_has_bound_expr_and_is_in_vars(expr, vars)) {
                //std::cout << "BndIV" << std::endl;
                has_regrouped = true; // expr is not included
            }
            // BndV
            else if (expr_is_bound_and_is_in_vars(expr, opt_vars) && !changed_to_join) {
                //std::cout << "BndV" << std::endl;
                has_regrouped = true;
                changed_to_join = true;
                auto op_optional = cast_to<OpOptional>(std::move(op_filter->op));
                op_filter->op = std::make_unique<OpJoin>(
                    std::move(op_optional->lhs),
                    std::move(op_optional->rhs)
                );
            }
            else {
                //std::cout << "Pushed the old filter" << std::endl;
                new_filters.push_back(std::move(expr));
            }
        }
        op_filter->filters = std::move(new_filters);

        unknown_op = std::move(op_filter);
        return has_regrouped;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        // All the work was done in is_possible_to_regroup.
        if (return_empty_op) {
            return std::make_unique<OpEmpty>(unknown_op);
        }
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        if (op_filter->filters.size() == 0) {
            return std::move(op_filter->op);
        }
        return op_filter;
    }

private:
    std::set<VarId> get_vars_that_would_produce_a_join_change(std::unique_ptr<Op>& op) {
        if (!is_castable_to<OpOptional>(op))
            return {};

        auto op_optional = dynamic_cast<OpOptional*>(op.get());
        auto safe_vars_in_a2 = op_optional->rhs->get_safe_vars();
        auto vars_in_a1 = op_optional->lhs->get_all_vars();

        std::set<VarId> safe_vars_in_a2_and_not_in_a1;
        for (auto &var : safe_vars_in_a2)
            if (vars_in_a1.find(var) == vars_in_a1.end())
                safe_vars_in_a2_and_not_in_a1.insert(var);

        return safe_vars_in_a2_and_not_in_a1;
    }

    bool expr_is_bound_and_is_in_vars(std::unique_ptr<Expr>& expr,
                                      const std::set<VarId>& vars)
    {
        if (!is_castable_to<ExprBound>(expr)) {
            return false;
        }
        auto bound_expr = dynamic_cast<ExprBound*>(expr.get());
        //std::cout << "The var in expr is: " << bound_expr->var << std::endl;
        return vars.find(bound_expr->var) != vars.end();
    }

    bool expr_is_not_and_has_bound_expr(std::unique_ptr<Expr>& expr) {
        if (!is_castable_to<ExprNot>(expr)) {
            return false;
        }
        auto not_expr = dynamic_cast<ExprNot*>(expr.get());
        return is_castable_to<ExprBound>(not_expr->expr);
    }

    bool expr_not_has_bound_expr_and_is_in_vars(
            std::unique_ptr<Expr>& expr,
            const std::set<VarId>& vars) {
        if (!is_castable_to<ExprNot>(expr)) {
            return false;
        }
        auto not_expr = dynamic_cast<ExprNot*>(expr.get());
        //std::cout << "It is a ExprNot" << std::endl;
        //not_expr->print_to_ostream(std::cout);
        if (!is_castable_to<ExprBound>(not_expr->expr)) {
            return false;
        }
        auto bound_expr = dynamic_cast<ExprBound*>(not_expr->expr.get());
        //std::cout << "It is a bound Expr" << std::endl;
        //bound_expr->print_to_ostream(std::cout);
        return vars.find(bound_expr->var) != vars.end();
    }

};
} // namespace SPARQL
