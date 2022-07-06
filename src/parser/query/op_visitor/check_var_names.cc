#include "check_var_names.h"

#include "base/exceptions.h"
#include "parser/query/op/ops.h"
#include "parser/query/expr/exprs.h"
#include "parser/query/op_visitor/return_item_check_group.h"
#include "parser/query/op_visitor/return_item_check_var_name.h"
#include "parser/query/return_item/return_item_var.h"

void CheckVarNames::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);

    ReturnItemCheckVarName visitor(declared_vars, declared_path_vars, "GROUP BY");

    // check no repeated var, "GROUP BY ?x, ?x.key" is not possible
    std::set<Var> vars;

    for (auto& var : op_group_by.items) {
        ReturnItemVar item(var);
        item.accept_visitor(visitor);

        auto pos = var.name.find('.');
        if (pos == std::string::npos) {
            if (vars.find(var) == vars.end()) {
                vars.insert(var);
            } else {
                throw QuerySemanticException("Variable \"" + var.name
                                           + "\" can't be used twice in GROUP BY");
            }
        }
        group_var_names.insert(var.name);
    }

    for (auto& var : op_group_by.items) {
        ReturnItemVar item(var);
        item.accept_visitor(visitor);

        auto pos = var.name.find('.');
        if (pos != std::string::npos) {
            // var is like "?x.key" transform into "?x"
            Var var_without_property(var.name.substr(0, pos));
            if (vars.find(var_without_property) == vars.end()) {
                vars.insert(var);
            } else {
                throw QuerySemanticException("Variable \"" + var.name
                                           + "\" can't be used in GROUP BY along with " + var_without_property.name);
            }
        }
    }
}


void CheckVarNames::visit(OpReturn& op_return) {
    op_return.op->accept_visitor(*this);

    ReturnItemCheckVarName visitor(declared_vars, declared_path_vars, "RETURN", op_return.distinct);
    ReturnItemCheckGroup visitor2(group_var_names);

    bool seen_agg = false;
    bool seen_non_agg = false;

    for (auto& item : op_return.return_items) {
        item->accept_visitor(visitor);
        item->accept_visitor(visitor2);
        const auto& varname = item->get_var().name;
        if (varname[0] == '?') {
            // non_agg
            seen_non_agg = true;
            if (group_var_names.size() == 0 && seen_agg) {
                throw QuerySemanticException("Can't mix aggregates with non aggregates in return when there is no group by");
            }
        } else {
            // agg
            seen_agg = true;
            if (group_var_names.size() == 0 && seen_non_agg) {
                throw QuerySemanticException("Can't mix aggregates with non aggregates in return when there is no group by");
            }
        }
    }
}


void CheckVarNames::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);

    ReturnItemCheckVarName visitor(declared_vars, declared_path_vars, "ORDER BY");
    ReturnItemCheckGroup visitor2(group_var_names);
    std::set<Var> vars;
    for (auto& item : op_order_by.items) {
        item->accept_visitor(visitor);
        item->accept_visitor(visitor2);

        auto var = item->get_var();

        // check no repeated vars, "ORDER BY ?x, ?y, ?x" would cause trouble
        if (vars.find(var) == vars.end()) {
            vars.insert(var);
        } else {
            throw QuerySemanticException("Variable \"" + var.name + "\" can't be repeated in ORDER BY");
        }
    }
}


void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    auto insert_vars = [&](const std::set<Var>& vars) {
        for (auto& var : vars) {
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + var.name
                                            + "\". Paths must have an unique variable");
            }
        }
    };

    for (auto& label : op_basic_graph_pattern.labels) {
        insert_vars(label.get_vars());
    }
    for (auto& property : op_basic_graph_pattern.properties) {
        insert_vars(property.get_vars());
    }
    for (auto& edge : op_basic_graph_pattern.edges) {
        insert_vars(edge.get_vars());
    }
    for (auto& isolated_var : op_basic_graph_pattern.isolated_vars) {
        insert_vars(isolated_var.get_vars());
    }

    for (auto& path : op_basic_graph_pattern.paths) {
        if (path.from.is_var()) {
            auto var = path.from.to_var();
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + var.name
                                            + "\". Paths must have an unique variable");
            }
        }

        if (path.to.is_var()) {
            auto var = path.to.to_var();
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + var.name
                                            + "\". Paths must have an unique variable");
            }
        }

        if (!declared_path_vars.insert(path.var).second) {
            throw QuerySemanticException("Duplicated path variable \"" + path.var.name
                                         + "\". Paths must have an unique variable");
        }
        if (!declared_vars.insert(path.var).second) {
            throw QuerySemanticException("Duplicated path variable \"" + path.var.name
                                         + "\". Paths must have an unique variable");
        }
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
    CheckVarNamesExpr expr_visitor(declared_vars, declared_path_vars);
    op_where.expr->accept_visitor(expr_visitor);
}


void CheckVarNames::visit(OpSet& op_set) {
    op_set.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpMatch& op_match) {
    op_match.op->accept_visitor(*this);
}


/*************************** ExprVisitor ***************************/
void CheckVarNamesExpr::visit(ExprVar& expr) {
    if (declared_vars.find(expr.var) == declared_vars.end()) {
        throw QuerySemanticException("Variable \"" + expr.var.name + "\" used in WHERE is not declared in MATCH");
    }
}


void CheckVarNamesExpr::visit(ExprVarProperty& expr) {
    if (declared_vars.find(expr.object_var) == declared_vars.end()) {
        throw QuerySemanticException("Variable \"" + expr.object_var.name
                                        + "\" used in WHERE is not declared in MATCH");
    } else if (declared_path_vars.find(expr.object_var) != declared_path_vars.end()) {
        throw QuerySemanticException("Variable \"" + expr.object_var.name
                                        + "\" is a path and cannot have properties");
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
