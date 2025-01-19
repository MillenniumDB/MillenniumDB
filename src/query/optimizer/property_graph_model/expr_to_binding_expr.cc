#include "expr_to_binding_expr.h"

#include "query/executor/binding_iter/binding_expr/gql_binding_exprs.h"
#include "query/optimizer/property_graph_model/binding_iter_constructor.h"
#include "query/parser/expr/gql_exprs.h"

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
