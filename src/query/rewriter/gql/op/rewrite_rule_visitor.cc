#include "rewrite_rule_visitor.h"

#include "query/parser/op/gql/ops.h"

using namespace GQL;

void RewriteRuleVisitor::visit(OpQueryStatements& op_simple_linear_query_statements)
{
    for (auto& rule : rules) {
        for (auto& op : op_simple_linear_query_statements.ops) {
            if (rule->is_possible_to_regroup(op)) {
                op = rule->regroup(std::move(op));
            }
        }
    }

    for (auto& op : op_simple_linear_query_statements.ops) {
        op->accept_visitor(*this);
    }
}

void RewriteRuleVisitor::visit(OpReturn& op_return)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_return.op)) {
            op_return.op = rule->regroup(std::move(op_return.op));
            has_rewritten = true;
        }
    }

    op_return.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpGroupBy& op_group_by)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_group_by.op)) {
            op_group_by.op = rule->regroup(std::move(op_group_by.op));
            has_rewritten = true;
        }
    }
    op_group_by.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpOrderBy& op_order_by)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_order_by.op)) {
            op_order_by.op = rule->regroup(std::move(op_order_by.op));
            has_rewritten = true;
        }
    }
    op_order_by.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpGraphPatternList& op_graph_pattern)
{
    for (auto& rule : rules) {
        for (auto& pattern : op_graph_pattern.patterns) {
            if (rule->is_possible_to_regroup(pattern)) {
                pattern = rule->regroup(std::move(pattern));
                has_rewritten = true;
            }
        }
    }

    for (auto& pattern : op_graph_pattern.patterns) {
        pattern->accept_visitor(*this);
    }
}

void RewriteRuleVisitor::visit(OpLinearPattern& op_linear_pattern)
{
    for (auto& rule : rules) {
        for (auto& pattern : op_linear_pattern.patterns) {
            if (rule->is_possible_to_regroup(pattern)) {
                pattern = rule->regroup(std::move(pattern));
                has_rewritten = true;
            }
        }
    }

    for (auto& pattern : op_linear_pattern.patterns) {
        pattern->accept_visitor(*this);
    }
}

void RewriteRuleVisitor::visit(OpGraphPattern& op_graph_pattern)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_graph_pattern.op)) {
            op_graph_pattern.op = rule->regroup(std::move(op_graph_pattern.op));
            has_rewritten = true;
        }
    }

    op_graph_pattern.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    for (auto& rule : rules) {
        for (auto& pattern : op_basic_graph_pattern.patterns) {
            if (rule->is_possible_to_regroup(pattern)) {
                pattern = rule->regroup(std::move(pattern));
                has_rewritten = true;
            }
        }
    }

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);
    }
}

void RewriteRuleVisitor::visit(OpRepetition& op_repetition)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_repetition.op)) {
            op_repetition.op = rule->regroup(std::move(op_repetition.op));
            has_rewritten = true;
        }
    }
    op_repetition.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpFilter& op_filter)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_filter.op)) {
            op_filter.op = rule->regroup(std::move(op_filter.op));
            has_rewritten = true;
        }
    }
    op_filter.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpOptProperties& op_property)
{
    for (auto& rule : rules) {
        if (rule->is_possible_to_regroup(op_property.op)) {
            op_property.op = rule->regroup(std::move(op_property.op));
            has_rewritten = true;
        }
    }
    op_property.op->accept_visitor(*this);
}

void RewriteRuleVisitor::visit(OpPathUnion& op)
{
    for (auto& rule : rules) {
        for (auto& pattern : op.op_list) {
            if (rule->is_possible_to_regroup(pattern)) {
                pattern = rule->regroup(std::move(pattern));
                has_rewritten = true;
            }
        }
    }

    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
    }
}
