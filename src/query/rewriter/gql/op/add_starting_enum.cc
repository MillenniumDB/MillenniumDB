#include "add_starting_enum.h"
#include "query/executor/binding_iter/gql/linear_pattern_path.h"
#include "query/parser/op/gql/ops.h"

using namespace GQL;

void AddStartingEnum::visit(OpReturn& op)
{
    op.op->accept_visitor(*this);
}

void AddStartingEnum::visit(OpGroupBy& op)
{
    op.op->accept_visitor(*this);
}

void AddStartingEnum::visit(OpQueryStatements& op)
{
    for (auto& statement : op.ops) {
        statement->accept_visitor(*this);
    }
}

void AddStartingEnum::visit(OpFilter& op)
{
    op.op->accept_visitor(*this);
}

void AddStartingEnum::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);
}

void AddStartingEnum::visit(OpGraphPatternList& op)
{
    for (auto& pattern : op.patterns) {
        repetition_found = false;
        pattern->accept_visitor(*this);
    }
}

void AddStartingEnum::visit(OpBasicGraphPattern& op)
{
    repetition_found = false;
    op.patterns[0]->accept_visitor(*this);
    if (!repetition_found) {
        return;
    }

    VarId node_id = get_query_ctx().get_internal_var();
    auto node = std::make_unique<OpNode>(node_id);
    std::vector<std::unique_ptr<Op>> vec;
    vec.push_back(std::move(node));
    auto linear_pattern = std::make_unique<OpLinearPattern>(
        std::move(vec),
        std::make_unique<VarId>(node_id),
        std::make_unique<VarId>(node_id)
    );

    op.patterns.insert(op.patterns.begin(), std::move(linear_pattern));
}

void AddStartingEnum::visit(OpRepetition&)
{
    repetition_found = true;
}

void AddStartingEnum::visit(OpLinearPattern& op)
{
    repetition_found = false;
    op.patterns[0]->accept_visitor(*this);
}

void AddStartingEnum::visit(OpOptProperties& op)
{
    op.op->accept_visitor(*this);
}

void AddStartingEnum::visit(OpPathUnion& op)
{
    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
    }
}

void AddStartingEnum::visit(OpOrderBy& op)
{
    op.op->accept_visitor(*this);
}

void AddStartingEnum::visit(OpFilterStatement&) { }

void AddStartingEnum::visit(OpOrderByStatement&) { }

void AddStartingEnum::visit(OpLet&) { }

void AddStartingEnum::visit(OpNode&) { }

void AddStartingEnum::visit(OpEdge&) { }

void AddStartingEnum::visit(OpNodeLabel&) { }

void AddStartingEnum::visit(OpEdgeLabel&) { }

void AddStartingEnum::visit(OpProperty&) { }
