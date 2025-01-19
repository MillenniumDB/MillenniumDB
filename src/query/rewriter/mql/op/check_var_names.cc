#include "check_var_names.h"

#include "query/exceptions.h"
#include "query/parser/expr/mql_exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void CheckVarNames::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);

    for (auto var : op_group_by.vars) {
        if (declared_vars.find(var) == declared_vars.end()) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(var) + "\" not declared"
            );
        }
    }
}

void CheckVarNames::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);

    for (auto op_property : optional_properties) {
        if (op_property.node.is_var()) {
            if (declared_vars.find(op_property.node.get_var()) == declared_vars.end()) {
                throw QuerySemanticException(
                    "Variable \"" + get_query_ctx().get_var_name(op_property.node.get_var())
                    + "\" not declared"
                );
            }
        }
        if (op_property.value.is_var()) {
            declared_vars.insert(op_property.value.get_var());
        }
    }

    for (const auto& [var, expr] : op_return.projection) {
        if (expr) {
            // Projected expression
            CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars, return_vars);
            expr->accept_visitor(expr_visitor);

            if (declared_vars.find(var) != declared_vars.end() || return_vars.find(var) != return_vars.end())
            {
                throw QuerySemanticException(
                    "Alias for variable \"" + get_query_ctx().get_var_name(var) + "\" already declared"
                );
            }

            return_vars.insert(var);
        } else {
            if (declared_vars.find(var) == declared_vars.end()) {
                throw QuerySemanticException(
                    "Variable \"" + get_query_ctx().get_var_name(var) + "\" not declared"
                );
            }
        }
    }
}

void CheckVarNames::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    for (const auto& item : op_order_by.items) {
        if (std::holds_alternative<VarId>(item)) {
            auto& var = std::get<VarId>(item);
            if (declared_vars.find(var) == declared_vars.end()) {
                throw QuerySemanticException(
                    "Variable \"" + get_query_ctx().get_var_name(var) + "\" not declared"
                );
            }
        } else {
            auto& expr = std::get<std::unique_ptr<Expr>>(item);
            CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars, return_vars);
            expr->accept_visitor(expr_visitor);
        }
    }
}

void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    // Get optional_properties of OpbasicGraphPattern
    for (auto property : optional_properties) {
        for (auto var : op_basic_graph_pattern.get_all_vars()) {
            if (var == property.node.get_var()
                && used_properties.find(property.value) == used_properties.end()) {
                op_basic_graph_pattern.optional_properties.push_back(property);
                used_properties.emplace(property.value);
            }
        }
    }

    auto insert_joinable_var = [&](const VarId var) {
        declared_vars.insert(var);
        if (unjoinable_vars.find(var) != unjoinable_vars.end())
            throw QuerySemanticException(
                "Duplicated unjoinable variable \"" + get_query_ctx().get_var_name(var) + "\""
            );
    };
    auto insert_unjoinable_var = [&](const VarId var) {
        if (!declared_vars.insert(var).second)
            throw QuerySemanticException(
                "Duplicated unjoinable variable \"" + get_query_ctx().get_var_name(var) + "\""
            );
        unjoinable_vars.insert(var);
    };
    auto insert_joinable_vars = [&](const std::set<VarId>& vars) {
        for (auto& var : vars)
            insert_joinable_var(var);
    };

    for (auto& label : op_basic_graph_pattern.labels) {
        insert_joinable_vars(label.get_all_vars());
    }
    for (auto& property : op_basic_graph_pattern.properties) {
        insert_joinable_vars(property.get_all_vars());
    }
    for (auto& edge : op_basic_graph_pattern.edges) {
        insert_joinable_vars(edge.get_all_vars());
    }
    for (auto& disjoint_var : op_basic_graph_pattern.disjoint_vars) {
        insert_joinable_vars(disjoint_var.get_all_vars());
    }

    for (auto& path : op_basic_graph_pattern.paths) {
        if (path.from.is_var())
            insert_joinable_var(path.from.get_var());
        if (path.to.is_var())
            insert_joinable_var(path.to.get_var());
        insert_unjoinable_var(path.var);
    }

    for (auto op_property : op_basic_graph_pattern.optional_properties) {
        if (op_property.node.is_var()) {
            if (declared_vars.find(op_property.node.get_var()) == declared_vars.end()) {
                throw QuerySemanticException(
                    "Variable \"" + get_query_ctx().get_var_name(op_property.node.get_var())
                    + "\" not declared"
                );
            }
        }
        if (op_property.value.is_var()) {
            declared_vars.insert(op_property.value.get_var());
        }
    }
}

void CheckVarNames::visit(OpOptional& op_optional)
{
    op_optional.op->accept_visitor(*this);
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}

void CheckVarNames::visit(OpWhere& op_where)
{
    op_where.op->accept_visitor(*this);
    CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars, return_vars);
    op_where.expr->accept_visitor(expr_visitor);
}

void CheckVarNames::visit(OpSet& op_set)
{
    op_set.op->accept_visitor(*this);
}

/*************************** ExprVisitor ***************************/
void CheckVarNamesExpr::visit(ExprVar& expr)
{
    if (declared_vars.find(expr.var) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var)
            + "\" used in WHERE is not declared in MATCH"
        );
    }

    if (return_vars.find(expr.var) != return_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var) + "\" is a projected expression"
        );
    }

    if (return_vars.find(expr.var) != return_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var) + "\" is a projected expression"
        );
    }
}

void CheckVarNamesExpr::visit(ExprVarProperty& expr)
{
    if (declared_vars.find(expr.var_without_property) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" used in WHERE is not declared in MATCH"
        );
    }
    if (return_vars.find(expr.var_without_property) != return_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" is a projected expression"
        );
    }

    if (unjoinable_vars.find(expr.var_without_property) != unjoinable_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" is unjoinable and cannot have properties"
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

void CheckVarNamesExpr::visit(ExprTensorDistance& expr)
{
    expr.expr->accept_visitor(*this);

    if (expr.expr_ref != nullptr) {
        expr.expr_ref->accept_visitor(*this);
    }
}

void CheckVarNamesExpr::visit(ExprTextSearch& expr)
{
    if (declared_vars.find(expr.object_var) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \"" + get_query_ctx().get_var_name(expr.object_var)
            + "\" used in WHERE is not declared in MATCH"
        );
    }

    for (const auto& var : expr.get_all_vars()) {
        if (return_vars.find(var) != return_vars.end()) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(var) + "\" is a projected expression"
            );
        }
        if (unjoinable_vars.find(var) != unjoinable_vars.end()) {
            throw QuerySemanticException(
                "Variable \"" + get_query_ctx().get_var_name(var)
                + "\" is unjoinable and cannot have properties"
            );
        }
    }

    declared_vars.insert(expr.match_var);
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
