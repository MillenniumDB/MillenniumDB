#include "extract_optional_properties.h"

#include "query/parser/expr/gql/expr_property.h"
#include "query/parser/op/gql/ops.h"
#include "query/rewriter/gql/expr/extract_optional_properties_from_expr.h"

namespace GQL {

void ExtractOptionalProperties::visit(GQL::OpReturn& op_return)
{
    //for (auto& expr : op_return.expr_list) {
    //    auto expr_property = dynamic_cast<ExprProperty*>(expr.get());
    //    if (expr_property == nullptr) {
    //        continue;
    //    }
    ExtractOptionalPropertiesFromExpr expr_visitor;

    for (auto& item : op_return.return_items) {
        item.expr->accept_visitor(expr_visitor);
    }

    for(auto& property : expr_visitor.properties) {
        if (std::find(properties.begin(), properties.end(), property) != properties.end()){
            continue;
        }
        properties.push_back(property);
    }

    op_return.op->accept_visitor(*this);

    if (!properties.empty()) {
        auto op_property = std::make_unique<OpOptProperties>(
            std::move(op_return.op),
            properties
        );

        op_return.op = std::move(op_property);
        properties.clear();
    }
}

void ExtractOptionalProperties::visit(GQL::OpOrderBy& op_order_by)
{
    ExtractOptionalPropertiesFromExpr expr_visitor;

    for (auto& expr : op_order_by.items) {
        expr->accept_visitor(expr_visitor);
    }

    for(auto& property : expr_visitor.properties) {
        if (std::find(properties.begin(), properties.end(), property) != properties.end()){
            continue;
        }
        properties.push_back(property);
    }

    op_order_by.op->accept_visitor(*this);

    if (!properties.empty()) {
        auto op_property = std::make_unique<OpOptProperties>(
            std::move(op_order_by.op),
            properties
        );
        op_order_by.op = std::move(op_property);
        properties.clear();
    }
}

void ExtractOptionalProperties::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    ExtractOptionalPropertiesFromExpr expr_visitor;

    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(expr_visitor);
    }

    if (!expr_visitor.properties.empty()) {
        auto op_property = std::make_unique<OpOptProperties>(
            std::move(op_filter.op),
            expr_visitor.properties
        );
        op_filter.op = std::move(op_property);
    }
}

void ExtractOptionalProperties::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);
}

void ExtractOptionalProperties::visit(OpGraphPatternList& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalProperties::visit(OpBasicGraphPattern& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalProperties::visit(OpRepetition& op)
{
    op.op->accept_visitor(*this);
}

void ExtractOptionalProperties::visit(OpOptProperties& op)
{
    op.op->accept_visitor(*this);
}

void ExtractOptionalProperties::visit(OpPathUnion& op)
{
    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
    }
}

void ExtractOptionalProperties::visit(OpNode&) { }
void ExtractOptionalProperties::visit(OpEdge&) { }
void ExtractOptionalProperties::visit(OpNodeLabel&) { }
void ExtractOptionalProperties::visit(OpEdgeLabel&) { }
void ExtractOptionalProperties::visit(OpProperty&) { }

} // namespace GQL
