#include "obtain_var_count.h"

#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void ObtainVarCount::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}

void ObtainVarCount::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    auto vars = op_basic_graph_pattern.get_all_vars();
    for (auto& var : vars) {
        var_count[var] = var_count.find(var) == var_count.end() ? 1
                       : var_count[var] + 1;
    }
}

void ObtainVarCount::visit(OpEmpty&) {}

void ObtainVarCount::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);
}

void ObtainVarCount::visit(OpSelect& op_select) {
    // op_select creates a new scope, so the vars of this scope are only
    // the vars that are selected by op_select.
    for (auto& var : op_select.vars) {
        var_count[var] = var_count.find(var) == var_count.end() ? 1
                       : var_count[var] + 1;
    }
    // The children of the op_select are not visited, because it is another
    // scope.
}


void ObtainVarCount::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}


void ObtainVarCount::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void ObtainVarCount::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}

void ObtainVarCount::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
}

void ObtainVarCount::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}

void ObtainVarCount::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}


void ObtainVarCount::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}

void ObtainVarCount::visit(OpService& op_service) {
    auto vars = op_service.get_all_vars();
    for (auto& var : vars) {
        var_count[var] = var_count.find(var) == var_count.end() ? 1
                       : var_count[var] + 1;
    }
}

void ObtainVarCount::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void ObtainVarCount::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}


void ObtainVarCount::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}


void ObtainVarCount::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}

void ObtainVarCount::visit(OpUnion& op_union) {
    for (auto &child : op_union.unions) {
        child->accept_visitor(*this);
    }
}


void ObtainVarCount::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
}


void ObtainVarCount::visit(OpUnitTable&) { }


void ObtainVarCount::visit(OpValues&) {
    // TODO: should variables that have no UNDEF value be added to save vars?
}
