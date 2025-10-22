#include "replace_parameters.h"

#include "query/parser/op/gql/ops.h"
#include "query/rewriter/gql/expr/replace_parameters_expr.h"

using namespace GQL;

void ReplaceParameters::visit(OpGraphPattern& op_graph_pattern)
{
    op_graph_pattern.op->accept_visitor(*this);

    if (op_graph_pattern.path_var_id.has_value()) {
        const auto path_var_id = op_graph_pattern.path_var_id.value();
        if (parameters.find(path_var_id) != parameters.end()) {
            throw QuerySemanticException(
                "Path variable ?" + get_query_ctx().get_var_name(path_var_id) + " cannot be a parameter"
            );
        }
    }
}

void ReplaceParameters::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    for (auto& op : op_basic_graph_pattern.patterns) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpGraphPatternList& op_graph_pattern_list)
{
    for (auto& op : op_graph_pattern_list.patterns) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpNode& op_node)
{
    if (parameters.find(op_node.id) != parameters.end()) {
        throw QuerySemanticException(
            "Node variable ?" + get_query_ctx().get_var_name(op_node.id) + " cannot be a parameter"
        );
    }
}

void ReplaceParameters::visit(OpEdge& op_edge)
{
    if (parameters.find(op_edge.id) != parameters.end()) {
        throw QuerySemanticException(
            "Edge variable ?" + get_query_ctx().get_var_name(op_edge.id) + " cannot be a parameter"
        );
    }

    if (parameters.find(op_edge.from) != parameters.end()) {
        throw QuerySemanticException(
            "Edge source variable ?" + get_query_ctx().get_var_name(op_edge.from) + " cannot be a parameter"
        );
    }

    if (parameters.find(op_edge.to) != parameters.end()) {
        throw QuerySemanticException(
            "Edge target variable ?" + get_query_ctx().get_var_name(op_edge.id) + " cannot be a parameter"
        );
    }

    if (parameters.find(op_edge.direction_var) != parameters.end()) {
        throw QuerySemanticException(
            "Edge direction variable ?" + get_query_ctx().get_var_name(op_edge.id) + " cannot be a parameter"
        );
    }
}

void ReplaceParameters::visit(OpWhere& op_where)
{
    op_where.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_where.exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
    if (op_return.op_order_by) {
        op_return.op_order_by->accept_visitor(*this);
    }

    ReplaceParametersExpr visitor(parameters);
    for (auto& item : op_return.return_items) {
        if (item.alias.has_value()) {
            const auto& var_id = item.alias.value();
            if (parameters.find(var_id) != parameters.end()) {
                throw QuerySemanticException(
                    "RETURN variable ?" + get_query_ctx().get_var_name(var_id) + " cannot be a parameter"
                );
            }
        } else {
            // TODO: replace posible projection
        }
    }
}

void ReplaceParameters::visit(OpPathUnion& op_path_union)
{
    for (auto& op : op_path_union.op_list) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpRepetition& op_repetition)
{
    op_repetition.op->accept_visitor(*this);
}

void ReplaceParameters::visit(OpLinearPattern& op_linear_pattern)
{
    for (auto& op : op_linear_pattern.patterns) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpFilter& op_filter)
{
    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_filter.exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpLet& op_let)
{
    ReplaceParametersExpr visitor(parameters);
    for (auto& item : op_let.items) {
        if (parameters.find(item.var_id) != parameters.end()) {
            throw QuerySemanticException(
                "LET variable ?" + get_query_ctx().get_var_name(item.var_id) + " cannot be a parameter"
            );
        }
        visitor.visit_or_replace_parameter(item.expr);
    }
}

void ReplaceParameters::visit(OpOrderBy& op_order_by)
{
    ReplaceParametersExpr visitor(parameters);
    for (auto& item : op_order_by.items) {
        visitor.visit_or_replace_parameter(item);
    }
}

void ReplaceParameters::visit(OpQueryStatements& op_query_statements)
{
    for (auto& op : op_query_statements.ops) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpGroupBy& op_group_by)
{
    op_group_by.accept_visitor(*this);
    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_group_by.exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpUnitTable&) { }

void ReplaceParameters::visit(OpEmpty&) { }
