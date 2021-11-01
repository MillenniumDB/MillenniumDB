#include "check_var_names.h"

#include "base/exceptions.h"
#include "base/parser/logical_plan/op/op_basic_graph_pattern.h"
#include "base/parser/logical_plan/op/op_distinct.h"
#include "base/parser/logical_plan/op/op_group_by.h"
#include "base/parser/logical_plan/op/op_match.h"
#include "base/parser/logical_plan/op/op_optional.h"
#include "base/parser/logical_plan/op/op_order_by.h"
#include "base/parser/logical_plan/op/op_select.h"
#include "base/parser/logical_plan/op/op_where.h"
#include "base/parser/logical_plan/op/visitors/check_var_names_formula.h"

void CheckVarNames::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);

    for (auto& select_item : op_select.select_items) {
        if (declared_vars.find(Var(select_item.var)) == declared_vars.end()) {
            throw QuerySemanticException("Variable \"" + select_item.var +
                "\" used in SELECT is not declared in MATCH");
        }
    }
}


void CheckVarNames::visit(OpOptional& op_optional) {
    op_optional.op->accept_visitor(*this);
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}


void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    for (const auto& var : op_basic_graph_pattern.vars) {
        declared_vars.emplace(var.name);
    }
}


void CheckVarNames::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);

    CheckVarNamesFormula formula_visitor(declared_vars);
    formula_visitor(op_where.formula_disjunction);
}


void CheckVarNames::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpMatch& op_match) {
     op_match.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpDistinct& op_distinct) {
    op_distinct.op->accept_visitor(*this);
}
