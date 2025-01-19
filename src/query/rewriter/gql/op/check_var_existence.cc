#include "check_var_existence.h"

#include "query/parser/op/gql/ops.h"

namespace GQL {

void CheckVarExistence::visit(GQL::OpGraphPattern& op_graph_pattern)
{
    op_graph_pattern.op->accept_visitor(*this);

    if (op_graph_pattern.path_var_id.has_value()) {
        if (variables.count(op_graph_pattern.path_var_id.value())) {
            throw QuerySemanticException("Path variable cannot appear in the graph pattern.");
        }

        variables.insert(op_graph_pattern.path_var_id.value());
    }
}

void CheckVarExistence::visit(GQL::OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);

    std::set<VarId> expr_variables;

    for (auto& item : op_return.return_items) {
        for (auto var : item.expr->get_all_vars()) {
            expr_variables.insert(var);
        }
    }

    for (auto& expr_variable : expr_variables) {
        if (!variables.count(expr_variable)) {
            throw QuerySemanticException(
                "Condition expression contains the variable \"" + get_query_ctx().get_var_name(expr_variable)
                + "\" which does not appear in the pattern expression."
            );
        }
    }
}

void CheckVarExistence::visit(GQL::OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
    std::set<VarId> order_by_variables = std::move(variables);

    std::set<VarId> expr_variables;

    for (auto& expr : op_order_by.items) {
        for (auto var : expr->get_all_vars()) {
            expr_variables.insert(var);
        }
    }

    for (auto& expr_variable : expr_variables) {
        if (!order_by_variables.count(expr_variable)) {
            throw QuerySemanticException(
                "Condition expression contains the variable \"" + get_query_ctx().get_var_name(expr_variable)
                + "\" which does not appear in the pattern expression."
            );
        }
    }

    variables = std::move(order_by_variables);
    variables.merge(expr_variables);
}

void CheckVarExistence::visit(GQL::OpGraphPatternList& op_graph_pattern_list)
{
    std::set<VarId> subvariables;
    for (auto& graph_pattern : op_graph_pattern_list.patterns) {
        graph_pattern->accept_visitor(*this);

        subvariables.merge(variables);
        variables.clear();
    }

    variables = std::move(subvariables);
}

void CheckVarExistence::visit(GQL::OpPathUnion& op_path_union)
{
    std::set<VarId> subvariables;
    for (auto& graph_pattern : op_path_union.op_list) {
        graph_pattern->accept_visitor(*this);

        subvariables.merge(variables);
        variables.clear();
    }

    variables = std::move(subvariables);
}

void CheckVarExistence::visit(GQL::OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);
    std::set<VarId> filter_variables = std::move(variables);

    std::set<VarId> expr_variables;

    for (auto& expr : op_filter.exprs) {
        for (auto var : expr->get_all_vars()) {
            expr_variables.insert(var);
        }
    }

    for (auto& expr_variable : expr_variables) {
        std::string var_name = get_query_ctx().get_var_name(expr_variable);
        size_t pos = var_name.find('.');

        if (pos != var_name.npos && pos != 0) {
            var_name = std::string(var_name, 0, pos);
            bool found;
            VarId new_expr_var = get_query_ctx().get_var(var_name, &found);
            if (!found || !filter_variables.count(new_expr_var)) {
                throw QuerySemanticException(
                    "Condition expression contains the variable \"" + var_name
                    + "\" which does not appear in the pattern expression."
                );
            }
        }

        else if (!filter_variables.count(expr_variable))
        {
            throw QuerySemanticException(
                "Condition expression contains the variable \"" + get_query_ctx().get_var_name(expr_variable)
                + "\" which does not appear in the pattern expression."
            );
        }
    }

    variables = std::move(filter_variables);
    variables.merge(expr_variables);
}

void CheckVarExistence::visit(GQL::OpOptProperties& op_property)
{
    op_property.op->accept_visitor(*this);

    for (auto& property : op_property.properties) {
        variables.insert(property.object);
        variables.insert(property.value);
    }
}

void CheckVarExistence::visit(GQL::OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::set<VarId> subvariables;
    for (auto& graph_pattern : op_basic_graph_pattern.patterns) {
        graph_pattern->accept_visitor(*this);

        subvariables.merge(variables);
        variables.clear();
    }

    variables = std::move(subvariables);
}

void CheckVarExistence::visit(GQL::OpProperty& op_property)
{
    for (auto& property : op_property.properties) {
        variables.insert(property.object);
    }
}

void CheckVarExistence::visit(OpNode& op_node)
{
    variables.insert(op_node.id);
}

void CheckVarExistence::visit(OpEdge& op_edge)
{
    variables.insert(op_edge.id);
}

} // namespace GQL
