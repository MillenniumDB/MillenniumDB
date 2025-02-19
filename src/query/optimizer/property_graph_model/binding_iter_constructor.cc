#include "binding_iter_constructor.h"

#include "graph_models/gql/comparisons.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/expr_evaluator.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/index_left_outer_join.h"
#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/order_by.h"
#include "query/executor/binding_iter/sequence.h"
#include "query/executor/binding_iter/set_constants.h"
#include "query/executor/binding_iter/set_end_boundary_variable.h"
#include "query/executor/binding_iter/set_labels.h"
#include "query/executor/binding_iter/set_repeated_variable.h"
#include "query/executor/binding_iter/set_start_boundary_variable.h"
#include "query/executor/binding_iter/union.h"
#include "query/executor/binding_iters.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_optimizer.h"
#include "query/optimizer/plan/join_order/selinger_optimizer.h"
#include "query/optimizer/plan/plan.h"
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

constexpr auto MAX_SELINGER_PLANS = 0;

std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>
    get_non_redundant_exprs_gql(std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>&& exprs)
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

        res.push_back({ var, std::move(e) });
    }
    exprs.clear(); // vectors are not guaranteed to be empty after the move, so we make sure of that
    return res;
}

void BindingIterConstructor::visit(OpOrderBy& op_order_by)
{
    for (const auto& item : op_order_by.items) {
        if (item->has_aggregation()) {
            grouping = true;
            break;
        }
    }

    op_order_by.op->accept_visitor(*this);

    assert(op_order_by.items.size() > 0);

    this->op_order_by = &op_order_by;
}

void BindingIterConstructor::visit(OpReturn& op_return)
{
    for (auto& item : op_return.return_items) {
        if (item.expr != nullptr && item.expr->has_aggregation()) {
            grouping = true;
            break;
        }
    }
    op_return.op->accept_visitor(*this);

    std::vector<VarId> order_vars;
    if (op_order_by) {
        for (auto& item : op_order_by->items) {
            auto casted_expr_var = dynamic_cast<ExprVar*>(item.get());
            auto casted_expr_property = dynamic_cast<ExprProperty*>(item.get());
            if (casted_expr_var) {
                order_vars.push_back(casted_expr_var->id);
                order_saved_vars.insert(casted_expr_var->id);
            } else if (casted_expr_property) {
                order_vars.push_back(casted_expr_property->value);
                order_saved_vars.insert(casted_expr_property->value);
            } else {
                auto expr = item.get();
                auto var = get_query_ctx().get_internal_var();

                order_vars.push_back(var);
                order_saved_vars.insert(var);

                ExprToBindingExpr expr_to_binding_expr(this, {});

                expr->accept_visitor(expr_to_binding_expr);

                projection_order_exprs.emplace_back(var, std::move(expr_to_binding_expr.tmp));
            }
        }
        auto non_redundant_expr_eval = get_non_redundant_exprs_gql(std::move(projection_order_exprs));
        if (non_redundant_expr_eval.size() > 0) {
            tmp = std::make_unique<ExprEvaluator>(std::move(tmp), std::move(non_redundant_expr_eval));
        }
        for (auto& var : op_return.get_all_vars()) {
            order_saved_vars.insert(var);
        }
        if (op_order_by->null_order[0]) {
            tmp = std::make_unique<OrderBy>(
                std::move(tmp),
                std::move(order_saved_vars),
                std::move(order_vars),
                std::move(op_order_by->ascending_order),
                &GQL::Comparisons::compare_null_first
            );
        } else {
            tmp = std::make_unique<OrderBy>(
                std::move(tmp),
                std::move(order_saved_vars),
                std::move(order_vars),
                std::move(op_order_by->ascending_order),
                &GQL::Comparisons::compare_null_last
            );
        }
        op_order_by = nullptr;
    }



    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> projection_exprs;

    for (auto& item : op_return.return_items) {
        if (item.alias.has_value()) {
            ExprToBindingExpr expr_to_binding_expr(this, item.alias.value());
            item.expr->accept_visitor(expr_to_binding_expr);

            projection_exprs.emplace_back(item.alias.value(), std::move(expr_to_binding_expr.tmp));
        }
    }

    if (aggregations.size() > 0 || group_vars.size() > 0) {
        tmp = std::make_unique<Aggregation>(
            std::move(tmp),
            std::move(aggregations),
            std::move(group_vars)
        );
    }

    if (projection_exprs.size() > 0) {
        tmp = std::make_unique<ExprEvaluator>(std::move(tmp), std::move(projection_exprs));
    }

    if (op_return.distinct) {
        auto projected_vars_set = op_return.get_expr_vars();
        std::vector<VarId> projected_vars;
        std::copy(projected_vars_set.begin(), projected_vars_set.end(), std::back_inserter(projected_vars));
        tmp = std::make_unique<DistinctHash>(std::move(tmp), std::move(projected_vars));
    }
}

void BindingIterConstructor::visit(OpOptProperties& op_opt_properties)
{
    op_opt_properties.op->accept_visitor(*this);

    for (auto& property : op_opt_properties.properties) {
        const auto it = std::find_if(
            setted_vars.begin(),
            setted_vars.end(),
            [&](const std::pair<VarId, ObjectId>& pair) { return pair.first == property.value; }
        );
        if (it != setted_vars.end()) {
            continue;
        }

        std::array<std::unique_ptr<ScanRange>, 3> ranges { ScanRange::get(property.object, true),
                                                           ScanRange::get(property.key),
                                                           ScanRange::get(property.value, false) };

        std::unique_ptr<IndexScan<3>> index_scan;

        if (property.type == VarType::Node) {
            index_scan = std::make_unique<IndexScan<3>>(*gql_model.node_key_value, std::move(ranges));
        } else {
            index_scan = std::make_unique<IndexScan<3>>(*gql_model.edge_key_value, std::move(ranges));
        }

        std::vector<VarId> rhs_vars = { property.value };
        tmp = std::make_unique<IndexLeftOuterJoin>(
            std::move(tmp),
            std::move(index_scan),
            std::move(rhs_vars)
        );
    }
}

void BindingIterConstructor::visit(OpGraphPatternList& op)
{
    std::unique_ptr<BindingIter> current_tmp;

    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);

        if (current_tmp == nullptr) {
            current_tmp = std::move(tmp);
        } else {
            current_tmp = std::make_unique<IndexNestedLoopJoin>(std::move(current_tmp), std::move(tmp));
        }
        node_at_left = false;
    }
    tmp = std::move(current_tmp);
}

void BindingIterConstructor::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);

    if (op.path_var_id.has_value()) {
        // tmp = std::make_unique<SetPath>(std::move(tmp), op.path_var_id.value());
    }
}

void BindingIterConstructor::visit(OpPathUnion& op_union)
{
    std::vector<std::unique_ptr<BindingIter>> iters;

    std::set<VarId> previous_assigned_vars = assigned_vars;
    bool previous_end_var = node_at_left;

    for (auto& pattern : op_union.op_list) {
        pattern->accept_visitor(*this);
        iters.push_back(std::move(tmp));

        assigned_vars = previous_assigned_vars;
        node_at_left = previous_end_var;
    }
    tmp = std::make_unique<Union>(std::move(iters));
    node_at_left = true;
}

void BindingIterConstructor::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    std::vector<std::unique_ptr<BindingExpr>> binding_exprs;

    ExprToBindingExpr expr_to_binding_expr;
    for (auto& expr : op_filter.exprs) {
        expr->accept_visitor(expr_to_binding_expr);
        binding_exprs.push_back(std::move(expr_to_binding_expr.tmp));
    }

    tmp = std::make_unique<Filter>(
        &SPARQL::Conversions::to_boolean,
        std::move(tmp),
        std::move(binding_exprs)
    );
}

void BindingIterConstructor::visit(OpOptLabels& op)
{
    op.op->accept_visitor(*this);
    tmp = std::make_unique<SetLabels>(std::move(tmp), op.labels);
}

void BindingIterConstructor::visit(OpRepetition& op_repetition)
{
    op_repetition.op->accept_visitor(*this);
}

void BindingIterConstructor::visit(OpLinearPattern& op_linear_pattern)
{
    base_plans.clear();
    possible_disjoint_nodes.clear();
    seen_nodes.clear();

    for (auto& pattern : op_linear_pattern.patterns) {
        pattern->accept_visitor(*this);
    }

    for (auto& node_id : possible_disjoint_nodes) {
        if (!seen_nodes.count(node_id)) {
            base_plans.push_back(std::make_unique<NodePlan>(node_id));
        }
    }

    // assign the leftmost variable
    if (node_at_left && op_linear_pattern.start != nullptr) {
        std::set<VarId> input_var = { *op_linear_pattern.start };
        for (auto& plan : base_plans) {
            plan->set_input_vars(input_var);
        }
    }

    // Build the basic graph pattern join
    for (auto& plan : base_plans) {
        plan->set_input_vars(assigned_vars);
    }

    // try to use leapfrog if there is a join
    if (base_plans.size() > 1) {
        if (assigned_vars.size() > 0) {
            tmp = LeapfrogOptimizer::try_get_iter_with_assigned(base_plans);
        } else {
            tmp = LeapfrogOptimizer::try_get_iter_without_assigned(base_plans);
        }
    }

    if (tmp == nullptr) {
        std::unique_ptr<Plan> root_plan = nullptr;
        if (base_plans.size() <= MAX_SELINGER_PLANS) {
            SelingerOptimizer selinger_optimizer(base_plans);
            root_plan = selinger_optimizer.get_plan();
        } else {
            root_plan = GreedyOptimizer::get_plan(base_plans);
        }

        tmp = root_plan->get_binding_iter();
    }

    if (!setted_vars.empty()) {
        tmp = std::make_unique<SetConstants>(std::move(tmp), setted_vars);
    }

    // set start and end vars
    if (node_at_left && op_linear_pattern.start != nullptr) {
        get_query_ctx().get_or_create_var(".end");

        if (all_seen_vars.count(*op_linear_pattern.start)) {
            tmp = std::make_unique<SetRepeatedVariable>(std::move(tmp), *op_linear_pattern.start);
        } else {
            tmp = std::make_unique<SetStartBoundaryVariable>(std::move(tmp), *op_linear_pattern.start);
        }
    }

    if (op_linear_pattern.end != nullptr) {
        get_query_ctx().get_or_create_var(".end");
        tmp = std::make_unique<SetEndBoundaryVariable>(std::move(tmp), *op_linear_pattern.end);
    }

    // insert new assigned vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            assigned_vars.insert(var);
        }
    }
    all_seen_vars.insert(assigned_vars.begin(), assigned_vars.end());

    node_at_left = true;
}

void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::vector<std::unique_ptr<BindingIter>> iters;

    for (auto& pattern : op_basic_graph_pattern.patterns) {
        pattern->accept_visitor(*this);
        iters.push_back(std::move(tmp));
    }
    if (iters.size() > 1) {
        tmp = std::make_unique<Sequence>(std::move(iters));
    } else {
        tmp = std::move(iters[0]);
    }
}

void BindingIterConstructor::visit(OpNode& op_node)
{
    possible_disjoint_nodes.insert(op_node.id);
}

void BindingIterConstructor::visit(OpEdge& op_edge)
{
    seen_nodes.insert(op_edge.from);
    seen_nodes.insert(op_edge.to);

    switch (op_edge.type) {
    case OpEdge::Directed:
        base_plans.push_back(std::make_unique<DirectedEdgePlan>(op_edge.id, op_edge.from, op_edge.to));
        break;
    case OpEdge::Undirected:
        base_plans.push_back(std::make_unique<UndirectedEdgePlan>(op_edge.id, op_edge.from, op_edge.to));
        break;
    case OpEdge::UndirectedOrDirectedTo:
        base_plans.push_back(
            std::make_unique<UndirectedDirectedEdgePlan>(op_edge.id, op_edge.from, op_edge.to)
        );
        break;
    case OpEdge::DirectedLeftOrRight:
        base_plans.push_back(std::make_unique<DirectedLeftRightEdgePlan>(op_edge.id, op_edge.from, op_edge.to)
        );
        break;
    case OpEdge::AnyDirection:
        base_plans.push_back(std::make_unique<AnyDirectionEdgePlan>(op_edge.id, op_edge.from, op_edge.to));
        break;
    }
}

void BindingIterConstructor::visit(OpNodeLabel& op_node_label)
{
    seen_nodes.insert(op_node_label.node_id);
    base_plans.push_back(std::make_unique<NodeLabelPlan>(op_node_label.node_id, op_node_label.label_id));
}

void BindingIterConstructor::visit(OpEdgeLabel& op_edge_label)
{
    base_plans.push_back(std::make_unique<EdgeLabelPlan>(op_edge_label.edge_id, op_edge_label.label_id));
}

void BindingIterConstructor::visit(OpProperty& op_property)
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
