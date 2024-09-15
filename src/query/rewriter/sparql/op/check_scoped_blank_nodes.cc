#include "check_scoped_blank_nodes.h"

#include "query/exceptions.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void CheckScopedBlankNodes::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    // It is important to notice that we treat blank nodes as
    // "new variables that can't be referred to"
    // and we identify them by their name (they start with "_:")
    std::set<VarId> scope_blank_nodes;
    for (auto& triple : op_basic_graph_pattern.triples) {
        if (triple.subject.is_var()) {
            if (get_query_ctx().get_var_name(triple.subject.get_var()).find("_:") == 0) {
                scope_blank_nodes.insert(triple.subject.get_var());
            }
        }
        if (triple.object.is_var()) {
            if (get_query_ctx().get_var_name(triple.object.get_var()).find("_:") == 0) {
                scope_blank_nodes.insert(triple.object.get_var());
            }
        }
    }
    for (auto& path : op_basic_graph_pattern.paths) {
        if (path.subject.is_var()) {
            if (get_query_ctx().get_var_name(path.subject.get_var()).find("_:") == 0) {
                scope_blank_nodes.insert(path.subject.get_var());
            }
        }
        if (path.object.is_var()) {
            if (get_query_ctx().get_var_name(path.object.get_var()).find("_:") == 0) {
                scope_blank_nodes.insert(path.object.get_var());
            }
        }
    }
    // Check if any blank node was mentioned before in another scope
    for (auto& bnode : scope_blank_nodes) {
        if (mentioned_blank_nodes.find(bnode) != mentioned_blank_nodes.end()) {
            throw QuerySemanticException("Query is not well defined. Blank node "
                                         + get_query_ctx().get_var_name(bnode)
                                         + " is breaking the rule");
        }
    }
    // Store current scope blank nodes into mentioned blank nodes
    mentioned_blank_nodes.insert(scope_blank_nodes.begin(), scope_blank_nodes.end());
}


void CheckScopedBlankNodes::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}


void CheckScopedBlankNodes::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpService& /*op_service*/) { }


void CheckScopedBlankNodes::visit(OpSequence& op_sequence) {
    for (auto &op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void CheckScopedBlankNodes::visit(OpUnion& op_union) {
    for (auto &child : op_union.unions) {
        child->accept_visitor(*this);
    }
}


void CheckScopedBlankNodes::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
}


void CheckScopedBlankNodes::visit(OpUnitTable&) { }


void CheckScopedBlankNodes::visit(OpValues&) { }
