#include "push_where_properties.h"

#include "query/parser/expr/mql/exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void PushWhereProperties::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
}

void PushWhereProperties::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
}

void PushWhereProperties::visit(OpUpdate& op_update)
{
    op_update.op->accept_visitor(*this);
}

void PushWhereProperties::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
}

void PushWhereProperties::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    auto bgp_vars = op_basic_graph_pattern.get_all_vars();
    auto it = props_to_push.begin();

    while (it != props_to_push.end()) {
        assert(it->obj.is_var());
        if (bgp_vars.find(it->obj.get_var()) != bgp_vars.end()) {
            op_basic_graph_pattern.add_property(it->obj, it->key, it->value);
            it = props_to_push.erase(it);
        } else {
            ++it;
        }
    }
}

void PushWhereProperties::visit(OpOptional& op_optional)
{
    op_optional.op->accept_visitor(*this);
    for (const auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}

void PushWhereProperties::visit(OpSequence& op_sequence)
{
    for (const auto& op : op_sequence.sequence) {
        op->accept_visitor(*this);
    }
}

void PushWhereProperties::visit(OpWhere& op_where)
{
    PushWherePropertiesExpr expr_visitor(op_where.expr, props_to_push);
    op_where.expr->accept_visitor(expr_visitor);
    op_where.op->accept_visitor(*this);
}

void PushWhereProperties::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);
}

void PushWherePropertiesExpr::visit(ExprAnd& expr_and)
{
    auto old_parent = current_parent;

    auto it = expr_and.and_list.begin();

    while (it != expr_and.and_list.end()) {
        auto& e = *it;
        current_parent = &e;
        e->accept_visitor(*this);
        if (auto casted_const = dynamic_cast<ExprConstant*>(e.get())) {
            if (casted_const->value.is_true()) {
                it = expr_and.and_list.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    current_parent = old_parent;

    if (expr_and.and_list.empty()) {
        *current_parent = std::make_unique<ExprConstant>(ObjectId(ObjectId::BOOL_TRUE));
    }
}

void PushWherePropertiesExpr::visit(ExprEquals& expr_equals)
{
    if (auto casted_prop = dynamic_cast<ExprVarProperty*>(expr_equals.lhs.get())) {
        if (auto casted_const = dynamic_cast<ExprConstant*>(expr_equals.rhs.get())) {
            props_to_push
                .emplace_back(casted_prop->var_without_property, casted_prop->key, casted_const->value);
            *current_parent = std::make_unique<ExprConstant>(ObjectId(ObjectId::BOOL_TRUE));
        }
        // else if (auto casted_prop2 = dynamic_cast<ExprVarProperty*>(expr_equals.rhs.get())) {
        //     props_to_push
        //         .emplace_back(casted_prop->var_without_property, casted_prop->key, casted_prop->var_with_property);
        //     props_to_push
        //         .emplace_back(casted_prop2->var_without_property, casted_prop2->key, casted_prop2->var_with_property);
        //     *current_parent = std::make_unique<ExprConstant>(ObjectId(ObjectId::BOOL_TRUE));
        // }
    } else if (auto casted_prop = dynamic_cast<ExprVarProperty*>(expr_equals.rhs.get())) {
        if (auto casted_const = dynamic_cast<ExprConstant*>(expr_equals.lhs.get())) {
            props_to_push
                .emplace_back(casted_prop->var_without_property, casted_prop->key, casted_const->value);
            *current_parent = std::make_unique<ExprConstant>(ObjectId(ObjectId::BOOL_TRUE));
        }
        // else if (auto casted_prop2 = dynamic_cast<ExprVarProperty*>(expr_equals.lhs.get())) {
        //     props_to_push
        //         .emplace_back(casted_prop->var_without_property, casted_prop->key, casted_prop->var_with_property);
        //     props_to_push
        //         .emplace_back(casted_prop2->var_without_property, casted_prop2->key, casted_prop2->var_with_property);
        //     *current_parent = std::make_unique<ExprConstant>(ObjectId(ObjectId::BOOL_TRUE));
        // }
    }
}
