#include "binding_iter_constructor.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/gql/comparisons.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/expr_evaluator.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/hash_join/generic/hybrid/left_join.h"
#include "query/executor/binding_iter/index_left_outer_join.h"
#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/order_by.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/set_constants.h"
#include "query/executor/binding_iter/set_labels.h"
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
get_non_redundant_exprs_gql(std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>&& exprs) {
    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> res;

    for (auto&& [var, e] : exprs) {
        auto casted_expr_var = dynamic_cast<BindingExprVar*>(e.get());
        if (casted_expr_var) {
            if (casted_expr_var->var == var) {
                // avoid redundant assignation
                continue;
            }
        }

        res.push_back({var, std::move(e)});
    }
    exprs.clear(); // vectors are not guaranteed to be empty after the move, so we make sure of that
    return res;
}

void BindingIterConstructor::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    assert(op_order_by.items.size() > 0);

    this->op_order_by = &op_order_by;
}

void BindingIterConstructor::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
    std::vector<VarId> order_vars;
    if (op_order_by) {
        for (auto& item : op_order_by->items) {
            auto casted_expr_var = dynamic_cast<ExprVar*>(item.get());
            auto casted_expr_property = dynamic_cast<ExprProperty*>(item.get());
            if (casted_expr_var) {
                order_vars.push_back(casted_expr_var->id);
                order_saved_vars.insert(casted_expr_var->id);
            } else if (casted_expr_property){
                order_vars.push_back(casted_expr_property->value);
                order_saved_vars.insert(casted_expr_property->value);
            } else {
                auto expr = item.get();
                auto var = get_query_ctx().get_internal_var();

                order_vars.push_back(var);
                order_saved_vars.insert(var);

                ExprToBindingExpr expr_to_binding_expr(this, {});

                expr->accept_visitor(expr_to_binding_expr);

                projection_order_exprs.emplace_back(
                        var,
                        std::move(expr_to_binding_expr.tmp)
                    );
            }
        }
        auto non_redundant_expr_eval = get_non_redundant_exprs_gql(std::move(projection_order_exprs));
        if (non_redundant_expr_eval.size() > 0) {
            tmp = std::make_unique<ExprEvaluator>(
                std::move(tmp),
                std::move(non_redundant_expr_eval)
            );
        }
        for (auto& var : op_return.get_all_vars()) {
            order_saved_vars.insert(var);
        }
        if (op_order_by->null_order[0]){
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

    if (projection_exprs.size() > 0) {
        tmp = std::make_unique<ExprEvaluator>(std::move(tmp), std::move(projection_exprs));
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

        std::vector<VarId> rhs_vars = { property.object, property.value };
        tmp = std::make_unique<IndexLeftOuterJoin>(
            std::move(tmp),
            std::move(index_scan),
            std::move(rhs_vars)
        );
    }
}

void BindingIterConstructor::visit(OpGraphPatternList& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void BindingIterConstructor::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);
}

void BindingIterConstructor::visit(OpPathUnion& op)
{
    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
    }
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

void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    for (auto& pattern : op_basic_graph_pattern.patterns) {
        bgp_depth++;
        pattern->accept_visitor(*this);
        bgp_depth--;
    }

    for (auto& node_id : possible_disjoint_nodes) {
        if (!seen_nodes.count(node_id)) {
            base_plans.push_back(std::make_unique<NodePlan>(node_id));
        }
    }

    // TODO: maybe this should be in a higher scope
    // only when depth = 0, we get the iter
    if (bgp_depth > 0) {
        return;
    }

    // Build the basic graph pattern join
    for (auto& plan : base_plans) {
        plan->set_input_vars(safe_assigned_vars);
    }

    // try to use leapfrog if there is a join
    if (base_plans.size() > 1) {
        if (safe_assigned_vars.size() > 0) {
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

    // insert new safe_assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            safe_assigned_vars.insert(var);
        }
    }

    if (!setted_vars.empty()) {
        tmp = std::make_unique<SetConstants>(std::move(tmp), setted_vars);
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
    for (auto& op_property : op_property.properties) {
        seen_nodes.insert(op_property.object);

        setted_vars.push_back({ op_property.var_with_property, op_property.value });

        if (op_property.type == VarType::Node) {
            base_plans.push_back(
                std::make_unique<NodePropertyPlan>(op_property.object, op_property.key, op_property.value)
            );
        } else {
            base_plans.push_back(
                std::make_unique<EdgePropertyPlan>(op_property.object, op_property.key, op_property.value)
            );
        }
    }
}
