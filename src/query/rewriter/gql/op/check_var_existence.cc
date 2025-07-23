#include "check_var_existence.h"

#include "query/parser/op/gql/ops.h"
#include "query/rewriter/gql/expr/check_group_vars.h"

namespace GQL {

void CheckVarExistence::check_expr_variables(const std::set<VarId>& expr_variables)
{
    for (auto& expr_variable : expr_variables) {
        std::string var_name = get_query_ctx().get_var_name(expr_variable);
        size_t pos = var_name.find('.');

        if (pos == 0) {
            continue;
        } else if (pos != var_name.npos && pos != 0) {
            var_name = std::string(var_name, 0, pos);
            bool found;
            VarId new_expr_var = get_query_ctx().get_var(var_name, &found);
            if (!found || !variables.count(new_expr_var)) {
                throw QuerySemanticException(
                    "Condition expression contains the variable \"" + var_name
                    + "\" which does not appear in the pattern expression."
                );
            }
        }

        else if (!variables.count(expr_variable))
        {
            throw QuerySemanticException(
                "Condition expression contains the variable \"" + var_name
                + "\" which does not appear in the pattern expression."
            );
        }
    }
}

void CheckVarExistence::visit(OpQueryStatements& op_statements)
{
    for (auto& op : op_statements.ops) {
        op->accept_visitor(*this);
    }
}

void CheckVarExistence::visit(OpFilterStatement& op_filter)
{
    std::set<VarId> expr_variables;
    for (auto& expr : op_filter.exprs) {
        expr_variables.merge(expr->get_all_vars());
    }
    check_expr_variables(expr_variables);
}

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
    // we store the op return vars, because the OpGroupBy references them
    std::vector<VarId> op_return_vars_vec = op_return.get_expr_vars();
    op_return_vars = std::set<VarId>(op_return_vars_vec.begin(), op_return_vars_vec.end());

    op_return.op->accept_visitor(*this);

    std::set<VarId> expr_variables;
    for (auto& item : op_return.return_items) {
        expr_variables.merge(item.expr->get_all_vars());
    }
    check_expr_variables(expr_variables);

    if (!group_vars.empty()) {
        CheckGroupVars check_group_vars(group_vars);
        for (auto& item : op_return.return_items) {
            if (!item.alias.has_value() || (item.alias.has_value() && !group_vars.count(*item.alias))) {
                item.expr->accept_visitor(check_group_vars);
            }
        }
    }

    // we add the op return vars, because the OpOrderBy that goes after the OpReturn might reference them
    variables.merge(op_return_vars);

    if (op_return.op_order_by != nullptr) {
        op_return.op_order_by->accept_visitor(*this);
    }
}

void CheckVarExistence::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);

    for (auto& expr : op_group_by.exprs) {
        group_vars.merge(expr->get_all_vars());
    }

    std::set<VarId> previous_vars = variables;

    variables.insert(op_return_vars.begin(), op_return_vars.end());

    check_expr_variables(group_vars);
    variables = previous_vars;
}

void CheckVarExistence::visit(GQL::OpLet& op_let)
{
    std::set<VarId> expr_variables;
    for (auto& item : op_let.items) {
        expr_variables.merge(item.expr->get_all_vars());
        let_variables.insert(item.var_id);
    }
    check_expr_variables(expr_variables);

    for (auto& item : op_let.items) {
        bool inserted = variables.insert(item.var_id).second;
        if (!inserted) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(item.var_id)
                + "\" in LET statement cannot be redefined."
            );
        }
    }
}

void CheckVarExistence::visit(GQL::OpOrderByStatement& op_order_by)
{
    std::set<VarId> expr_variables;

    for (auto& expr : op_order_by.items) {
        for (auto var : expr->get_all_vars()) {
            expr_variables.insert(var);
        }
    }
    check_expr_variables(expr_variables);
}

void CheckVarExistence::visit(GQL::OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    std::set<VarId> expr_variables;
    for (auto& expr : op_order_by.items) {
        for (auto var : expr->get_all_vars()) {
            expr_variables.insert(var);
        }
    }
    check_expr_variables(expr_variables);
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

void CheckVarExistence::visit(GQL::OpRepetition& op_repetition)
{
    op_repetition.op->accept_visitor(*this);
}

void CheckVarExistence::visit(GQL::OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    std::set<VarId> expr_variables;

    for (auto& expr : op_filter.exprs) {
        for (auto var : expr->get_all_vars()) {
            expr_variables.insert(var);
        }
    }
    check_expr_variables(expr_variables);
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
    variables.insert(op_property.property.object);
}

void CheckVarExistence::visit(OpNode& op_node)
{
    if (let_variables.count(op_node.id)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(op_node.id)
            + "\" in LET statement cannot be redefined in the graph pattern."
        );
    }
    variables.insert(op_node.id);
}

void CheckVarExistence::visit(OpEdge& op_edge)
{
    if (let_variables.count(op_edge.id)) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(op_edge.id)
            + "\" in LET statement cannot be redefined in the graph pattern."
        );
    }
    variables.insert(op_edge.id);
}

} // namespace GQL
