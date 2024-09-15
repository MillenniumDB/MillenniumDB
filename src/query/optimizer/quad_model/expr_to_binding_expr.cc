#include "expr_to_binding_expr.h"

#include "query/query_context.h"
#include "query/optimizer/quad_model/binding_iter_constructor.h"

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
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_and.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_equals.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_is.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_less.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_less_or_equals.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_modulo.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_not.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_not_equals.h"
#include "query/executor/binding_iter/binding_expr/mql/binding_expr_or.h"
#include "query/parser/expr/mql_exprs.h"

// TODO: generalize or not use SPARQL
#include "query/executor/binding_iter/binding_expr/sparql/binding_expr_addition.h"
#include "query/executor/binding_iter/binding_expr/sparql/binding_expr_division.h"
#include "query/executor/binding_iter/binding_expr/sparql/binding_expr_multiplication.h"
#include "query/executor/binding_iter/binding_expr/sparql/binding_expr_subtraction.h"
#include "query/executor/binding_iter/binding_expr/sparql/binding_expr_unary_minus.h"
#include "query/executor/binding_iter/binding_expr/sparql/binding_expr_unary_plus.h"

using namespace MQL;

void ExprToBindingExpr::visit(ExprVar& expr_var) {
    tmp = std::make_unique<BindingExprVar>(expr_var.var);

    if (bic == nullptr) {
        return;
    }

    bic->group_saved_vars.insert(expr_var.var);
    bic->order_by_saved_vars.insert(expr_var.var);
}


void ExprToBindingExpr::visit(ExprVarProperty& expr_var) {
    if (bic != nullptr) {
        bic->group_saved_vars.insert(expr_var.var_with_property);
        bic->order_by_saved_vars.insert(expr_var.var_with_property);
    }
    tmp = std::make_unique<BindingExprVar>(expr_var.var_with_property);
}


void ExprToBindingExpr::visit(ExprConstant& expr_constant) {
    tmp = std::make_unique<BindingExprTerm>(expr_constant.value);
}


void ExprToBindingExpr::visit(ExprAddition& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<SPARQL::BindingExprAddition>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprDivision& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<SPARQL::BindingExprDivision>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprModulo& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprModulo>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprMultiplication& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<SPARQL::BindingExprMultiplication>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprSubtraction& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<SPARQL::BindingExprSubtraction>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprUnaryMinus& expr) {
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<SPARQL::BindingExprUnaryMinus>(
        std::move(tmp)
    );
}


void ExprToBindingExpr::visit(ExprUnaryPlus& expr) {
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<SPARQL::BindingExprUnaryPlus>(
        std::move(tmp)
    );
}


void ExprToBindingExpr::visit(ExprEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprEquals>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprGreaterOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLessOrEquals>(
        std::move(rhs_binding_expr),
        std::move(lhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprGreater& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLess>(
        std::move(rhs_binding_expr),
        std::move(lhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprIs& expr) {
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprIs>(
        std::move(tmp),
        expr.negation,
        expr.type
    );
}


void ExprToBindingExpr::visit(ExprLessOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLessOrEquals>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprLess& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprLess>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr)
    );
}


void ExprToBindingExpr::visit(ExprNotEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(tmp);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(tmp);

    tmp = std::make_unique<BindingExprNotEquals>(
        std::move(lhs_binding_expr),
        std::move(rhs_binding_expr));
}


void ExprToBindingExpr::visit(ExprAnd& expr) {
    std::vector<std::unique_ptr<BindingExpr>> and_list;
    for (auto& e : expr.and_list) {
        e->accept_visitor(*this);
        if (tmp != nullptr) {
            and_list.push_back(std::move(tmp));
        }
    }
    if (and_list.size() > 1) {
        tmp = std::make_unique<BindingExprAnd>(std::move(and_list));
    } else if (and_list.size() == 1) {
        tmp = std::move(and_list[0]);
    }
}


void ExprToBindingExpr::visit(ExprNot& expr) {
    expr.expr->accept_visitor(*this);
    tmp = std::make_unique<BindingExprNot>(std::move(tmp));
}


void ExprToBindingExpr::visit(ExprOr& expr) {
    std::vector<std::unique_ptr<BindingExpr>> or_list;
    for (auto& e : expr.or_list) {
        e->accept_visitor(*this);
        or_list.push_back(std::move(tmp));
    }
    tmp = std::make_unique<BindingExprOr>(std::move(or_list));
}


void ExprToBindingExpr::visit(MQL::ExprAggAvg& expr) {
    assert(var.has_value() && bic != nullptr);
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    inside_aggregation = true;
    expr.expr->accept_visitor(*this);
    std::unique_ptr<Agg> agg = std::make_unique<AggAvg>(var.value(), std::move(tmp));
    bic->aggregations.insert({var.value(), std::move(agg)});
}


void ExprToBindingExpr::visit(MQL::ExprAggCountAll& expr) {
    assert(var.has_value() && bic != nullptr);
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    inside_aggregation = true;
    std::unique_ptr<Agg> agg;
    if (expr.distinct) {
        std::vector<VarId> vars;

        for (auto var : get_query_ctx().get_non_internal_vars()) {
            vars.push_back(var);
        }
        agg = std::make_unique<AggCountAllDistinct>(
            var.value(),
            std::move(vars)
        );
    } else {
        agg = std::make_unique<AggCountAll>(var.value());
    }
    bic->aggregations.insert({var.value(), std::move(agg)});

    auto all_vars = get_query_ctx().get_all_vars();

    if (bic != nullptr) {
        for (auto var : all_vars) {
            bic->group_saved_vars.insert(var);
        }
        for (auto var : all_vars) {
            bic->order_by_saved_vars.insert(var);
        }
    }
}


void ExprToBindingExpr::visit(MQL::ExprAggCount& expr) {
    assert(var.has_value() && bic != nullptr);
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    inside_aggregation = true;
    std::unique_ptr<Agg> agg;
    expr.expr->accept_visitor(*this);
    if (expr.distinct) {
        agg = std::make_unique<AggCountDistinct>(var.value(), std::move(tmp));
    } else {
        agg = std::make_unique<AggCount>(var.value(), std::move(tmp));
    }
    bic->aggregations.insert({var.value(), std::move(agg)});
}


void ExprToBindingExpr::visit(MQL::ExprAggMax& expr) {
    assert(var.has_value() && bic != nullptr);
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    inside_aggregation = true;
    expr.expr->accept_visitor(*this);
    std::unique_ptr<Agg> agg = std::make_unique<AggMax>(var.value(), std::move(tmp));
    bic->aggregations.insert({var.value(), std::move(agg)});
}


void ExprToBindingExpr::visit(MQL::ExprAggMin& expr) {
    assert(var.has_value() && bic != nullptr);
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    inside_aggregation = true;
    expr.expr->accept_visitor(*this);
    std::unique_ptr<Agg> agg = std::make_unique<AggMin>(var.value(), std::move(tmp));
    bic->aggregations.insert({var.value(), std::move(agg)});
}


void ExprToBindingExpr::visit(MQL::ExprAggSum& expr) {
    assert(var.has_value() && bic != nullptr);
    if (!bic->grouping) {
        throw QuerySemanticException("Aggregation where it is not allowed");
    }
    inside_aggregation = true;
    expr.expr->accept_visitor(*this);
    std::unique_ptr<Agg> agg = std::make_unique<AggSum>(var.value(), std::move(tmp));
    bic->aggregations.insert({var.value(), std::move(agg)});
}
