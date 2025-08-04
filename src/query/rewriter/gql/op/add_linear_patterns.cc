#include "add_linear_patterns.h"

#include "query/parser/op/gql/ops.h"

using namespace GQL;

void AddLinearPatterns::visit(OpQueryStatements& op_statements)
{
    std::vector<std::unique_ptr<Op>> new_ops;
    for (auto& op : op_statements.ops) {
        op->accept_visitor(*this);
        new_ops.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpQueryStatements>(std::move(new_ops));
}

void AddLinearPatterns::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
    tmp = std::make_unique<OpReturn>(
        std::move(tmp),
        std::move(op_return.return_items),
        op_return.distinct,
        std::move(op_return.op_order_by)
    );
}

void AddLinearPatterns::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    tmp = std::make_unique<OpGroupBy>(std::move(tmp), std::move(op_group_by.exprs));
}

void AddLinearPatterns::visit(OpOrderByStatement& op_order_by)
{
    tmp = std::make_unique<OpOrderByStatement>(
        std::move(op_order_by.items),
        std::move(op_order_by.ascending_order),
        std::move(op_order_by.null_order),
        op_order_by.offset,
        op_order_by.limit
    );
}

void AddLinearPatterns::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
    tmp = std::make_unique<OpOrderBy>(
        std::move(tmp),
        std::move(op_order_by.items),
        std::move(op_order_by.ascending_order),
        std::move(op_order_by.null_order),
        op_order_by.offset,
        op_order_by.limit
    );
}

void AddLinearPatterns::visit(OpGraphPatternList& op_graph_pattern_list)
{
    std::vector<std::unique_ptr<Op>> new_patterns;
    for (auto& pattern : op_graph_pattern_list.patterns) {
        pattern->accept_visitor(*this);
        new_patterns.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpGraphPatternList>(std::move(new_patterns));
}

void AddLinearPatterns::visit(OpGraphPattern& op_graph_pattern)
{
    op_graph_pattern.op->accept_visitor(*this);
    tmp = std::make_unique<OpGraphPattern>(
        std::move(tmp),
        op_graph_pattern.mode,
        op_graph_pattern.path_var_id
    );
    linear_pattern = None;
}

void AddLinearPatterns::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);
    tmp = std::make_unique<OpFilter>(std::move(tmp), std::move(op_filter.exprs));
}

void AddLinearPatterns::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::vector<std::unique_ptr<Op>> linear_patterns;
    std::vector<std::unique_ptr<Op>> new_patterns;

    std::unique_ptr<VarId> previous_start;
    std::unique_ptr<VarId> previous_end;

    linear_pattern = None;

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);

        switch (linear_pattern) {
        case Node:
            if (consecutive_nodes_found) {
                new_patterns.push_back(std::make_unique<OpLinearPattern>(std::move(linear_patterns)));
            }
            linear_patterns.push_back(std::move(tmp));
            break;
        case Edge:
            linear_patterns.push_back(std::move(tmp));
            consecutive_nodes_found = false;
            break;
        case None:
            if (!linear_patterns.empty()) {
                new_patterns.push_back(std::make_unique<OpLinearPattern>(std::move(linear_patterns)));
            }
            new_patterns.push_back(std::move(tmp));
            consecutive_nodes_found = false;
        }
    }
    if (!linear_patterns.empty()) {
        new_patterns.push_back(std::make_unique<OpLinearPattern>(std::move(linear_patterns)));
    }

    tmp = std::make_unique<OpBasicGraphPattern>(std::move(new_patterns));
}

void AddLinearPatterns::visit(OpNode& op)
{
    tmp = std::make_unique<OpNode>(op.id);

    if (linear_pattern == Node) {
        consecutive_nodes_found = true;
    }
    linear_pattern = Node;
}

void AddLinearPatterns::visit(OpEdge& op)
{
    tmp = std::make_unique<OpEdge>(op);
    linear_pattern = Edge;
}

void AddLinearPatterns::visit(OpPathUnion& op_path_union)
{
    std::vector<std::unique_ptr<Op>> new_patterns;

    for (auto& pattern : op_path_union.op_list) {
        pattern->accept_visitor(*this);
        new_patterns.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpPathUnion>(std::move(new_patterns));
    linear_pattern = None;
}

void AddLinearPatterns::visit(OpRepetition& op_repetition)
{
    op_repetition.op->accept_visitor(*this);
    tmp = std::make_unique<OpRepetition>(std::move(tmp), op_repetition.lower, op_repetition.upper);
    linear_pattern = None;
}

void AddLinearPatterns::visit(OpLet& op_let)
{
    tmp = std::make_unique<OpLet>(std::move(op_let.items));
}

void AddLinearPatterns::visit(OpFilterStatement& op)
{
    tmp = std::make_unique<OpFilterStatement>(std::move(op.exprs));
}

void AddLinearPatterns::visit(OpLinearPattern&) { }
