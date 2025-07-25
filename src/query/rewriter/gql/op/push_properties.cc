#include "push_properties.h"

#include "query/parser/op/gql/ops.h"

using namespace GQL;

void PushProperties::visit(OpQueryStatements& op_simple_linear_query_statements)
{
    std::vector<std::unique_ptr<Op>> new_ops;

    for (auto& op : op_simple_linear_query_statements.ops) {
        op->accept_visitor(*this);
        new_ops.push_back(std::move(tmp));
    }
    tmp = std::make_unique<OpQueryStatements>(std::move(new_ops));
}

void PushProperties::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
    tmp = std::make_unique<OpReturn>(
        std::move(tmp),
        std::move(op_return.return_items),
        op_return.distinct,
        std::move(op_return.op_order_by)
    );
}

void PushProperties::visit(OpOrderBy& op_order_by)
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

void PushProperties::visit(OpOrderByStatement& op_order_by)
{
    tmp = std::make_unique<OpOrderByStatement>(
        std::move(op_order_by.items),
        std::move(op_order_by.ascending_order),
        std::move(op_order_by.null_order),
        op_order_by.offset,
        op_order_by.limit
    );
}

void PushProperties::visit(OpGraphPatternList& op_graph_pattern_list)
{
    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_graph_pattern_list.patterns) {
        pattern->accept_visitor(*this);

        patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpGraphPatternList>(std::move(patterns));
}

void PushProperties::visit(OpGraphPattern& op_graph_pattern)
{
    op_graph_pattern.op->accept_visitor(*this);
    tmp = std::make_unique<OpGraphPattern>(
        std::move(tmp),
        op_graph_pattern.mode,
        op_graph_pattern.path_var_id
    );
}

void PushProperties::visit(OpRepetition& op_repetition)
{
    op_repetition.op->accept_visitor(*this);
    tmp = std::make_unique<OpRepetition>(std::move(tmp), op_repetition.lower, op_repetition.upper);
}

void PushProperties::visit(OpFilter& op_filter)
{
    std::vector<std::unique_ptr<Expr>> exprs;

    visitor.clear_properties();
    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(visitor);

        if (visitor.tmp != nullptr) {
            exprs.push_back(std::move(visitor.tmp));
        }
    }

    size_t num_of_new_properties = visitor.properties.size();
    properties.insert(properties.end(), visitor.properties.begin(), visitor.properties.end());

    op_filter.op->accept_visitor(*this);

    properties.erase(properties.end() - num_of_new_properties, properties.end());

    if (exprs.size() > 0) {
        tmp = std::make_unique<OpFilter>(std::move(tmp), std::move(exprs));
    }
}

void PushProperties::visit(OpOptProperties& op_property)
{
    op_property.op->accept_visitor(*this);
    tmp = std::make_unique<OpOptProperties>(std::move(tmp), op_property.properties);
}

void PushProperties::visit(OpPathUnion& op_path_union)
{
    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_path_union.op_list) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpPathUnion>(std::move(patterns));
}

void PushProperties::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    tmp = std::make_unique<OpBasicGraphPattern>(std::move(patterns));
}

void PushProperties::visit(OpLinearPattern& op_linear_pattern)
{
    vars_in_linear_pattern.clear();
    std::vector<std::unique_ptr<Op>> patterns;

    for (auto& pattern : op_linear_pattern.patterns) {
        pattern->accept_visitor(*this);
        patterns.push_back(std::move(tmp));
    }

    for (auto& property : properties) {
        if (vars_in_linear_pattern.count(property.object)) {
            patterns.push_back(std::make_unique<OpProperty>(property));
        }
    }

    tmp = std::make_unique<OpLinearPattern>(
        std::move(patterns),
        std::move(op_linear_pattern.start),
        std::move(op_linear_pattern.end)
    );
}

void PushProperties::visit(OpLet& op)
{
    tmp = std::make_unique<OpLet>(std::move(op.items));
}

void PushProperties::visit(OpNode& op)
{
    vars_in_linear_pattern.insert(op.id);
    tmp = op.clone();
}

void PushProperties::visit(OpEdge& op)
{
    vars_in_linear_pattern.insert(op.id);
    tmp = op.clone();
}

void PushProperties::visit(OpNodeLabel& op)
{
    tmp = op.clone();
}

void PushProperties::visit(OpEdgeLabel& op)
{
    tmp = op.clone();
}

void PushProperties::visit(OpFilterStatement& op)
{
    tmp = op.clone();
}
