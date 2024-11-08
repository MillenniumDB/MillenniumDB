#include "check_well_designed.h"

#include <cassert>
#include <set>

#include "query/exceptions.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void CheckWellDesigned::visit(OpOptional& op_optional) {
    // Q = [Q1 OPT Q2] it follows that all variables that appear
    // both outside of Q and in Q2 must also appear in Q1.
    auto q1_vars = op_optional.lhs->get_all_vars();
    auto q2_vars = op_optional.rhs->get_all_vars();

    for (auto var : q2_vars) {
        if (seen_vars.find(var) != seen_vars.end()) {
            // must also appear in Q1
            if (q1_vars.find(var) == q1_vars.end()) {
                is_well_designed = false;
                return;
            }
        }
    }

    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}

void CheckWellDesigned::visit(OpValues& op_values) {
    for (auto& var : op_values.vars) {
        seen_vars.insert(var);
    }
}


void CheckWellDesigned::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    for (auto& op_triple : op_basic_graph_pattern.triples) {
        for (auto& var : op_triple.get_all_vars()) {
            seen_vars.insert(var);
        }
    }

    for (auto& op_path : op_basic_graph_pattern.paths) {
        if (op_path.subject.is_var()) {
            seen_vars.insert(op_path.subject.get_var());
        }

        if (op_path.object.is_var()) {
            seen_vars.insert(op_path.object.get_var());
        }
    }
}


void CheckWellDesigned::visit(OpService& op_service) {
    for (auto& var : op_service.get_scope_vars()) {
        seen_vars.insert(var);
    }
}

void CheckWellDesigned::visit(OpBind& op_bind) {
    seen_vars.insert(op_bind.var);
    op_bind.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpSelect& op_select) {
    all_vars = op_select.get_all_vars();
    op_select.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        all_vars = op_describe.get_all_vars();
        op_describe.op->accept_visitor(*this);
    }
}


void CheckWellDesigned::visit(OpConstruct& op_construct) {
    all_vars = op_construct.get_all_vars();
    op_construct.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpAsk& op_ask) {
    all_vars = op_ask.get_all_vars();
    op_ask.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}


void CheckWellDesigned::visit(OpUnion& op_union) {
    for (auto &child : op_union.unions) {
        child->accept_visitor(*this);
    }
}


void CheckWellDesigned::visit(OpSequence& op_sequence) {
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}


void CheckWellDesigned::visit(OpUnitTable&) { }
void CheckWellDesigned::visit(OpEmpty&) { }
