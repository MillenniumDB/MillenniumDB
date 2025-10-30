#include "evaluate_constants.h"

#include "query/optimizer/quad_model/expr_to_binding_expr.h"
#include "query/parser/expr/mql/exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void EvaluateConstants::visit(OpCall& op_call)
{
    EvaluateConstantsExpr visitor;
    for (auto& expr : op_call.argument_exprs) {
        visitor.visit_and_try_eval_expr(expr);
    }
}

void EvaluateConstants::visit(OpLet& op_let)
{
    EvaluateConstantsExpr visitor;
    for (auto& [var, expr] : op_let.var_expr) {
        visitor.visit_and_try_eval_expr(expr);
    }
}

void EvaluateConstants::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);

    EvaluateConstantsExpr visitor;
    for (auto& expr : op_group_by.exprs) {
        visitor.visit_and_try_eval_expr(expr);
    }
}

void EvaluateConstants::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);

    EvaluateConstantsExpr visitor;
    visitor.visit_and_try_eval_expr(op_having.expr);
}

void EvaluateConstants::visit(OpOptional& op_optional)
{
    op_optional.op->accept_visitor(*this);

    for (auto& op : op_optional.optionals) {
        op->accept_visitor(*this);
    }
}

void EvaluateConstants::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    EvaluateConstantsExpr visitor;
    for (auto& expr : op_order_by.items) {
        visitor.visit_and_try_eval_expr(expr);
    }
}

void EvaluateConstants::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);

    EvaluateConstantsExpr visitor;
    for (auto& [expr, var_id] : op_return.projection) {
        visitor.visit_and_try_eval_expr(expr);
    }
}

void EvaluateConstants::visit(OpSequence& op_sequence)
{
    for (auto& op : op_sequence.sequence) {
        op->accept_visitor(*this);
    }
}

void EvaluateConstants::visit(OpUpdate& op_update)
{
    op_update.op->accept_visitor(*this);

    EvaluateConstantsUpdateAction visitor;
    for (auto& update_action : op_update.update_actions) {
        update_action->accept_visitor(visitor);
    }
}

void EvaluateConstants::visit(OpWhere& op_where)
{
    op_where.op->accept_visitor(*this);

    EvaluateConstantsExpr visitor;
    visitor.visit_and_try_eval_expr(op_where.expr);
}

void EvaluateConstantsExpr::visit_and_try_eval_expr(std::unique_ptr<Expr>& expr)
{
    // recurse children if any
    expr->accept_visitor(*this);

    if (expr->has_aggregation()) {
        // aggregation, cannot evaluate
        return;
    }

    if (!expr->get_all_vars().empty()) {
        // expr has variables, cannot evaluate
        return;
    }

    if (dynamic_cast<ExprConstant*>(expr.get()) != nullptr) {
        // expr is already a constant, nothing to do
        return;
    }

    // convert expr to binding_expr
    ExprToBindingExpr expr_to_binding_expr(nullptr, {}, false);
    expr->accept_visitor(expr_to_binding_expr);
    auto binding_expr = std::move(expr_to_binding_expr.tmp);

    // evaluate and replace expr
    const auto oid = binding_expr->eval(binding);
    expr = std::make_unique<ExprConstant>(oid);
}

void EvaluateConstantsExpr::visit(ExprAddition& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprDivision& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprModulo& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprMultiplication& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprSubtraction& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprUnaryMinus& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprUnaryPlus& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprEquals& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprGreaterOrEquals& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprGreater& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprIs& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprLessOrEquals& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprLess& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprNotEquals& expr)
{
    visit_and_try_eval_expr(expr.lhs);
    visit_and_try_eval_expr(expr.rhs);
}

void EvaluateConstantsExpr::visit(ExprAnd& expr)
{
    for (auto& e : expr.and_list) {
        visit_and_try_eval_expr(e);
    }
}

void EvaluateConstantsExpr::visit(ExprNot& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprOr& expr)
{
    for (auto& e : expr.or_list) {
        visit_and_try_eval_expr(e);
    }
}

void EvaluateConstantsExpr::visit(ExprRegex& expr)
{
    visit_and_try_eval_expr(expr.expr1);
    visit_and_try_eval_expr(expr.expr2);
    if (expr.expr3) {
        visit_and_try_eval_expr(expr.expr3);
    }
}

void EvaluateConstantsExpr::visit(ExprCosineSimilarity& expr)
{
    visit_and_try_eval_expr(expr.expr1);
    visit_and_try_eval_expr(expr.expr2);
}

void EvaluateConstantsExpr::visit(ExprCosineDistance& expr)
{
    visit_and_try_eval_expr(expr.expr1);
    visit_and_try_eval_expr(expr.expr2);
}

void EvaluateConstantsExpr::visit(ExprManhattanDistance& expr)
{
    visit_and_try_eval_expr(expr.expr1);
    visit_and_try_eval_expr(expr.expr2);
}

void EvaluateConstantsExpr::visit(ExprEuclideanDistance& expr)
{
    visit_and_try_eval_expr(expr.expr1);
    visit_and_try_eval_expr(expr.expr2);
}

void EvaluateConstantsExpr::visit(ExprEditDistance& expr)
{
    visit_and_try_eval_expr(expr.expr1);
    visit_and_try_eval_expr(expr.expr2);
}

void EvaluateConstantsExpr::visit(ExprNormalize& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprStr& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprLabels& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprType& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprProperties& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprAggAvg& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprAggCountAll&) { }

void EvaluateConstantsExpr::visit(ExprAggCount& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprAggMax& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprAggMin& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsExpr::visit(ExprAggSum& expr)
{
    visit_and_try_eval_expr(expr.expr);
}

void EvaluateConstantsUpdateAction::visit(InsertPropertyExpr& insert_property_expr)
{
    EvaluateConstantsExpr visitor;
    visitor.visit_and_try_eval_expr(insert_property_expr.value);
}
