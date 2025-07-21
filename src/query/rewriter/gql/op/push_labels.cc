#include "push_labels.h"

using namespace GQL;

void PushLabels::visit(OpQueryStatements& op_statements)
{
    std::vector<std::unique_ptr<Op>> new_ops;

    for (auto& op : op_statements.ops) {
        op->accept_visitor(*this);
        new_ops.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpQueryStatements>(std::move(new_ops));
}

void PushLabels::visit(OpGraphPattern& op_graph_pattern)
{
    op_graph_pattern.op->accept_visitor(*this);
    tmp = std::make_unique<OpGraphPattern>(
        std::move(tmp),
        op_graph_pattern.mode,
        op_graph_pattern.path_var_id
    );
}

void PushLabels::visit(OpGraphPatternList& op_graph_pattern_list)
{
    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_graph_pattern_list.patterns) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpGraphPatternList>(std::move(patterns));
}

void PushLabels::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpBasicGraphPattern>(std::move(patterns));
}

void PushLabels::visit(OpLinearPattern& op_linear_pattern)
{
    vars_in_linear_pattern.clear();

    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_linear_pattern.patterns) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    for (auto& label : labels_to_push) {
        if (vars_in_linear_pattern.count(label.id)) {
            patterns.push_back(label.op->clone());
        }
    }

    tmp = std::make_unique<OpLinearPattern>(
        std::move(patterns),
        std::move(op_linear_pattern.start),
        std::move(op_linear_pattern.end)
    );
}

void PushLabels::visit(OpRepetition& op)
{
    op.op->accept_visitor(*this);
    tmp = std::make_unique<OpRepetition>(std::move(tmp), op.lower, op.upper);
}

void PushLabels::visit(OpReturn& op)
{
    op.op->accept_visitor(*this);
    tmp = std::make_unique<OpReturn>(
        std::move(tmp),
        std::move(op.return_items),
        op.distinct,
        std::move(op.op_order_by)
    );
}

void PushLabels::visit(OpGroupBy& op)
{
    op.op->accept_visitor(*this);
    tmp = std::make_unique<OpGroupBy>(std::move(tmp), std::move(op.exprs));
}

void PushLabels::visit(OpLet& op)
{
    tmp = std::make_unique<OpLet>(std::move(op.items));
}

void PushLabels::visit(OpOrderByStatement& op)
{
    tmp = std::make_unique<OpOrderByStatement>(
        std::move(op.items),
        std::move(op.ascending_order),
        std::move(op.null_order),
        op.offset,
        op.limit
    );
}

void PushLabels::visit(OpOrderBy& op)
{
    op.op->accept_visitor(*this);
    tmp = std::make_unique<OpOrderBy>(
        std::move(tmp),
        std::move(op.items),
        std::move(op.ascending_order),
        std::move(op.null_order),
        op.offset,
        op.limit
    );
}

void PushLabels::visit(OpFilter& op_filter)
{
    std::vector<std::unique_ptr<Expr>> exprs;

    visitor.clear_labels();
    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(visitor);

        if (visitor.tmp != nullptr) {
            exprs.push_back(std::move(visitor.tmp));
        }
    }

    size_t num_of_new_labels = visitor.labels.size();

    for (auto& label : visitor.labels) {
        labels_to_push.emplace_back(label.op->clone(), label.id, label.label_id);
    }

    op_filter.op->accept_visitor(*this);

    labels_to_push.erase(labels_to_push.end() - num_of_new_labels, labels_to_push.end());

    if (exprs.size() > 0) {
        tmp = std::make_unique<OpFilter>(std::move(tmp), std::move(exprs));
    }
}

void PushLabels::visit(OpOptProperties& op_property)
{
    op_property.op->accept_visitor(*this);
    tmp = std::make_unique<OpOptProperties>(std::move(tmp), op_property.properties);
}

void PushLabels::visit(OpPathUnion& op)
{
    std::vector<std::unique_ptr<Op>> patterns;
    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpPathUnion>(std::move(patterns));
}

void PushLabels::visit(OpNode& op)
{
    vars_in_linear_pattern.insert(op.id);
    tmp = op.clone();
}

void PushLabels::visit(OpEdge& op)
{
    vars_in_linear_pattern.insert(op.id);
    tmp = op.clone();
}

void PushLabels::visit(OpNodeLabel& op)
{
    tmp = op.clone();
}

void PushLabels::visit(OpEdgeLabel& op)
{
    tmp = op.clone();
}

void PushLabels::visit(OpProperty& op)
{
    tmp = op.clone();
}

void PushLabels::visit(OpFilterStatement& op_filter)
{
    tmp = op_filter.clone();
}
