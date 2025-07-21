#include "extract_exprs.h"

namespace GQL {

void ExtractExprs::visit(OpQueryStatements& op_statements)
{
    std::vector<std::unique_ptr<Op>> new_patterns;
    for (auto& op : op_statements.ops) {
        op->accept_visitor(*this);
        new_patterns.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpQueryStatements>(std::move(new_patterns));
}

void ExtractExprs::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
    tmp = std::make_unique<OpReturn>(
        std::move(tmp),
        std::move(op_return.return_items),
        op_return.distinct,
        std::move(op_return.op_order_by)
    );
}

void ExtractExprs::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    tmp = std::make_unique<OpGroupBy>(std::move(tmp), std::move(op_group_by.exprs));
}

void ExtractExprs::visit(OpOrderByStatement& op_order_by)
{
    tmp = std::make_unique<OpOrderByStatement>(
        std::move(op_order_by.items),
        std::move(op_order_by.ascending_order),
        std::move(op_order_by.null_order),
        op_order_by.offset,
        op_order_by.limit
    );
}

void ExtractExprs::visit(OpGraphPatternList& op_graph_pattern_list)
{
    std::vector<std::unique_ptr<Op>> new_patterns;
    for (auto& pattern : op_graph_pattern_list.patterns) {
        pattern->accept_visitor(*this);
        new_patterns.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpGraphPatternList>(std::move(new_patterns));
}

void ExtractExprs::visit(OpGraphPattern& op_graph_pattern)
{
    bool this_is_first_gp = !graph_pattern_visited;

    graph_pattern_visited = true;
    op_graph_pattern.op->accept_visitor(*this);
    graph_pattern_visited = !this_is_first_gp;

    tmp = std::make_unique<OpGraphPattern>(
        std::move(tmp),
        op_graph_pattern.mode,
        op_graph_pattern.path_var_id
    );

    if (this_is_first_gp && !tmp_exprs.empty()) {
        tmp = std::make_unique<OpFilter>(std::move(tmp), std::move(tmp_exprs));
    }
}

void ExtractExprs::visit(OpOrderBy& op_order_by)
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

void ExtractExprs::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    if (graph_pattern_visited) {
        // store the exprs
        tmp_exprs.insert(
            tmp_exprs.end(),
            std::make_move_iterator(op_filter.exprs.begin()),
            std::make_move_iterator(op_filter.exprs.end())
        );
    } else {
        tmp = std::make_unique<OpFilter>(std::move(tmp), std::move(op_filter.exprs));
    }
}

void ExtractExprs::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::vector<std::unique_ptr<Op>> new_patterns;

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);
        new_patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpBasicGraphPattern>(std::move(new_patterns));
}

void ExtractExprs::visit(OpPathUnion& op_path_union)
{
    std::vector<std::unique_ptr<Op>> new_patterns;

    for (auto& pattern : op_path_union.op_list) {
        pattern->accept_visitor(*this);
        new_patterns.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpPathUnion>(std::move(new_patterns));
}

void ExtractExprs::visit(OpRepetition& op_repetition)
{
    op_repetition.op->accept_visitor(*this);
    tmp = std::make_unique<OpRepetition>(std::move(tmp), op_repetition.lower, op_repetition.upper);
}

void ExtractExprs::visit(OpOptProperties& op_opt_properties)
{
    op_opt_properties.op->accept_visitor(*this);
    tmp = std::make_unique<OpOptProperties>(std::move(tmp), op_opt_properties.properties);
}

void ExtractExprs::visit(OpLinearPattern& op)
{
    tmp = std::make_unique<OpLinearPattern>(std::move(op.patterns), std::move(op.start), std::move(op.end));
}

void ExtractExprs::visit(OpNode& op)
{
    tmp = std::make_unique<OpNode>(op.id);
}

void ExtractExprs::visit(OpEdge& op)
{
    tmp = std::make_unique<OpEdge>(op);
}

void ExtractExprs::visit(OpLet& op_let)
{
    tmp = std::make_unique<OpLet>(std::move(op_let.items));
}

void ExtractExprs::visit(OpProperty& op)
{
    tmp = std::make_unique<OpProperty>(op);
}

void ExtractExprs::visit(OpNodeLabel& op)
{
    tmp = std::make_unique<OpNodeLabel>(op);
}

void ExtractExprs::visit(OpEdgeLabel& op)
{
    tmp = std::make_unique<OpEdgeLabel>(op);
}

void ExtractExprs::visit(OpFilterStatement& op)
{
    tmp = std::make_unique<OpFilterStatement>(std::move(op.exprs));
}

} // namespace GQL
