#include "check_var_names.h"

#include "query/exceptions.h"
#include "query/parser/expr/mql_exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void CheckVarNames::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);

    for (auto var : op_group_by.vars) {
        if (declared_vars.find(var) == declared_vars.end()) {
            throw QuerySemanticException("Variable \""
                + get_query_ctx().get_var_name(var)
                + "\" not declared");
        }
    }
}


void CheckVarNames::visit(OpReturn& op_return) {
    op_return.op->accept_visitor(*this);

    for (auto&& [var, expr] : op_return.projection) {
        if (expr) {
            CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars);
            expr->accept_visitor(expr_visitor);
        } else {
            if (declared_vars.find(var) == declared_vars.end()) {
                throw QuerySemanticException("Variable \""
                    + get_query_ctx().get_var_name(var)
                    + "\" not declared");
            }
        }
    }
}


void CheckVarNames::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);

    for (auto&& [var, expr] : op_order_by.items) {
        if (expr) {
            CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars);
            expr->accept_visitor(expr_visitor);
        } else {
            if (declared_vars.find(var) == declared_vars.end()) {
                throw QuerySemanticException("Variable \""
                    + get_query_ctx().get_var_name(var)
                    + "\" not declared");
            }
        }
    }
}


void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    auto insert_joinable_var = [&](const VarId var) {
        declared_vars.insert(var);
        if (unjoinable_vars.find(var) != unjoinable_vars.end())
            throw QuerySemanticException("Duplicated unjoinable variable \"" + get_query_ctx().get_var_name(var) + "\"");
    };
    auto insert_unjoinable_var = [&](const VarId var) {
        if (!declared_vars.insert(var).second)
            throw QuerySemanticException("Duplicated unjoinable variable \"" + get_query_ctx().get_var_name(var) + "\"");
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
    for (auto& similarity_search : op_basic_graph_pattern.similarity_searches) {
        insert_joinable_var(similarity_search.object_var);
        insert_unjoinable_var(similarity_search.similarity_var);
    }

    for (auto& path : op_basic_graph_pattern.paths) {
        if (path.from.is_var())
            insert_joinable_var(path.from.get_var());
        if (path.to.is_var())
            insert_joinable_var(path.to.get_var());
        insert_unjoinable_var(path.var);
    }
}


void CheckVarNames::visit(OpOptional& op_optional) {
    op_optional.op->accept_visitor(*this);
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}


void CheckVarNames::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
    CheckVarNamesExpr expr_visitor(declared_vars, unjoinable_vars);
    op_where.expr->accept_visitor(expr_visitor);
}


void CheckVarNames::visit(OpProjectSimilarity& op_project_similarity) {
    op_project_similarity.op->accept_visitor(*this);
    if (declared_vars.find(op_project_similarity.object_var) == declared_vars.end()) {
        throw QuerySemanticException("Variable \""
            + get_query_ctx().get_var_name(op_project_similarity.object_var)
            + "\" not declared for similarity projection");
    }
    if (!declared_vars.insert(op_project_similarity.similarity_var).second)
        throw QuerySemanticException("Duplicated unjoinable variable \""
                                     + get_query_ctx().get_var_name(op_project_similarity.similarity_var) + "\"");
    unjoinable_vars.insert(op_project_similarity.similarity_var);
}


void CheckVarNames::visit(OpSet& op_set) {
    op_set.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpMatch& op_match) {
    op_match.op->accept_visitor(*this);

    for (auto op_property : op_match.optional_properties) {
        if (op_property.node.is_var()) {
            if (declared_vars.find(op_property.node.get_var()) == declared_vars.end()) {
                throw QuerySemanticException("Variable \""
                    + get_query_ctx().get_var_name(op_property.node.get_var())
                    + "\" not declared");
            }
        }
        if (op_property.value.is_var()) {
            declared_vars.insert(op_property.value.get_var());
        }
    }
}


/*************************** ExprVisitor ***************************/
void CheckVarNamesExpr::visit(ExprVar& expr) {
    if (declared_vars.find(expr.var) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \""
            + get_query_ctx().get_var_name(expr.var)
            + "\" used in WHERE is not declared in MATCH"
        );
    }
}


void CheckVarNamesExpr::visit(ExprVarProperty& expr) {
    if (declared_vars.find(expr.var_without_property) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \""
            + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" used in WHERE is not declared in MATCH"
        );
    }
    if (unjoinable_vars.find(expr.var_without_property) != unjoinable_vars.end()) {
        throw QuerySemanticException(
            "Variable \""
            + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" is unjoinable and cannot have properties");
    }
}


void CheckVarNamesExpr::visit(ExprAddition& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprDivision& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprModulo& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprMultiplication& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprSubtraction& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprGreaterOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprGreater& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprLessOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprLess& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprNotEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprUnaryMinus& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprUnaryPlus& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprNot& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprIs& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAnd& expr) {
    for (auto& e : expr.and_list) {
        e->accept_visitor(*this);
    }
}


void CheckVarNamesExpr::visit(ExprOr& expr) {
    for (auto& e : expr.or_list) {
        e->accept_visitor(*this);
    }
}


void CheckVarNamesExpr::visit(ExprAggAvg& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAggCountAll&) {
    // Do nothing
}


void CheckVarNamesExpr::visit(ExprAggCount&) {
    // Do nothing
}


void CheckVarNamesExpr::visit(ExprAggMax& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAggMin& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAggSum& expr) {
    expr.expr->accept_visitor(*this);
}
