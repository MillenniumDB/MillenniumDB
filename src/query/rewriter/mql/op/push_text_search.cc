#include "push_text_search.h"

#include "query/parser/expr/mql_exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void PushTextSearch::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    for (auto&& expr_text_search : expr_text_searches) {
        op_basic_graph_pattern.add_text_search(std::move(expr_text_search));
    }
}

void PushTextSearch::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_group_by.op = std::move(where_child);
    }
}

void PushTextSearch::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_order_by.op = std::move(where_child);
    }
}

void PushTextSearch::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_return.op = std::move(where_child);
    }
}

void PushTextSearch::visit(OpSet& op_set)
{
    op_set.op->accept_visitor(*this);
}

void PushTextSearch::visit(OpWhere& op_where)
{
    PushTextSearchExpr expr_visitor(expr_text_searches);
    op_where.expr->accept_visitor(expr_visitor);
    op_where.op->accept_visitor(*this);

    if (expr_visitor.can_remove) {
        must_delete_where = true;
        where_child = std::move(op_where.op);
    } else if (expr_visitor.remaining_expr != nullptr) {
        op_where.expr = std::move(expr_visitor.remaining_expr);
    }
}

/******************************************************************************
 *                             PushTextSearchExpr                             *
 ******************************************************************************/

void PushTextSearchExpr::visit(ExprAnd& expr_and)
{
    std::vector<std::unique_ptr<Expr>> remaining_and;

    for (auto& expr : expr_and.and_list) {
        can_remove = true;
        expr->accept_visitor(*this);
        if (!can_remove) {
            if (remaining_expr != nullptr) {
                remaining_and.emplace_back(std::move(remaining_expr));
            } else {
                remaining_and.emplace_back(std::move(expr));
            }
        }
    }

    if (remaining_and.empty()) {
        can_remove = true;
    } else if (remaining_and.size() == 1) {
        remaining_expr = std::move(remaining_and[0]);
        can_remove = false;
    } else {
        expr_and.and_list = std::move(remaining_and);
        can_remove = false;
    }
}

void PushTextSearchExpr::visit(ExprTextSearch& expr_text_search)
{
    expr_text_searches.emplace_back(expr_text_search);
    can_remove = true;
}

void PushTextSearchExpr::visit(ExprAddition&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprDivision&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprEquals&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprGreater&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprGreaterOrEquals&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprIs&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprLess&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprLessOrEquals&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprModulo&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprMultiplication&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprNot&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprNotEquals&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprOr&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprRegex&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprSubtraction&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprTensorDistance&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprUnaryMinus&)
{
    can_remove = false;
}
void PushTextSearchExpr::visit(ExprUnaryPlus&)
{
    can_remove = false;
}