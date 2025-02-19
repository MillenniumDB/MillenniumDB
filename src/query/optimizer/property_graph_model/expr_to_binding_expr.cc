#include "expr_to_binding_expr.h"

#include "query/executor/binding_iter/binding_expr/gql_binding_exprs.h"
#include "query/optimizer/property_graph_model/binding_iter_constructor.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/executor/binding_iter/aggregation/gql/aggs.h"

using namespace GQL;

void ExprToBindingExpr::visit(ExprVar& expr_var)
{
    tmp = std::make_unique<BindingExprVar>(expr_var.id);
}

void ExprToBindingExpr::visit(ExprTerm& expr_term)
{
    tmp = std::make_unique<BindingExprTerm>(expr_term.term);
}

void ExprToBindingExpr::visit(ExprOr& expr)
{
    std::vector<std::unique_ptr<BindingExpr>> or_list;
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
        if (tmp != nullptr) {
            or_list.push_back(std::move(tmp));
        }
    }
    if (or_list.size() > 1) {
        tmp = std::make_unique<BindingExprOr>(std::move(or_list));
    } else if (or_list.size() == 1) {
        tmp = std::move(or_list[0]);
    }
}

void ExprToBindingExpr::visit(ExprAnd& expr)
{
    std::vector<std::unique_ptr<BindingExpr>> and_list;
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
        if (tmp != nullptr) {
            and_list.push_back(std::move(tmp));
        }
    }
    // if the list is empty, the evaluation is true
    if (and_list.size() == 1) {
        tmp = std::move(and_list[0]);
    } else {
        tmp = std::make_unique<BindingExprAnd>(std::move(and_list));
    }
}

void ExprToBindingExpr::visit(ExprNot& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprNot>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprHasNodeLabel& expr)
{
    tmp = std::make_unique<BindingExprHasNodeLabel>(expr.node_id, expr.label_id);
}

void ExprToBindingExpr::visit(ExprHasEdgeLabel& expr)
{
    tmp = std::make_unique<BindingExprHasEdgeLabel>(expr.edge_id, expr.label_id);
}

void ExprToBindingExpr::visit(ExprEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprProperty& expr)
{
    tmp = std::make_unique<BindingExprVar>(expr.value);
}

void ExprToBindingExpr::visit(ExprConcat& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprConcat>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprGreaterOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprGreaterOrEquals>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}

void ExprToBindingExpr::visit(ExprGreater& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprGreater>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprIs& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprIs>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
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

void ExprToBindingExpr::visit(ExprXor& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprXor>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprAddition& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprAddition>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprSubtraction& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprSubtraction>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprMultiplication& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprMultiplication>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}

void ExprToBindingExpr::visit(ExprDivision& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprDivision>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprAbs& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprAbs>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprModulo& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprModulo>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprSin& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprSin>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprCos& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprCos>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprTan& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprTan>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprCot& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprCot>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprSinh& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprSinh>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprCosh& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprCosh>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprTanh& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprTanh>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprAsin& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprAsin>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprAcos& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprAcos>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprAtan& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprAtan>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprDegrees& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprDegrees>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprRadians& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprRadians>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprLog& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLog>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprLog10& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprLog10>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprLn& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprLn>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprExp& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprExp>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprPower& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprPower>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprSqrt& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprSqrt>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprFloor& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprFloor>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprCeil& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprCeil>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprLength& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprLength>(std::move(tmp));
}

void ExprToBindingExpr::visit(ExprSubStr& expr)
{
    expr.expr->accept_visitor(*this);
    auto str_binding_expr = std::move(tmp);
    expr.str_len->accept_visitor(*this);
    auto str_len_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprSubStr>(std::move(str_binding_expr), std::move(str_len_binding_expr), std::move(expr.left));
}

void ExprToBindingExpr::visit(ExprFold& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprFold>(std::move(tmp), std::move(expr.upper));
}

void ExprToBindingExpr::visit(ExprSingleTrim& expr)
{
    expr.str->accept_visitor(*this);
    auto str_binding_expr = std::move(tmp);
    std::unique_ptr<BindingExpr> char_binding_expr;
    if (expr.single_char == nullptr) {
        char_binding_expr = nullptr;
    } else {
        expr.single_char->accept_visitor(*this);
        char_binding_expr = std::move(tmp);
    }

    tmp = std::make_unique<BindingExprSingleTrim>(std::move(str_binding_expr), std::move(char_binding_expr), std::move(expr.specification));
}

void ExprToBindingExpr::visit(ExprMultiTrim& expr)
{
    expr.trim_src->accept_visitor(*this);
    auto str_binding_expr = std::move(tmp);
    std::unique_ptr<BindingExpr> delim_binding_expr;
    if (expr.delim_str == nullptr) {
        delim_binding_expr = nullptr;
    } else {
        expr.delim_str->accept_visitor(*this);
        delim_binding_expr = std::move(tmp);
    }

    tmp = std::make_unique<BindingExprMultiTrim>(std::move(str_binding_expr), std::move(delim_binding_expr), std::move(expr.specification));
}

void ExprToBindingExpr::visit(ExprNormalize& expr)
{
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprNormalize>(std::move(tmp), std::move(expr.form));
}

void ExprToBindingExpr::visit(ExprNullIf& expr)
{
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprNullIf>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}

void ExprToBindingExpr::visit(ExprCoalesce& expr)
{
    std::vector<std::unique_ptr<BindingExpr>> expressions;
    for (const auto& expression : expr.expressions) {
        if (expression != nullptr) {
            expression->accept_visitor(*this);
            expressions.push_back(std::move(tmp));
        }
    }

    tmp = std::make_unique<BindingExprCoalesce>(std::move(expressions));
}

void ExprToBindingExpr::visit(ExprSimpleCase& expr)
{
    expr.case_operand->accept_visitor(*this);
    auto case_operand_binding_expr = std::move(tmp);
    std::vector<std::pair<std::pair<std::string, std::vector<std::unique_ptr<BindingExpr>>>, std::unique_ptr<BindingExpr>>> when_clauses_binding;
    for (const auto& clauses : expr.when_clauses) {
        std::vector<std::unique_ptr<BindingExpr>> clauses_binding;
        for (const auto& clause : clauses.first.second) {
            clause->accept_visitor(*this);
            clauses_binding.push_back(std::move(tmp));
        }
        std::pair clause_binding = {clauses.first.first, std::move(clauses_binding)};
        clauses.second->accept_visitor(*this);
        std::pair final_clause_bindings = {std::move(clause_binding), std::move(tmp)};
        when_clauses_binding.push_back(std::move(final_clause_bindings));
    }
    std::unique_ptr<BindingExpr> else_binding_expr;
    if (expr.else_expr == nullptr) {
        else_binding_expr = nullptr;
    } else {
        expr.else_expr->accept_visitor(*this);
        else_binding_expr = std::move(tmp);
    }

    tmp = std::make_unique<BindingExprSimpleCase>(std::move(case_operand_binding_expr), std::move(when_clauses_binding), std::move(else_binding_expr));
}

void ExprToBindingExpr::visit(ExprSearchedCase& expr)
{
    std::vector<std::pair<std::unique_ptr<BindingExpr>, std::unique_ptr<BindingExpr>>> when_clauses;
    for (auto& clause : expr.when_clauses){
        clause.first->accept_visitor(*this);
        auto condition = std::move(tmp);
        clause.second->accept_visitor(*this);
        auto result = std::move(tmp);
        when_clauses.emplace_back(std::make_pair(std::move(condition), std::move(result)));
    }
    std::unique_ptr<BindingExpr> else_binding_expr;
    if (expr.else_clause == nullptr) {
        else_binding_expr = nullptr;
    } else {
        expr.else_clause->accept_visitor(*this);
        else_binding_expr = std::move(tmp);
    }

    tmp = std::make_unique<BindingExprSearchedCase>(std::move(when_clauses), std::move(else_binding_expr));
}

void ExprToBindingExpr::visit(ExprCast& expr)
{
    expr.operand->accept_visitor(*this);
    tmp = std::make_unique<BindingExprCast>(std::move(tmp), std::move(expr.targetType));
}

void ExprToBindingExpr::visit(ExprAggCountAll&) {
    check_and_make_aggregate<AggCountAll>(nullptr);
}

void ExprToBindingExpr::visit(ExprAggCount& expr)
{
    if (expr.distinct) {
        check_and_make_aggregate<AggCountDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggCount>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggAvg& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggAvgDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggAvg>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggMin& expr) {
    check_and_make_aggregate<AggMin>(expr.expr.get());
}

void ExprToBindingExpr::visit(ExprAggMax& expr) {
    check_and_make_aggregate<AggMax>(expr.expr.get());
}

void ExprToBindingExpr::visit(ExprAggSum& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggSumDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggSum>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggStddevPop& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggStddevPopDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggStddevPop>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggStddevSamp& expr) {
    if (expr.distinct) {
        check_and_make_aggregate<AggStddevSampDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggStddevSamp>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggCollect& expr)
{
    if (expr.distinct) {
        check_and_make_aggregate<AggCollectDistinct>(expr.expr.get());
    } else {
        check_and_make_aggregate<AggCollect>(expr.expr.get());
    }
}

void ExprToBindingExpr::visit(ExprAggPercentileCont& expr)
{
    expr.percentile->accept_visitor(*this);
    auto percentile_expr = std::move(tmp);
    if (expr.distinct) {
        check_and_make_aggregate<AggPercentileContDistinct>(expr.expr.get(), std::move(percentile_expr));
    } else {
        check_and_make_aggregate<AggPercentileCont>(expr.expr.get(), std::move(percentile_expr));
    }
}

void ExprToBindingExpr::visit(ExprAggPercentileDisc& expr)
{
    expr.percentile->accept_visitor(*this);
    auto percentile_expr = std::move(tmp);
    if (expr.distinct) {
        check_and_make_aggregate<AggPercentileDiscDistinct>(expr.expr.get(), std::move(percentile_expr));
    } else {
        check_and_make_aggregate<AggPercentileDisc>(expr.expr.get(), std::move(percentile_expr));
    }
}

template<typename AggType, class ... Args>
void ExprToBindingExpr::check_and_make_aggregate(Expr* expr, Args&&... args) {
    if (bic == nullptr) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    if (inside_aggregation) {
        throw QuerySemanticException("Nested aggregations are not allowed");
    }

    VarId var(0);
    if (at_root && as_var.has_value()) {
        // If we are at the root of the expression, and the expressions is associated with a variable
        // (<expr> AS <var>), then we can avoid creating an unnecessary internal variable.
        var = as_var.value();
    } else {
        // Otherwise we have to create an internal variable to associated with the aggregation.
        var = get_query_ctx().get_internal_var();
    }

    at_root = false;
    inside_aggregation = true;

    std::unique_ptr<AggType> agg;
    if (expr) {
        expr->accept_visitor(*this);
        agg = std::make_unique<AggType>(var, std::move(tmp), std::forward<Args>(args)...);
    } else {
        agg = std::make_unique<AggType>(var, nullptr, std::forward<Args>(args)...);
    }

    bic->aggregations.insert({var, std::move(agg)});

    // auto agg_ptr = static_cast<AggType*>(bic->aggregations.at(var).get());

    tmp = std::make_unique<BindingExprVar>(var);

    // We are done with this branch of the expressions tree.
    // This aggregation does not count for any other branches, which have their own aggregations.
    inside_aggregation = false;
}
