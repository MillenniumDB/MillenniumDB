#include "binding_list_iter_constructor.h"

#include "graph_models/gql/comparisons.h"
#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/aggregation/aggregation.h"
#include "query/executor/binding_iter/distinct_hash.h"
#include "query/executor/binding_iter/expr_evaluator.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/gql/assign_properties.h"
#include "query/executor/binding_iter/gql/check_repeated_variable.h"
#include "query/executor/binding_iter/gql/extend_right.h"
#include "query/executor/binding_iter/gql/linear_pattern_path.h"
#include "query/executor/binding_iter/gql/path_to_binding.h"
#include "query/executor/binding_iter/gql/repetition.h"
#include "query/executor/binding_iter/gql/union_path.h"
#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/object_enum.h"
#include "query/executor/binding_iter/order_by.h"
#include "query/executor/binding_iter/set_variable_value.h"
#include "query/executor/binding_iter/single_result_binding_iter.h"
#include "query/executor/binding_iter/slice.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/selinger_optimizer.h"
#include "query/optimizer/property_graph_model/expr_to_binding_expr.h"
#include "query/optimizer/property_graph_model/plan/any_direction_edge_plan.h"
#include "query/optimizer/property_graph_model/plan/directed_edge_plan.h"
#include "query/optimizer/property_graph_model/plan/directed_left_right_edge_plan.h"
#include "query/optimizer/property_graph_model/plan/edge_label_plan.h"
#include "query/optimizer/property_graph_model/plan/edge_property_plan.h"
#include "query/optimizer/property_graph_model/plan/node_label_plan.h"
#include "query/optimizer/property_graph_model/plan/node_plan.h"
#include "query/optimizer/property_graph_model/plan/node_property_plan.h"
#include "query/optimizer/property_graph_model/plan/undirected_directed_edge_plan.h"
#include "query/optimizer/property_graph_model/plan/undirected_edge_plan.h"
#include "query/parser/expr/gql/expr_property.h"
#include "query/parser/op/gql/ops.h"

using namespace GQL;

std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>
    get_non_redundant_exprs(std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>& exprs)
{
    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> res;

    for (auto&& [var, e] : exprs) {
        auto casted_expr_var = dynamic_cast<BindingExprVar*>(e.get());
        if (casted_expr_var) {
            if (casted_expr_var->var == var) {
                // avoid redundant assignation
                continue;
            }
        }

        res.emplace_back(var, std::move(e));
    }
    exprs.clear();
    return res;
}

void PathBindingIterConstructor::visit(OpReturn& op_return)
{
    for (auto& item : op_return.return_items) {
        if (item.expr != nullptr && item.expr->has_aggregation()) {
            grouping = true;
            break;
        }
    }

    for (auto& var : op_return.get_expr_vars()) {
        return_op_vars.push_back(var);
    }
    op_return.op->accept_visitor(*this);

    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> projection_exprs;

    for (auto& item : op_return.return_items) {
        if (item.alias.has_value()) {
            ExprToBindingExpr expr_to_binding_expr(this, item.alias.value(), true);
            item.expr->accept_visitor(expr_to_binding_expr);

            projection_exprs.emplace_back(item.alias.value(), std::move(expr_to_binding_expr.tmp));
        }
    }

    tmp_iter = get_pending_properties(std::move(tmp_iter));

    auto non_redundant_exprs = get_non_redundant_exprs(projection_exprs);
    if (non_redundant_exprs.size() > 0) {
        tmp_iter = std::make_unique<ExprEvaluator>(std::move(tmp_iter), std::move(non_redundant_exprs));
    }

    if (has_group_by) {
        std::vector<bool> ascending(group_vars.size(), true);
        std::vector<VarId> group_vars_vector(group_vars.begin(), group_vars.end());

        tmp_iter = std::make_unique<OrderBy>(
            std::move(tmp_iter),
            std::move(group_saved_vars),
            std::move(group_vars_vector),
            std::move(ascending),
            &OrderBy::internal_compare
        );
    }

    if (aggregations.size() > 0 || group_vars.size() > 0) {
        tmp_iter = std::make_unique<Aggregation>(
            std::move(tmp_iter),
            std::move(aggregations),
            std::move(group_vars)
        );
    }

    if (op_return.distinct) {
        auto projected_vars_set = op_return.get_expr_vars();
        std::vector<VarId> projected_vars;
        std::copy(projected_vars_set.begin(), projected_vars_set.end(), std::back_inserter(projected_vars));
        tmp_iter = std::make_unique<DistinctHash>(std::move(tmp_iter), std::move(projected_vars));
    }

    if (op_return.op_order_by != nullptr) {
        op_return.op_order_by->accept_visitor(*this);
    }
}

void PathBindingIterConstructor::visit(OpQueryStatements& op_statements)
{
    for (auto& op : op_statements.ops) {
        op->accept_visitor(*this);
    }
}

void PathBindingIterConstructor::visit(OpFilterStatement& op_filter)
{
    std::vector<std::unique_ptr<BindingExpr>> binding_exprs;

    ExprToBindingExpr expr_to_binding_expr(this, {}, false);
    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(expr_to_binding_expr);
        binding_exprs.push_back(std::move(expr_to_binding_expr.tmp));
    }

    tmp_iter = get_pending_properties(std::move(tmp_iter));

    tmp_iter = std::make_unique<Filter>(
        &Conversions::to_boolean,
        std::move(tmp_iter),
        std::move(binding_exprs)
    );
}

void PathBindingIterConstructor::visit(OpOrderByStatement& op_order_by)
{
    for (const auto& item : op_order_by.items) {
        if (item->has_aggregation()) {
            grouping = true;
            break;
        }
    }

    if (!op_order_by.items.empty()) {
        handle_order_by(op_order_by.items, op_order_by.ascending_order, op_order_by.null_order);
        if (op_order_by.offset != Op::DEFAULT_OFFSET || op_order_by.limit != Op::DEFAULT_LIMIT) {
            tmp_iter = std::make_unique<Slice>(std::move(tmp_iter), op_order_by.offset, op_order_by.limit);
        }
    } else {
        tmp_iter = std::make_unique<Slice>(std::move(tmp_iter), op_order_by.offset, op_order_by.limit);
    }
}

void PathBindingIterConstructor::visit(OpOrderBy& op_order_by)
{
    for (const auto& item : op_order_by.items) {
        if (item->has_aggregation()) {
            grouping = true;
            break;
        }
    }
    op_order_by.op->accept_visitor(*this);

    if (!op_order_by.items.empty()) {
        handle_order_by(op_order_by.items, op_order_by.ascending_order, op_order_by.null_order);
        if (op_order_by.offset != Op::DEFAULT_OFFSET || op_order_by.limit != Op::DEFAULT_LIMIT) {
            tmp_iter = std::make_unique<Slice>(std::move(tmp_iter), op_order_by.offset, op_order_by.limit);
        }
    } else {
        tmp_iter = std::make_unique<Slice>(std::move(tmp_iter), op_order_by.offset, op_order_by.limit);
    }
}

void PathBindingIterConstructor::handle_order_by(
    std::vector<std::unique_ptr<Expr>>& items,
    std::vector<bool>& ascending_order,
    std::vector<bool>& null_order
)
{
    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> exprs_to_eval;

    std::vector<VarId> order_vars;

    std::set<VarId> order_saved_vars;

    for (auto& item : items) {
        auto casted_expr_var = dynamic_cast<ExprVar*>(item.get());
        auto casted_expr_property = dynamic_cast<ExprProperty*>(item.get());
        if (casted_expr_var) {
            order_vars.push_back(casted_expr_var->id);
            order_saved_vars.insert(casted_expr_var->id);
        } else if (casted_expr_property) {
            order_vars.push_back(casted_expr_property->value);
            order_saved_vars.insert(casted_expr_property->value);

            ExprToBindingExpr expr_to_binding_expr(this, {}, false);
            casted_expr_property->accept_visitor(expr_to_binding_expr);
            exprs_to_eval.emplace_back(casted_expr_property->value, std::move(expr_to_binding_expr.tmp));
        } else {
            auto expr = item.get();
            auto var = get_query_ctx().get_internal_var();

            order_vars.push_back(var);
            order_saved_vars.insert(var);

            ExprToBindingExpr expr_to_binding_expr(this, {}, false);
            expr->accept_visitor(expr_to_binding_expr);
            exprs_to_eval.emplace_back(var, std::move(expr_to_binding_expr.tmp));
        }
    }

    tmp_iter = get_pending_properties(std::move(tmp_iter));

    if (!exprs_to_eval.empty()) {
        tmp_iter = std::make_unique<ExprEvaluator>(std::move(tmp_iter), std::move(exprs_to_eval));
    }

    for (auto& var : return_op_vars) {
        order_saved_vars.insert(var);
    }
    if (null_order[0]) {
        tmp_iter = std::make_unique<OrderBy>(
            std::move(tmp_iter),
            std::move(order_saved_vars),
            std::move(order_vars),
            std::move(ascending_order),
            &GQL::Comparisons::compare_null_first
        );
    } else {
        tmp_iter = std::make_unique<OrderBy>(
            std::move(tmp_iter),
            std::move(order_saved_vars),
            std::move(order_vars),
            std::move(ascending_order),
            &GQL::Comparisons::compare_null_last
        );
    }
}

void PathBindingIterConstructor::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    grouping = true;
    has_group_by = true;

    for (auto& e : op_group_by.exprs) {
        Expr* expr = e.get();
        if (auto casted = dynamic_cast<ExprVar*>(expr); casted != nullptr) {
            auto var = casted->id;
            group_vars.insert(var);
        } else {
            // TODO: impossible case for now
        }
    }
}

void PathBindingIterConstructor::visit(OpOptProperties& op_opt_properties)
{
    op_opt_properties.op->accept_visitor(*this);
}

void PathBindingIterConstructor::visit(OpLet& op_let)
{
    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> binding_exprs;

    ExprToBindingExpr expr_to_binding_expr(this, {}, false);
    for (auto& let_item : op_let.items) {
        let_item.expr->accept_visitor(expr_to_binding_expr);
        binding_exprs.emplace_back(let_item.var_id, std::move(expr_to_binding_expr.tmp));
    }

    auto cur_iter = get_pending_properties(std::make_unique<SingleResultBindingIter>());
    cur_iter = std::make_unique<SetVariableValues>(std::move(cur_iter), std::move(binding_exprs));

    if (tmp_iter != nullptr) {
        tmp_iter = std::make_unique<IndexNestedLoopJoin>(std::move(tmp_iter), std::move(cur_iter));
    } else {
        tmp_iter = std::move(cur_iter);
    }
}

void PathBindingIterConstructor::visit(OpGraphPatternList& op)
{
    std::unique_ptr<BindingIter> current_tmp;
    auto previous_iter = std::move(tmp_iter);

    for (auto& pattern : op.patterns) {
        is_first_iter = true;
        graph_pattern_vars.clear();

        pattern->accept_visitor(*this);

        if (current_tmp == nullptr) {
            current_tmp = std::move(tmp_iter);
        } else {
            current_tmp = std::make_unique<IndexNestedLoopJoin>(std::move(current_tmp), std::move(tmp_iter));
        }
    }

    if (previous_iter != nullptr) {
        tmp_iter = std::make_unique<IndexNestedLoopJoin>(std::move(previous_iter), std::move(current_tmp));
    } else {
        tmp_iter = std::move(current_tmp);
    }
}

void PathBindingIterConstructor::visit(OpGraphPattern& op_graph_pattern)
{
    // if there are nested graph patterns, we ignore the inner patterns
    if (is_first_gp) {
        is_first_gp = false;
        op_graph_pattern.op->accept_visitor(*this);

        if (op_graph_pattern.path_var_id.has_value()) {
            tmp_iter = std::make_unique<PathToBinding>(
                std::move(tmp_path),
                op_graph_pattern.get_var_types(),
                *op_graph_pattern.path_var_id
            );
        } else {
            tmp_iter = std::make_unique<PathToBinding>(std::move(tmp_path), op_graph_pattern.get_var_types());
        }
        is_first_gp = true;
    } else {
        op_graph_pattern.op->accept_visitor(*this);
    }
}

void PathBindingIterConstructor::visit(OpPathUnion& op_union)
{
    std::vector<std::unique_ptr<PathBindingIter>> iters;

    std::set<VarId> previous_assigned_vars = assigned_vars;
    bool previous_is_first_iter = is_first_iter; // consider handling this in the logical plan

    for (auto& pattern : op_union.op_list) {
        is_first_iter = previous_is_first_iter;
        pattern->accept_visitor(*this);
        iters.push_back(std::move(tmp_path));

        assigned_vars = previous_assigned_vars;
    }

    tmp_path = std::make_unique<UnionPath>(std::move(iters));
}

void PathBindingIterConstructor::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    std::vector<std::unique_ptr<BindingExpr>> binding_exprs;

    ExprToBindingExpr expr_to_binding_expr(this, {}, false);
    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(expr_to_binding_expr);
        binding_exprs.push_back(std::move(expr_to_binding_expr.tmp));
    }

    tmp_iter = get_pending_properties(std::move(tmp_iter));

    tmp_iter = std::make_unique<Filter>(
        &GQL::Conversions::to_boolean,
        std::move(tmp_iter),
        std::move(binding_exprs)
    );
}

void PathBindingIterConstructor::visit(OpOptLabels&) { }

void PathBindingIterConstructor::visit(OpRepetition& op_repetition)
{
    if (is_first_iter) {
        is_first_iter = false;
        op_repetition.op->accept_visitor(*this);

        auto node_var = get_query_ctx().get_internal_var();
        NodePlan node_plan(node_var);
        auto node_iter = node_plan.get_binding_iter();
        auto node_list_iter = std::make_unique<LinearPatternPath>(
            std::move(node_iter),
            std::vector<VarId> { node_var }
        );
        tmp_path = std::make_unique<Repetition>(
            std::move(tmp_path),
            op_repetition.lower,
            op_repetition.upper
        );
        tmp_path = std::make_unique<ExtendRight>(std::move(node_list_iter), std::move(tmp_path));
    } else {
        op_repetition.op->accept_visitor(*this);
        tmp_path = std::make_unique<Repetition>(
            std::move(tmp_path),
            op_repetition.lower,
            op_repetition.upper
        );
    }
}

void PathBindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    bool first = true;

    std::unique_ptr<PathBindingIter> local_tmp;

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);

        if (first) {
            local_tmp = std::move(tmp_path);
            first = false;
        } else {
            local_tmp = std::make_unique<ExtendRight>(std::move(local_tmp), std::move(tmp_path));
        }
    }

    tmp_path = std::move(local_tmp);
}

void PathBindingIterConstructor::visit(OpLinearPattern& op_linear_pattern)
{
    possible_disjoint_nodes.clear();
    linear_pattern_vars.clear();
    seen_nodes.clear();
    base_plans.clear();

    for (auto& pattern : op_linear_pattern.patterns) {
        pattern->accept_visitor(*this);
    }

    for (auto& node_id : possible_disjoint_nodes) {
        if (!seen_nodes.count(node_id)) {
            base_plans.push_back(std::make_unique<NodePlan>(node_id));
        }
    }

    for (auto& plan : base_plans) {
        plan->set_input_vars(assigned_vars);
    }

    if (!is_first_iter) {
        for (auto& plan : base_plans) {
            plan->set_input_vars({ linear_pattern_vars[0] });
        }
    }

    // insert new assigned vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            assigned_vars.insert(var);
        }
    }

    std::unique_ptr<Plan> root_plan = GreedyOptimizer::get_plan(base_plans);
    tmp_iter = root_plan->get_binding_iter();

    auto first_var = linear_pattern_vars.front();

    tmp_path = std::make_unique<LinearPatternPath>(std::move(tmp_iter), std::move(linear_pattern_vars));

    if (graph_pattern_vars.count(first_var)) {
        tmp_path = std::make_unique<CheckRepeatedVariable>(std::move(tmp_path), first_var);
    }

    graph_pattern_vars.insert(assigned_vars.begin(), assigned_vars.end());
    is_first_iter = false;
}

void PathBindingIterConstructor::visit(OpNode& op_node)
{
    possible_disjoint_nodes.insert(op_node.id);
    linear_pattern_vars.push_back(op_node.id);
}

void PathBindingIterConstructor::visit(OpEdge& op_edge)
{
    seen_nodes.insert(op_edge.from);
    seen_nodes.insert(op_edge.to);

    linear_pattern_vars.push_back(op_edge.id);
    linear_pattern_vars.push_back(op_edge.direction_var);

    switch (op_edge.type) {
    case OpEdge::Directed:
        base_plans.push_back(std::make_unique<DirectedEdgePlan>(
            op_edge.id,
            op_edge.from,
            op_edge.to,
            op_edge.direction_var,
            op_edge.direction.value()
        ));
        break;
    case OpEdge::Undirected:
        base_plans.push_back(
            std::make_unique<UndirectedEdgePlan>(op_edge.id, op_edge.from, op_edge.to, op_edge.direction_var)
        );
        break;
    case OpEdge::UndirectedOrDirectedTo:
        base_plans.push_back(std::make_unique<UndirectedDirectedEdgePlan>(
            op_edge.id,
            op_edge.from,
            op_edge.to,
            op_edge.direction_var,
            op_edge.direction.value()
        ));
        break;
    case OpEdge::DirectedLeftOrRight:
        base_plans.push_back(std::make_unique<DirectedLeftRightEdgePlan>(
            op_edge.id,
            op_edge.from,
            op_edge.to,
            op_edge.direction_var
        ));
        break;
    case OpEdge::AnyDirection:
        base_plans.push_back(std::make_unique<AnyDirectionEdgePlan>(
            op_edge.id,
            op_edge.from,
            op_edge.to,
            op_edge.direction_var
        ));
        break;
    }
}

void PathBindingIterConstructor::visit(OpNodeLabel& op_node_label)
{
    seen_nodes.insert(op_node_label.node_id);
    base_plans.push_back(std::make_unique<NodeLabelPlan>(op_node_label.node_id, op_node_label.label_id));
}

void PathBindingIterConstructor::visit(OpEdgeLabel& op_edge_label)
{
    base_plans.push_back(std::make_unique<EdgeLabelPlan>(op_edge_label.edge_id, op_edge_label.label_id));
}

void PathBindingIterConstructor::visit(OpProperty& op_property)
{
    seen_nodes.insert(op_property.property.object);
    setted_vars.insert({ op_property.property.var_with_property, op_property.property.value });

    if (op_property.property.type == VarType::Node) {
        base_plans.push_back(std::make_unique<NodePropertyPlan>(
            op_property.property.object,
            op_property.property.key,
            op_property.property.value
        ));
    } else {
        base_plans.push_back(std::make_unique<EdgePropertyPlan>(
            op_property.property.object,
            op_property.property.key,
            op_property.property.value
        ));
    }
}

std::unique_ptr<BindingIter>
    PathBindingIterConstructor::get_pending_properties(std::unique_ptr<BindingIter> binding_iter)
{
    std::vector<ExprProperty> var_properties;
    for (auto& [expr_var_property, assigned] : used_properties) {
        if (!assigned) {
            assigned = true;
            var_properties.push_back(expr_var_property);
        }
    }

    if (var_properties.size() > 0) {
        return std::make_unique<AssignProperties>(std::move(binding_iter), std::move(var_properties));
    }

    return binding_iter;
}
