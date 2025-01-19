#include "expr_to_binding_expr.h"

#include "query/optimizer/quad_model/binding_iter_constructor.h"
#include "query/query_context.h"

#include "query/executor/binding_iter/aggregation/mql/agg_avg.h"
#include "query/executor/binding_iter/aggregation/mql/agg_max.h"
#include "query/executor/binding_iter/aggregation/mql/agg_min.h"
#include "query/executor/binding_iter/aggregation/mql/agg_sum.h"

#include "query/executor/binding_iter/aggregation/mql/agg_count.h"
#include "query/executor/binding_iter/aggregation/mql/agg_count_all.h"
#include "query/executor/binding_iter/aggregation/mql/agg_count_all_distinct.h"
#include "query/executor/binding_iter/aggregation/mql/agg_count_distinct.h"

#include "query/executor/binding_iter/binding_expr/binding_expr_term.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_var.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_addition.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_and.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_division.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_equals.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_is.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_less.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_less_or_equals.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_modulo.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_multiplication.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_not.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_not_equals.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_or.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_regex.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_subtraction.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_tensor_distance_from_expr.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_tensor_distance_from_tensor.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_unary_minus.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_unary_plus.h"
#include "query/parser/expr/mql_exprs.h"

using namespace MQL;

void ExprToBindingExpr::visit(ExprVar& expr_var)
{
    tmp = std::make_unique<BindingExprVar>(expr_var.var);

    if (bic == nullptr) {
        return;
    }

    bic->group_saved_vars.insert(expr_var.var);

    if (inside_aggregation) {
        if (!bic->grouping) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(expr_var.var)
                + "\" cannot be used in aggregation without grouping"
            );
        }
    } else {
        // if (bic->grouping) {
        //     // TODO: Support this?
        //     // It's fine to use a var previously defined as an expression
        //     // Example:
        //     // MATCH  (?from)-[:?type]->(?to)
        //     // GROUP BY ?from
        //     // RETURN 2*sum(?to) AS ?x, -1*?x AS ?y
        //     // In that case ?x is not grouped but can be used
        //     for (const auto& [var, _] : bic->projection_order_exprs) {
        //         if (var == expr_var.var) {
        //             return;
        //         }
        //     }
        //     if (bic->group_vars.find(expr_var.var) == bic->group_vars.end()) {
        //         throw QuerySemanticException(
        //             "Variable \"" + get_query_ctx().get_var_name(expr_var.var) + "\" must be grouped"
        //         );
        //     }
        // }
    }
}

void ExprToBindingExpr::visit(ExprVarProperty& expr_var)
{
    tmp = std::make_unique<BindingExprVar>(expr_var.var_with_property);

    if (bic == nullptr) {
        return;
    }

    bic->group_saved_vars.insert(expr_var.var_with_property);
}

void ExprToBindingExpr::visit(ExprConstant& expr_constant)
{
    tmp = std::make_unique<BindingExprTerm>(expr_constant.value);
}

void ExprToBindingExpr::visit(ExprAddition& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprAddition>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprDivision& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprDivision>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprModulo& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprModulo>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprMultiplication& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp =
        std::make_unique<BindingExprMultiplication>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprSubtraction& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprSubtraction>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprUnaryMinus& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprUnaryMinus>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprUnaryPlus& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprUnaryPlus>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprGreaterOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLessOrEquals>(std::move(rhs_binding_expr), std::move(lhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprGreater& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLess>(std::move(rhs_binding_expr), std::move(lhs_binding_expr));
}


void ExprToBindingExpr::visit(ExprIs& expr)
{
    expr.expr->accept_visitor(*this);

    // property have one type fixed, trivial (NestedLoopJoin)
    auto property_var = dynamic_cast<ExprVarProperty*>(expr.expr.get());
    if (property_var != nullptr) {
        for (auto property : fixed_types_properties) {
            if (property.var_with_property == property_var->var_with_property) { // ?x.value
                tmp = std::make_unique<BindingExprTerm>(ObjectId(ObjectId::BOOL_TRUE));
                return;
            };
        }
    }
    tmp = std::make_unique<BindingExprIs>(std::move(tmp), expr.negation, expr.type);
}


void ExprToBindingExpr::visit(ExprLessOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLessOrEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprLess& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLess>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprNotEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprNotEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprAnd& expr)
{
    std::vector<std::unique_ptr<BindingExpr>> and_list;
    auto always_true = true;
    auto always_false = false;
    for (auto& e : expr.and_list) {
        e->accept_visitor(*this);

        if (tmp != nullptr) {
            auto potential_expr_term = dynamic_cast<BindingExprTerm*>(tmp.get());
            if (potential_expr_term == nullptr
                || potential_expr_term->object_id != ObjectId(ObjectId::BOOL_TRUE))
            {
                always_true = false;
                if (potential_expr_term != nullptr
                    && potential_expr_term->object_id == ObjectId(ObjectId::BOOL_FALSE))
                {
                    always_false = true;
                    break;
                }
            }
            and_list.push_back(std::move(tmp));
        }
    }
    if (always_true) {
        tmp = std::make_unique<BindingExprTerm>(ObjectId(ObjectId::BOOL_TRUE));
    } else if (always_false) {
        tmp = std::make_unique<BindingExprTerm>(ObjectId(ObjectId::BOOL_FALSE));
    } else if (and_list.size() > 1) {
        tmp = std::make_unique<BindingExprAnd>(std::move(and_list));
    } else if (and_list.size() == 1) {
        tmp = std::move(and_list[0]);
    }
}

void ExprToBindingExpr::visit(ExprNot& expr)
{
    expr.expr->accept_visitor(*this);
    auto potential_expr_term = dynamic_cast<BindingExprTerm*>(tmp.get());

    if (potential_expr_term != nullptr) {
        if (potential_expr_term->object_id == ObjectId(ObjectId::BOOL_TRUE)) {
            tmp = std::make_unique<BindingExprTerm>(ObjectId(ObjectId::BOOL_FALSE));
            return;
        } else if (potential_expr_term->object_id == ObjectId(ObjectId::BOOL_FALSE)) {
            tmp = std::make_unique<BindingExprTerm>(ObjectId(ObjectId::BOOL_TRUE));
            return;
        }
    }
    tmp = std::make_unique<BindingExprNot>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprOr& expr)
{
    std::vector<std::unique_ptr<BindingExpr>> or_list;
    auto always_true = false;
    for (auto& e : expr.or_list) {
        e->accept_visitor(*this);
        auto potential_expr_term = dynamic_cast<BindingExprTerm*>(tmp.get());
        if (potential_expr_term != nullptr && potential_expr_term->object_id == ObjectId(ObjectId::BOOL_TRUE))
        {
            always_true = true;
            break;
        }

        or_list.push_back(std::move(tmp));
    }
    if (always_true) {
        tmp = std::make_unique<BindingExprTerm>(ObjectId(ObjectId::BOOL_TRUE));
    } else {
        tmp = std::make_unique<BindingExprOr>(std::move(or_list));
    }
}

void ExprToBindingExpr::visit(MQL::ExprAggAvg& expr)
{
    check_and_make_aggregate<AggAvg>(expr.expr.get());
}

void ExprToBindingExpr::visit(MQL::ExprAggCountAll& expr)
{
    if (expr.distinct) {
        const auto non_internal_vars_set = get_query_ctx().get_non_internal_vars();
        std::vector<VarId> non_internal_vars_vec(non_internal_vars_set.begin(), non_internal_vars_set.end());
        check_and_make_aggregate<AggCountAllDistinct>(nullptr, std::move(non_internal_vars_vec));

        auto all_vars = get_query_ctx().get_all_vars();
        for (const auto& var : all_vars) {
            bic->group_saved_vars.insert(var);
        }
    } else {
        check_and_make_aggregate<AggCountAll>(nullptr);
    }

}

void ExprToBindingExpr::visit(MQL::ExprAggCount& expr)
{
    if (expr.distinct) {
        check_and_make_aggregate<AggCountDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggCount>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(MQL::ExprAggMax& expr)
{
    check_and_make_aggregate<AggMax>(expr.expr.get());
}

void ExprToBindingExpr::visit(MQL::ExprAggMin& expr)
{
    check_and_make_aggregate<AggMin>(expr.expr.get());
}

void ExprToBindingExpr::visit(MQL::ExprAggSum& expr)
{
    check_and_make_aggregate<AggSum>(expr.expr.get());
}

void ExprToBindingExpr::visit(ExprRegex& expr)
{
    expr.expr1->accept_visitor(*this);
    auto expr1 = std::move(tmp);

    expr.expr2->accept_visitor(*this);
    auto expr2 = std::move(tmp);

    if (expr.expr3) {
        expr.expr3->accept_visitor(*this);
        auto expr3 = std::move(tmp);
        tmp = std::make_unique<BindingExprRegex>(std::move(expr1), std::move(expr2), std::move(expr3));
    } else {
        tmp = std::make_unique<BindingExprRegex>(std::move(expr1), std::move(expr2), nullptr);
    }
}

void ExprToBindingExpr::visit(ExprTensorDistance& expr)
{
    expr.expr->accept_visitor(*this);
    auto expr_ = std::move(tmp);

    if (expr.expr_ref != nullptr) {
        // Both expressions
        expr.expr_ref->accept_visitor(*this);
        auto expr_ref = std::move(tmp);

        tmp = std::make_unique<BindingExprTensorDistanceFromExpr>(
            expr.tensor_store_name,
            expr.metric_type,
            std::move(expr_),
            std::move(expr_ref)
        );
        return;
    }

    // Expression and tensor
    tmp = std::make_unique<BindingExprTensorDistanceFromTensor>(
        expr.tensor_store_name,
        expr.metric_type,
        std::move(expr_),
        std::move(expr.tensor_ref)
    );
}

void ExprToBindingExpr::visit(ExprTextSearch&)
{
    throw QueryException("Non-mandatory TextSearches not implemented yet");
}

template<typename AggType, class... Args>
void ExprToBindingExpr::check_and_make_aggregate(Expr* expr, Args&&... args)
{
    if (bic == nullptr) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }

    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }

    if (inside_aggregation) {
        throw QuerySemanticException("Nested aggregations are not allowed");
    }

    VarId var { 0 };
    if (as_var.has_value()) {
        var = as_var.value();
    } else {
        var = get_query_ctx().get_internal_var();
    }

    inside_aggregation = true;

    std::unique_ptr<AggType> agg;
    if (expr != nullptr) {
        expr->accept_visitor(*this);
        agg = std::make_unique<AggType>(var, std::move(tmp), std::forward<Args>(args)...);
    } else {
        agg = std::make_unique<AggType>(var, nullptr, std::forward<Args>(args)...);
    }

    bic->aggregations.insert({ var, std::move(agg) });

    tmp = std::make_unique<BindingExprVar>(var);

    inside_aggregation = false;
}