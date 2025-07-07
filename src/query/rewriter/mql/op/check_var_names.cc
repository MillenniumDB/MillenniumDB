#include "check_var_names.h"

#include "query/exceptions.h"
#include "query/parser/expr/mql/exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void CheckVarNames::insert_joinable_var(VarId var)
{
    if (unjoinable_vars.contains(var)) {
        throw QuerySemanticException("Variable \"" + get_query_ctx().get_var_name(var) + "\" is unjoinable");
    }

    declared_vars.emplace(var);
}

void CheckVarNames::insert_unjoinable_var(VarId var)
{
    if (!declared_vars.insert(var).second) {
        throw QuerySemanticException("Variable \"" + get_query_ctx().get_var_name(var) + "\" is unjoinable");
    }

    unjoinable_vars.insert(var);
}

void CheckVarNames::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);

    for (const auto& expr : op_group_by.exprs) {
        CheckVarNamesExpr visitor(declared_vars, unjoinable_vars, alias_vars);
        expr->accept_visitor(visitor);
    }
}

void CheckVarNames::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);

    for (const auto& [expr, var] : op_return.projection) {
        CheckVarNamesExpr visitor(declared_vars, unjoinable_vars, alias_vars);
        expr->accept_visitor(visitor);

        auto expr_vars = expr->get_all_vars();

        if (expr_vars.find(var) == expr_vars.end()) {
            // alias
            if (declared_vars.contains(var) || alias_vars.contains(var)) {
                throw QuerySemanticException(
                    "Variable \"" + get_query_ctx().get_var_name(var) + "\" cannot be re-declared"
                );
            }
            alias_vars.insert(var);
        }
    }
}

void CheckVarNames::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    for (const auto& item : op_order_by.items) {
        CheckVarNamesExpr visitor(declared_vars, unjoinable_vars, alias_vars);
        item->accept_visitor(visitor);
    }
}

void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    for (const auto& label : op_basic_graph_pattern.labels) {
        for (const auto& var : label.get_all_vars()) {
            insert_joinable_var(var);
        }
    }

    for (const auto& property : op_basic_graph_pattern.properties) {
        for (const auto& var : property.get_all_vars()) {
            insert_joinable_var(var);
        }
    }

    for (const auto& edge : op_basic_graph_pattern.edges) {
        for (const auto& var : edge.get_all_vars()) {
            insert_joinable_var(var);
        }
    }

    for (const auto& disjoint_var : op_basic_graph_pattern.disjoint_vars) {
        for (const auto& var : disjoint_var.get_all_vars()) {
            insert_joinable_var(var);
        }
    }

    for (const auto& path : op_basic_graph_pattern.paths) {
        if (path.from.is_var()) {
            insert_joinable_var(path.from.get_var());
        }
        if (path.to.is_var()) {
            insert_joinable_var(path.to.get_var());
        }
        insert_unjoinable_var(path.var);
    }
}

void CheckVarNames::visit(OpCall& op_call)
{
    for (const auto& expr : op_call.argument_exprs) {
        CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars, alias_vars);
        expr->accept_visitor(expr_visitor);
    }

    for (const auto& var : op_call.yield_vars) {
        if (declared_vars.contains(var)) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(var) + "\" cannot be re-declared"
            );
        }
        insert_joinable_var(var);
    }
}

void CheckVarNames::visit(OpLet& op_let)
{
    for (const auto& [var, expr] : op_let.var_expr) {
        CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars, alias_vars);
        expr->accept_visitor(expr_visitor);

        if (declared_vars.contains(var)) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(var) + "\" cannot be re-declared"
            );
        }
        insert_joinable_var(var);
    }
}

void CheckVarNames::visit(OpOptional& op_optional)
{
    op_optional.op->accept_visitor(*this);
    for (const auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}

void CheckVarNames::visit(OpSequence& op_sequence)
{
    for (const auto& op : op_sequence.sequence) {
        op->accept_visitor(*this);
    }
}

void CheckVarNames::visit(OpWhere& op_where)
{
    op_where.op->accept_visitor(*this);
    CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars, alias_vars);
    op_where.expr->accept_visitor(expr_visitor);
}

/*************************** ExprVisitor ***************************/
void CheckVarNamesExpr::visit(ExprVar& expr)
{
    if (!declared_vars.contains(expr.var)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var) + "\" is not declared"
        );
    }

    if (alias_vars.contains(expr.var)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var) + "\" is cannot be re-declared"
        );
    }
}

void CheckVarNamesExpr::visit(ExprVarProperty& expr)
{
    if (!declared_vars.contains(expr.var_without_property)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var_without_property) + "\" is not declared"
        );
    }

    if (unjoinable_vars.contains(expr.var_without_property)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" is unjoinable and cannot have properties"
        );
    }

    if (alias_vars.contains(expr.var_without_property)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" is a return alias and cannot have properties"
        );
    }
}

void CheckVarNamesExpr::visit(ExprAddition& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprDivision& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprModulo& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprMultiplication& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprSubtraction& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprGreaterOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprGreater& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprLessOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprLess& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprNotEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprUnaryMinus& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprUnaryPlus& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprNot& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprIs& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprAnd& expr)
{
    for (auto& e : expr.and_list) {
        e->accept_visitor(*this);
    }
}

void CheckVarNamesExpr::visit(ExprOr& expr)
{
    for (auto& e : expr.or_list) {
        e->accept_visitor(*this);
    }
}

void CheckVarNamesExpr::visit(ExprRegex& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);

    if (expr.expr3 != nullptr) {
        expr.expr3->accept_visitor(*this);
    }
}

void CheckVarNamesExpr::visit(ExprCosineSimilarity& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprCosineDistance& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprManhattanDistance& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprEuclideanDistance& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprAggAvg& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprAggCountAll&)
{
    // Do nothing
}

void CheckVarNamesExpr::visit(ExprAggCount&)
{
    // Do nothing
}

void CheckVarNamesExpr::visit(ExprAggMax& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprAggMin& expr)
{
    expr.expr->accept_visitor(*this);
}

void CheckVarNamesExpr::visit(ExprAggSum& expr)
{
    expr.expr->accept_visitor(*this);
}
