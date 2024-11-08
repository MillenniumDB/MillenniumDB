#include "rewrite_rule_visitor.h"

#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;


void RewriteRuleVisitor::visit(OpOptional& op_optional) {
    for (auto &rule : rules) {
        if (rule->is_possible_to_regroup(op_optional.lhs)) {
            op_optional.lhs = rule->regroup(std::move(op_optional.lhs));
            has_rewritten   = true;
        }
        if (rule->is_possible_to_regroup(op_optional.rhs)) {
            op_optional.rhs = rule->regroup(std::move(op_optional.rhs));
            has_rewritten   = true;
        }
    }
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpBasicGraphPattern&) { }

void RewriteRuleVisitor::visit(OpEmpty&) {
}


void RewriteRuleVisitor::visit(OpFilter& op_filter) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_filter.op)) {
            op_filter.op  = rule->regroup(std::move(op_filter.op));
            has_rewritten = true;
        }
    }
    op_filter.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpSelect& op_select) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_select.op)) {
            op_select.op  = rule->regroup(std::move(op_select.op));
            has_rewritten = true;
        }
    }
    op_select.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}


void RewriteRuleVisitor::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpOrderBy& op_order_by) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_order_by.op)) {
            op_order_by.op = rule->regroup(std::move(op_order_by.op));
            has_rewritten  = true;
        }
    }
    op_order_by.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpGroupBy& op_group_by) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_group_by.op)) {
            op_group_by.op = rule->regroup(std::move(op_group_by.op));
            has_rewritten  = true;
        }
    }
    op_group_by.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpService& /*op_service*/) { }


void RewriteRuleVisitor::visit(OpJoin& op_join) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_join.lhs)) {
            op_join.lhs   = rule->regroup(std::move(op_join.lhs));
            has_rewritten = true;
        }
        if (rule->is_possible_to_regroup(op_join.rhs)) {
            op_join.rhs   = rule->regroup(std::move(op_join.rhs));
            has_rewritten = true;
        }
    }
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpSemiJoin& op_semi_join) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_semi_join.lhs)) {
            op_semi_join.lhs   = rule->regroup(std::move(op_semi_join.lhs));
            has_rewritten = true;
        }
        if (rule->is_possible_to_regroup(op_semi_join.rhs)) {
            op_semi_join.rhs   = rule->regroup(std::move(op_semi_join.rhs));
            has_rewritten = true;
        }
    }
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpMinus& op_minus) {
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_minus.lhs)) {
            op_minus.lhs   = rule->regroup(std::move(op_minus.lhs));
            has_rewritten = true;
        }
        if (rule->is_possible_to_regroup(op_minus.rhs)) {
            op_minus.rhs   = rule->regroup(std::move(op_minus.rhs));
            has_rewritten = true;
        }
    }
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpSequence&) {
    assert(false && "Rewrites should happen before OpSequence is created");
}

void RewriteRuleVisitor::visit(OpUnion& op_union) {
    for (auto& rule : rules) {
        for (auto& child : op_union.unions) {
            if (rule->is_possible_to_regroup(child)) {
                child         = rule->regroup(std::move(child));
                has_rewritten = true;
            }
        }
    }
    for (auto& child : op_union.unions) {
        child->accept_visitor(*this);
    }
}


void RewriteRuleVisitor::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
}


void RewriteRuleVisitor::visit(OpUnitTable&) { }


void RewriteRuleVisitor::visit(OpValues&) { }
