#include "extract_optional_labels.h"

#include "query/parser/expr/gql/expr_property.h"
#include "query/parser/op/gql/graph_pattern/op_opt_labels.h"
#include "query/parser/op/gql/ops.h"
#include "query/rewriter/gql/expr/extract_optional_labels_from_expr.h"
#include "query/rewriter/gql/expr/extract_optional_properties_from_expr.h"

using namespace GQL;

void ExtractOptionalLabels::visit(OpQueryStatements& op_statements)
{
    for (auto& op : op_statements.ops) {
        op->accept_visitor(*this);
    }
}

void ExtractOptionalLabels::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);

    ExtractVarWithLabelsFromExpr expr_visitor;

    for (auto& item : op_return.return_items) {
        item.expr->accept_visitor(expr_visitor);
    }

    if (!expr_visitor.vars.empty()) {
        op_return.op = std::make_unique<OpOptLabels>(std::move(op_return.op), std::move(expr_visitor.vars));
    }
}

void ExtractOptionalLabels::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    ExtractVarWithLabelsFromExpr expr_visitor;

    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(expr_visitor);
    }

    if (!expr_visitor.vars.empty()) {
        op_filter.op = std::make_unique<OpOptLabels>(std::move(op_filter.op), std::move(expr_visitor.vars));
    }
}

void ExtractOptionalLabels::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);
}

void ExtractOptionalLabels::visit(OpGraphPatternList& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalLabels::visit(OpBasicGraphPattern& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalLabels::visit(OpLinearPattern& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalLabels::visit(OpRepetition& op)
{
    op.op->accept_visitor(*this);
}

void ExtractOptionalLabels::visit(OpOptProperties& op)
{
    op.op->accept_visitor(*this);
}

void ExtractOptionalLabels::visit(OpPathUnion& op)
{
    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalLabels::visit(OpOrderBy&) { }
void ExtractOptionalLabels::visit(OpLet&) { }
void ExtractOptionalLabels::visit(OpFilterStatement&) { }

void ExtractOptionalLabels::visit(OpNode&) { }
void ExtractOptionalLabels::visit(OpEdge&) { }
void ExtractOptionalLabels::visit(OpNodeLabel&) { }
void ExtractOptionalLabels::visit(OpEdgeLabel&) { }
void ExtractOptionalLabels::visit(OpProperty&) { }
