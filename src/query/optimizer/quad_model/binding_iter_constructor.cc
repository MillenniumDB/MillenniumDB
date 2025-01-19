#include "binding_iter_constructor.h"

#include <cassert>
#include <memory>

#include "graph_models/quad_model/comparisons.h"
#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "misc/set_operations.h"
#include "query/executor/binding_iter/aggregation.h"
#include "query/executor/binding_iter/cross_product.h"
#include "query/executor/binding_iter/distinct_hash.h"
#include "query/executor/binding_iter/empty_binding_iter.h"
#include "query/executor/binding_iter/expr_evaluator.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/hash_join/bgp/in_memory/join_1_var.h"
#include "query/executor/binding_iter/index_left_outer_join.h"
#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/order_by.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/executor/binding_iter/single_result_binding_iter.h"
#include "query/executor/binding_iter/text_search_index_scan.h"
#include "query/executor/binding_iters.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_optimizer.h"
#include "query/optimizer/plan/join_order/selinger_optimizer.h"
#include "query/optimizer/quad_model/expr_property_types_visitor.h"
#include "query/optimizer/quad_model/expr_to_binding_expr.h"
#include "query/optimizer/quad_model/plan/disjoint_object_plan.h"
#include "query/optimizer/quad_model/plan/edge_plan.h"
#include "query/optimizer/quad_model/plan/label_plan.h"
#include "query/optimizer/quad_model/plan/path_plan.h"
#include "query/optimizer/quad_model/plan/property_plan.h"
#include "query/optimizer/quad_model/plan/property_type_plan.h"
#include "query/parser/op/mql/op_where.h"
#include "query/parser/op/mql/ops.h"
#include "query/parser/op/op_visitor.h"

using namespace MQL;

constexpr auto MAX_SELINGER_PLANS = 0;
namespace MQL {
std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>
    get_non_redundant_exprs(std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>&& exprs)
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
    exprs.clear(); // vectors are not guaranteed to be empty after the move, so we make sure of that
    return res;
}
}

BindingIterConstructor::BindingIterConstructor(std::map<VarId, ObjectId>& setted_vars) :
    setted_vars(setted_vars)
{
    begin_at_left.resize(get_query_ctx().get_var_size());
}

void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    // Handle Text Searches
    std::vector<std::unique_ptr<TextSearchIndexScan>> text_search_binding_iters;
    if (op_basic_graph_pattern.expr_text_searches.size() > 1) {
        // TODO: Multiple text searches
        throw QueryException("Multiple Text Searches not supported yet");
    }
    for (const auto& expr_text_search : op_basic_graph_pattern.expr_text_searches) {
        safe_assigned_vars.insert(expr_text_search.object_var);
        text_search_binding_iters.emplace_back(std::make_unique<TextSearchIndexScan>(
            *expr_text_search.text_search_index_ptr,
            expr_text_search.query,
            expr_text_search.search_type,
            expr_text_search.object_var,
            expr_text_search.match_var
        ));
    }

    std::vector<PropertyTypeConstraint> pattern_fixed_types_properties;
    for (const auto& property : properties_types_queries) {
        if (property.type_bitmap == 0) {
            throw QueryException("Impossible type query");
        }
        if (!(property.type_bitmap & static_cast<uint64_t>(PropertyType::TYPE_NULL))
            && boost::core::popcount(property.type_bitmap) == 1)
        {
            for (auto i = op_basic_graph_pattern.optional_properties.begin();
                 i != op_basic_graph_pattern.optional_properties.end();)
            {
                if (i->value.is_var() && i->value.get_var().id == property.var_with_property.id) {
                    fixed_types_properties.push_back(property);
                    pattern_fixed_types_properties.push_back(property);
                    op_basic_graph_pattern.optional_properties.erase(i);
                    break;
                } else {
                    i++;
                    assert(i != op_basic_graph_pattern.optional_properties.end());
                }
            }
        }
    }

    // Process disjoint Terms
    // if a term is not found we can assume the MATCH result is empty
    for (auto& disjoint_term : op_basic_graph_pattern.disjoint_terms) {
        if (!term_exists(disjoint_term.term)) {
            tmp = std::make_unique<EmptyBindingIter>();
            return;
        }
    }

    std::vector<std::unique_ptr<Plan>> base_plans;


    // Process Labels
    for (auto& op_label : op_basic_graph_pattern.labels) {
        base_plans.push_back(
            std::make_unique<LabelPlan>(replace_setted_var(op_label.node), replace_setted_var(op_label.label))
        );
    }

    // Process properties (value is fixed)
    for (auto& op_property : op_basic_graph_pattern.properties) {
        base_plans.push_back(std::make_unique<PropertyPlan>(
            replace_setted_var(op_property.node),
            op_property.key,
            replace_setted_var(op_property.value)
        ));
    }

    // Process properties (type is fixed)
    for (auto& property : pattern_fixed_types_properties) {
        base_plans.push_back(std::make_unique<PropertyTypePlan>(
            property.var_without_property, // TODO: replace_setted_var
            property.key,
            property.var_with_property,
            property.type_bitmap
        ));
    };

    // Push equalities from where into the basic graph pattern
    /*
    if (!pushed_fixed_properties) {
        pushed_fixed_properties = true;
        for (auto&& [var, key, value, value_var] : fixed_properties) {
            base_plans.push_back(std::make_unique<PropertyPlan>(replace_setted_var(var), key, value));
            setted_vars.insert({ value_var, value });
        }
    }*/

    // Process connections
    for (auto& op_edge : op_basic_graph_pattern.edges) {
        base_plans.push_back(std::make_unique<EdgePlan>(
            replace_setted_var(op_edge.from),
            replace_setted_var(op_edge.to),
            replace_setted_var(op_edge.type),
            replace_setted_var(op_edge.edge)
        ));
    }

    // Process property paths
    for (auto& path : op_basic_graph_pattern.paths) {
        base_plans.push_back(std::make_unique<PathPlan>(
            begin_at_left,
            path.direction,
            path.var,
            replace_setted_var(path.from),
            replace_setted_var(path.to),
            *path.path,
            path.semantic
        ));
    }

    std::set<VarId> join_vars;

    for (auto& plan: base_plans) {
        auto plan_vars = plan->get_vars();
        join_vars.insert(plan_vars.begin(), plan_vars.end());
    }

    // Process disjoint Vars
    for (auto& disjoint_var : op_basic_graph_pattern.disjoint_vars) {
        if (join_vars.find(disjoint_var.var) != join_vars.end()) {
            continue;
        }
        if (safe_assigned_vars.find(disjoint_var.var) != safe_assigned_vars.end()) {
            continue;
        }
        auto setted_var_found = setted_vars.find(disjoint_var.var);
        if (setted_var_found != setted_vars.end()) {
            if (!term_exists(setted_var_found->second)) {
                tmp = std::make_unique<EmptyBindingIter>();
                return;
            }
        } else {
            // we could have something like: MATCH (?x) WHERE ?x.age == 1.
            // ?x is not really an disjoint var
            bool join_with_where_property_equality = false;
            /*
            for (auto&& [var, key, value, value_var] : fixed_properties) {
                if (var == disjoint_var.var) {
                    join_with_where_property_equality = true;
                    break;
                }
            }*/
            if (!join_with_where_property_equality) {
                base_plans.push_back(std::make_unique<DisjointObjectPlan>(disjoint_var.var));
            }
        }
    }

    assert(tmp == nullptr);

    // Build the basic graph pattern join
    auto build_bgp_iter = [&]() {
        // Set input vars
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
    };

    if (base_plans.size() > 0) {
        build_bgp_iter();
    }

    if (!text_search_binding_iters.empty()) {
        // TODO: Multiple text searches
        if (base_plans.size() > 0) {
            tmp = std::make_unique<IndexNestedLoopJoin>(
                std::move(text_search_binding_iters[0]),
                std::move(tmp));
        } else {
            tmp = std::move(text_search_binding_iters[0]);
        }
    }

    // insert new safe_assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            safe_assigned_vars.insert(var);
        }
    }

    // TODO: Optimize when property variable is already in the text serach
    for (auto& property : op_basic_graph_pattern.optional_properties)
    {
        // ignore if it is in mandatory_properties
        bool skip = false;
        /*
        for (auto&& [fp_var, fp_key, fp_value, fp_value_var] : fixed_properties) {
            if (property.node.is_var() && property.node.get_var() == fp_var && property.key == fp_key) {
                skip = true;
                break;
            }
        }*/
        if (skip) {
            continue;
        }

        std::array<std::unique_ptr<ScanRange>, 3> ranges { ScanRange::get(property.node, true),
                                                           ScanRange::get(property.key),
                                                           ScanRange::get(property.value, false) };
        auto index_scan = std::make_unique<IndexScan<3>>(*quad_model.object_key_value, std::move(ranges));
        std::vector<VarId> rhs_only_vars = { property.value.get_var() };
        tmp = std::make_unique<IndexLeftOuterJoin>( // Mover
            std::move(tmp),
            std::move(index_scan),
            std::move(rhs_only_vars)
        );
    }
}

void BindingIterConstructor::visit(OpWhere& op_where)
{
    properties_types_queries.clear();
    ExprPropertyTypes expr_property_types(this);
    // visitor collect final types bitmaps of properties in propId_and_bitmap dict
    op_where.expr->accept_visitor(expr_property_types);
    for (auto& p_type : properties_types_queries) {
        auto key = p_type.var_with_property.id;
        auto it = expr_property_types.propId_and_bitmap->find(key);
        p_type.type_bitmap = it->second;
    }

    op_where.op->accept_visitor(*this);
    ExprToBindingExpr expr_to_binding_expr(fixed_types_properties);
    op_where.expr->accept_visitor(expr_to_binding_expr);

    auto binding_expr = std::move(expr_to_binding_expr.tmp);

    auto potential_expr_term = dynamic_cast<BindingExprTerm*>(binding_expr.get());
    auto is_trivial =
        potential_expr_term != nullptr && potential_expr_term->object_id == ObjectId(ObjectId::BOOL_TRUE);
    auto is_always_false =
        potential_expr_term != nullptr && potential_expr_term->object_id == ObjectId(ObjectId::BOOL_FALSE);

    if (binding_expr == nullptr || is_trivial)
        return;

    if (is_always_false) {
        tmp = std::make_unique<EmptyBindingIter>();
    } else {
        std::vector<std::unique_ptr<BindingExpr>> exprs;
        exprs.push_back(std::move(binding_expr));
        tmp = std::make_unique<Filter>(&MQL::Conversions::to_boolean, std::move(tmp), std::move(exprs));
    }
}


void BindingIterConstructor::visit(OpOptional& op_optional)
{
    const auto parent_vars = op_optional.op->get_all_vars();

    op_optional.op->accept_visitor(*this);

    for (auto& optional : op_optional.optionals) {
        auto lhs = std::move(tmp);
        optional->accept_visitor(*this);

        const auto rhs_vars = optional->get_all_vars();
        auto join_vars = misc::set_intersection(parent_vars, rhs_vars);
        auto rhs_only_vars = misc::set_difference(rhs_vars, join_vars);

        tmp = std::make_unique<IndexLeftOuterJoin>(
            std::move(lhs),
            std::move(tmp),
            misc::set_to_vector(rhs_only_vars)
        );
    }
}


void BindingIterConstructor::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    grouping = true;

    this->op_group_by = &op_group_by;

    for (auto& var : op_group_by.vars) {
        group_vars.insert(var);
        group_saved_vars.insert(var);
    }
}


void BindingIterConstructor::visit(OpOrderBy& op_order_by)
{
    for (const auto& item : op_order_by.items) {
        if (std::holds_alternative<std::unique_ptr<Expr>>(item)) {
            auto& expr = std::get<std::unique_ptr<Expr>>(item);
            if (expr->has_aggregation()) {
                grouping = true;
                break;
            }
        }
    }

    op_order_by.op->accept_visitor(*this);

    assert(op_order_by.items.size() > 0);

    this->op_order_by = &op_order_by;
}


void BindingIterConstructor::visit(OpReturn& op_return)
{
    for (auto&& [var, expr] : op_return.projection) {
        if (expr != nullptr && expr->has_aggregation()) {
            grouping = true;
            break;
        }
    }
    op_return.op->accept_visitor(*this);

    for (auto& [var, expr] : op_return.projection) {
        projected_vars.emplace_back(var);

        if (expr != nullptr) {
            ExprToBindingExpr expr_to_binding_expr(this, var);
            expr->accept_visitor(expr_to_binding_expr);

            projection_order_exprs.emplace_back(var, std::move(expr_to_binding_expr.tmp));
        } else {
            if (grouping && group_vars.find(var) == group_vars.end()) {
                throw QuerySemanticException(
                    "Invalid use of var \"" + get_query_ctx().get_var_name(var) + "\" in RETURN"
                );
            }
        }
    }

    if (op_order_by) {
        for (uint_fast32_t i = 0; i < op_order_by->items.size(); i++) {
            if (std::holds_alternative<VarId>(op_order_by->items[i])) {
                auto& var = std::get<VarId>(op_order_by->items[i]);
                order_by_vars.emplace_back(var);
                order_by_saved_vars.insert(var);
                group_saved_vars.insert(var);
            } else {
                auto& expr = std::get<std::unique_ptr<Expr>>(op_order_by->items[i]);
                auto var = get_query_ctx().get_internal_var();

                order_by_vars.emplace_back(var);
                order_by_saved_vars.insert(var);

                ExprToBindingExpr expr_to_binding_expr(this, {});

                expr->accept_visitor(expr_to_binding_expr);

                projection_order_exprs.emplace_back(var, std::move(expr_to_binding_expr.tmp));
            }
        }
    }

    if (op_group_by) {
        std::vector<bool> ascending(group_vars.size(), true);

        std::vector<VarId> group_vars_vector(group_vars.begin(), group_vars.end());

        tmp = std::make_unique<OrderBy>(
            std::move(tmp),
            std::move(group_saved_vars),
            std::move(group_vars_vector),
            std::move(ascending),
            &MQL::Comparisons::compare
        );
    }

    if (aggregations.size() > 0 || group_vars.size() > 0) {
        tmp = std::make_unique<Aggregation>(
            std::move(tmp),
            std::move(aggregations),
            std::move(group_vars)
        );
    }if (aggregations.size() > 0 || group_vars.size() > 0) {
        tmp = std::make_unique<Aggregation>(
            std::move(tmp),
            std::move(aggregations),
            std::move(group_vars)
        );
    }
    if (aggregations.size() > 0 || group_vars.size() > 0) {
        tmp = std::make_unique<Aggregation>(std::move(tmp), std::move(aggregations), std::move(group_vars));
    }

    assert(tmp != nullptr);
    auto non_redundant_expr_eval = get_non_redundant_exprs(std::move(projection_order_exprs));
    if (non_redundant_expr_eval.size() > 0) {
        tmp = std::make_unique<ExprEvaluator>(std::move(tmp), std::move(non_redundant_expr_eval));
    }

    if (op_order_by) {
        for (const auto& var : projected_vars) {
            order_by_saved_vars.insert(var);
        }

        tmp = std::make_unique<OrderBy>(
            std::move(tmp),
            std::move(order_by_saved_vars),
            std::move(order_by_vars),
            std::move(op_order_by->ascending_order),
            &MQL::Comparisons::compare
        );
    }

    if (op_return.distinct) {
        // TODO: if everything is ordered having the distinct variables at first we can avoid the hash
        tmp = std::make_unique<DistinctHash>(std::move(tmp), std::move(projected_vars));
    }

    if (op_return.offset != Op::DEFAULT_OFFSET || op_return.limit != Op::DEFAULT_LIMIT
        || quad_model.MAX_LIMIT != Op::DEFAULT_LIMIT)
    {
        auto limit = op_return.limit;
        if (op_return.limit > quad_model.MAX_LIMIT) {
            limit = quad_model.MAX_LIMIT;
        }
        tmp = std::make_unique<Slice>(std::move(tmp), op_return.offset, limit);
    }
}


Id BindingIterConstructor::replace_setted_var(Id id) const
{
    if (id.is_var()) {
        auto var = id.get_var();
        auto found_setted_var = setted_vars.find(var);
        if (found_setted_var == setted_vars.end()) {
            return id;
        } else {
            return found_setted_var->second;
        }
    } else {
        return id;
    }
}


bool BindingIterConstructor::term_exists(ObjectId term) const
{
    if (term.is_not_found()) {
        return false;
    } else if ((term.id & ObjectId::TYPE_MASK) == ObjectId::MASK_ANON_INLINED) {
        auto anon_id = term.id & ObjectId::VALUE_MASK;
        return anon_id <= quad_model.catalog.anonymous_nodes_count;
    } else if ((term.id & ObjectId::TYPE_MASK) == ObjectId::MASK_EDGE) {
        auto conn_id = term.id & ObjectId::VALUE_MASK;

        // TODO: will be wrong after implementing deletes
        return conn_id <= quad_model.catalog.edge_count;
    } else {
        // search in nodes
        Record<1> r = { term.id };
        bool interruption_requested = false;
        auto it = quad_model.nodes->get_range(&interruption_requested, r, r);
        return it.next() != nullptr;
    }
}
