#include "check_well_designed.h"

#include "base/exceptions.h"
#include "base/parser/logical_plan/op/op_basic_graph_pattern.h"
#include "base/parser/logical_plan/op/op_distinct.h"
#include "base/parser/logical_plan/op/op_group_by.h"
#include "base/parser/logical_plan/op/op_match.h"
#include "base/parser/logical_plan/op/op_optional.h"
#include "base/parser/logical_plan/op/op_order_by.h"
#include "base/parser/logical_plan/op/op_select.h"
#include "base/parser/logical_plan/op/op_where.h"

void CheckWellDesigned::visit(OpOptional& op_optional) {
    auto local_vars = op_optional.op->get_vars();

    for (const auto var : local_vars) {
        if (global.find(var) != global.end() && parent.find(var) == parent.end()) {
            throw QuerySemanticException("Query is not well defined. Var " + var.name + " is breaking the rule");
        }
        global.insert(var);
    }

    parent = local_vars;
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
        parent = local_vars;
    }
}


void CheckWellDesigned::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    auto local_vars = op_basic_graph_pattern.get_vars();

    for (const auto var : local_vars) {
        if (global.find(var) != global.end() && parent.find(var) == parent.end()) {
            throw QuerySemanticException("Query is not well defined. Var " + var.name + " is breaking the rule");
        }
        global.insert(var);
    }
}


void CheckWellDesigned::visit(OpMatch& op_match) {
    op_match.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpDistinct& op_distinct) {
    op_distinct.op->accept_visitor(*this);
}
