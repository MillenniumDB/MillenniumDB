#include "binding_iter_constructor.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <variant>

#include "graph_models/rdf_model/comparisons.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/aggregation.h"
#include "query/executor/binding_iter/bind.h"
#include "query/executor/binding_iter/distinct_hash.h"
#include "query/executor/binding_iter/empty_binding_iter.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/group_expressions.h"
#include "query/executor/binding_iter/hash_join/generic/in_memory/anti_join.h"
#include "query/executor/binding_iter/hash_join/generic/in_memory/join.h"
#include "query/executor/binding_iter/hash_join/generic/in_memory/left_join.h"
#include "query/executor/binding_iter/hash_join/generic/in_memory/semi_join.h"
#include "query/executor/binding_iter/hash_join/left_cross_product.h"
#include "query/executor/binding_iter/index_left_outer_join.h"
#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/nested_loop_anti_join.h"
#include "query/executor/binding_iter/nested_loop_join.h"
#include "query/executor/binding_iter/nested_loop_left_join.h"
#include "query/executor/binding_iter/nested_loop_semi_join.h"
#include "query/executor/binding_iter/no_free_variable_minus.h"
#include "query/executor/binding_iter/order_by_id.h"
#include "query/executor/binding_iter/projection_order_exprs.h"
#include "query/executor/binding_iter/single_result_binding_iter.h"
#include "query/executor/binding_iter/slice.h"
#include "query/executor/binding_iter/sparql_service.h"
#include "query/executor/binding_iter/sub_select.h"
#include "query/executor/binding_iter/union.h"
#include "query/executor/binding_iter/values.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_optimizer.h"
#include "query/optimizer/rdf_model/expr_to_binding_expr.h"
#include "query/optimizer/rdf_model/plan/path_plan.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"
#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/op_select.h"
#include "query/rewriter/mql/op/optimize_optional_tree.h"
#include "query/var_id.h"

using namespace SPARQL;

static inline std::set<VarId> set_intersection(const std::set<VarId>& set1, const std::set<VarId>& set2) {
    std::set<VarId> intersection;

    std::set_intersection(
        set1.cbegin(), set1.cend(),
        set2.cbegin(), set2.cend(),
        std::inserter(intersection, intersection.begin())
    );

    return intersection;
}

void BindingIterConstructor::print_set(const std::set<VarId>& set) const {
    std::cout << "{";
    for (const auto& var: set) {
        std::cout << " " << get_query_ctx().get_var_name(var);
    }
    std::cout << " }\n";
}

static inline std::set<VarId> set_union(const std::set<VarId>& set1, const std::set<VarId>& set2) {
    std::set<VarId> res;

    std::set_union(
        set1.cbegin(), set1.cend(),
        set2.cbegin(), set2.cend(),
        std::inserter(res, res.begin())
    );

    return res;
}

// set1 - set2
static inline std::set<VarId> set_difference(const std::set<VarId>& set1, const std::set<VarId>& set2) {
    std::set<VarId> res;

    for (const auto& var : set1) {
        if (set2.find(var) == set2.end()) {
            res.insert(var);
        }
    }

    return res;
}

static inline std::vector<VarId> set_to_vector(const std::set<VarId>& set) {
    std::vector<VarId> vec;
    vec.reserve(set.size());
    std::copy(set.begin(), set.end(), std::back_inserter(vec));
    return vec;
}


BindingIterConstructor::BindingIterConstructor() {
    begin_at_left.resize(get_query_ctx().get_var_size());
}

void BindingIterConstructor::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
}


bool BindingIterConstructor::is_aggregation_or_group_var(VarId var_id) const {
    if (aggregations.find(var_id) != aggregations.end()) {
        return true;
    }
    for (const auto& [var_id_, expr, is_agg] : projection_order_exprs) {
        if (var_id_ == var_id) {
            if (expr && is_agg) {
                return true;
            } else {
                break;
            }
        }
    }
    for (auto& var_id_ : group_vars) {
        if (var_id_ == var_id) {
            return true;
        }
    }
    return false;
}


void BindingIterConstructor::make_solution_modifier_operators(bool is_root_query, bool distinct, uint64_t offset, uint64_t limit) {
    // std::cout << "\naggregations: [ ";
    // for (auto &[var, agg_id] : aggregations) {
    //     std::cout << '?' << get_query_ctx().get_var_name(var) << " ";
    // }
    // std::cout << "]" << std::endl;

    {
        // If the query contains a HAVING clause and aggregations we have to reorder them so
        // that newly created Aggregation is the child of the Filter created previously by HAVING.

        std::unique_ptr<Aggregation> agg;

        // Create the Aggregation if necessary.
        if (aggregations.size() > 0) {
            agg = std::make_unique<Aggregation>(nullptr, std::move(aggregations), std::move(group_vars));
        } else if (group_vars.size() > 0) {
            // We do not have aggregations, however we need Aggregation because it does the grouping too.
            agg = std::make_unique<Aggregation>(nullptr, std::map<VarId, std::unique_ptr<Agg>>(), std::move(group_vars));
        }

        // Check if we have an Aggregation.
        if (agg) {
            // Check if the previously created BindingIter was a Filter.
            auto filter = dynamic_cast<Filter*>(tmp.get());
            if (filter && filter->is_having_filter) {
                // We have to reorder because we have both HAVING and aggregations.
                agg->child = std::move(filter->child);
                filter->child = std::move(agg);
            } else {
                // We do not need to reorder, however we still do need to insert the Aggregation and set it's child.
                agg->child = std::move(tmp);
                tmp = std::move(agg);
            }
        }
    }

    if (projection_order_exprs.size() > 0) {
        tmp = std::make_unique<ProjectionOrderExprs>(std::move(tmp), std::move(projection_order_exprs));
    }

    if (order_vars.size() > 0) {
        for (auto& var : projection_vars) {
            order_saved_vars.insert(var);
        }

        tmp = std::make_unique<OrderById>(
            std::move(tmp),
            std::move(order_saved_vars),
            std::move(order_vars),
            std::move(order_ascending),
            &SPARQL::Comparisons::compare
        );
    }

    if (distinct) {
        auto projection_vars_copy = projection_vars;
        tmp = std::make_unique<DistinctHash>(std::move(tmp), std::move(projection_vars_copy));
    }

    // TODO: Take advantage of order for DISTINCT
    // if (op_select.distinct) {
    //     vector<VarId> projected_var_ids;
    //     for (const auto& [var, var_id] : projection_vars) {
    //         projected_var_ids.push_back(var_id);
    //     }
    //     // TODO: Use DistinctOrdered when possible
    //     if (distinct_ordered_possible) {
    //         tmp = make_unique<DistinctOrdered>(std::move(tmp), std::move(projected_var_ids));
    //     } else {
    //         // TODO: This would never happen with the current operators
    //         if (!distinct_into_id) {
    //             tmp = make_unique<DistinctHash>(std::move(tmp), std::move(projected_var_ids));
    //         }
    //         // else DistinctHash was created when visiting OpWhere
    //     }
    // }

    if (offset != Op::DEFAULT_OFFSET || limit != Op::DEFAULT_LIMIT || rdf_model.MAX_LIMIT != Op::DEFAULT_LIMIT) {
        if (limit > rdf_model.MAX_LIMIT and is_root_query) {
            limit = rdf_model.MAX_LIMIT;
        }
        tmp = std::make_unique<Slice>(std::move(tmp), offset, limit);
    }
}


void BindingIterConstructor::visit(OpDescribe& op_describe) {
    op_describe.op->accept_visitor(*this);

     if (group_vars.size() > 0 || aggregations.size() > 0) {
        for (auto& var : op_describe.vars) {;
            if (not is_aggregation_or_group_var(var)) {
                throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
            }
        }
    }

    // Describe *
    if (op_describe.vars.size() == 0 and op_describe.iris.size() == 0) {
        for (auto var : possible_assigned_vars) {
            op_describe.vars.push_back(var);
        }
    }

    // This is to make sure that the describe_vars are saved
    // when building the solution modifier operators.
    for (auto var : op_describe.vars) {
        order_saved_vars.insert(var);
    }

    make_solution_modifier_operators(true, // is_root_query
                                     false, // distinct
                                     op_describe.offset,
                                     op_describe.limit);
}


void BindingIterConstructor::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);

    for (auto triple : op_construct.triples) {
        for (auto var : triple.get_all_vars()) {
            // Make sure the variables used by CONSTRUCT are saved
            order_saved_vars.insert(var);
            if (group_vars.size() > 0 || aggregations.size() > 0) {
                if (!is_aggregation_or_group_var(var)) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            }
        }
    }

    make_solution_modifier_operators(true,  // is_root_query
                                     false, // distinct
                                     op_construct.offset,
                                     op_construct.limit);
}


void BindingIterConstructor::handle_select(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);

    auto grouping_mode = (group_vars.size() > 0 || aggregations.size() > 0) ? GroupingMode::Grouping : GroupingMode::OptionalGrouping;
    std::set<VarId> group_vars_set;
    for (auto& var_id : group_vars) {
        group_vars_set.insert(var_id);
    }

    // Save any ORDER BY expressions, they should come after PROJECTION expressions.
    auto order_exprs = std::move(projection_order_exprs);

    for (size_t i = 0; i < op_select.vars.size(); i++) {
        auto var   = op_select.vars[i];
        auto& expr = op_select.vars_exprs[i];
        projection_vars.push_back(var);

        if (expr) {
            ExprToBindingExpr expr_to_binding_expr(
                safe_assigned_vars,
                possible_assigned_vars,
                group_vars_set,
                aggregations,
                projection_order_exprs,
                var,
                grouping_mode);

            expr->accept_visitor(expr_to_binding_expr);

            if (aggregations.size() > 0) {
                if (grouping_mode == GroupingMode::OptionalGrouping) {
                    grouping_mode = GroupingMode::Grouping;
                } else if (grouping_mode == GroupingMode::NoGrouping) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            } else {
                if (grouping_mode == GroupingMode::OptionalGrouping) {
                    grouping_mode = GroupingMode::NoGrouping;
                } else if (grouping_mode == GroupingMode::Grouping) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            }

            projection_order_exprs.emplace_back(
                var,
                std::move(expr_to_binding_expr.current_binding_expr),
                aggregations.size() > 0
            );
        } else {
            if (is_aggregation_or_group_var(var)) {
                if (grouping_mode == GroupingMode::OptionalGrouping) {
                    grouping_mode = GroupingMode::Grouping;
                } else if (grouping_mode == GroupingMode::NoGrouping) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            } else {
                if (grouping_mode == GroupingMode::OptionalGrouping) {
                    grouping_mode = GroupingMode::NoGrouping;
                } else if (grouping_mode == GroupingMode::Grouping) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            }
        }
    }

    for (auto& o_expr : order_exprs) {
        projection_order_exprs.push_back(std::move(o_expr));
    }

    make_solution_modifier_operators(!op_select.is_sub_select, // is_root_query
                                     op_select.distinct,
                                     op_select.offset,
                                     op_select.limit);
}


void BindingIterConstructor::visit(OpSelect& op_select) {
    if (op_select.is_sub_select) {
        handle_select(op_select);

        tmp = std::make_unique<SubSelect>(
            std::move(tmp),
            op_select.vars,
            set_to_vector(safe_assigned_vars)
        );
        return;
    } else {
        this->handle_select(op_select);
    }
}


void BindingIterConstructor::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);

    // TODO: no need to save all vars, detect only the ones required
    std::set<VarId> group_saved_vars = get_query_ctx().get_all_vars();

    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> group_expressions;

    for (size_t i = 0; i < op_group_by.items.size(); i ++) {
        auto& [op_var, op_expr] = op_group_by.items[i];

        VarId var(0);

        if (op_var) {
            var = *op_var;
        } else {
            var = get_query_ctx().get_internal_var();
        }

        if (op_expr) {
            ExprToBindingExpr expr_to_binding_expr(
                safe_assigned_vars,
                possible_assigned_vars
            );
            op_expr->accept_visitor(expr_to_binding_expr);
            group_expressions.push_back({var, std::move(expr_to_binding_expr.current_binding_expr)});
        }

        group_vars.push_back(var);
        group_saved_vars.insert(var);
    }

    std::vector<bool> ascending(group_vars.size(), true);

    if (group_expressions.size() > 0) {
        tmp = std::make_unique<GroupExpressions>(std::move(tmp), std::move(group_expressions));
    }

    auto group_vars_copy = group_vars;

    tmp = std::make_unique<OrderById>(
        std::move(tmp),
        std::move(group_saved_vars),
        std::move(group_vars_copy),
        std::move(ascending),
        &SPARQL::Comparisons::compare
    );
}


void BindingIterConstructor::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);

    std::vector<std::unique_ptr<BindingExpr>> having_exprs = std::vector<std::unique_ptr<BindingExpr>>();
    having_exprs.reserve(op_having.exprs.size());

    std::set<VarId> group_vars_set;
    for (auto& var_id : group_vars) {
        group_vars_set.insert(var_id);
    }

    std::set<VarId> empty;
    for (auto& expr: op_having.exprs) {
        VarId var = get_query_ctx().get_internal_var();

        ExprToBindingExpr expr_to_binding_expr(
            empty,          // safe_assigned_vars
            group_vars_set, // possible_assigned_vars
            group_vars_set, // group_vars
            aggregations,
            projection_order_exprs,
            var,
            GroupingMode::Grouping);

        expr->accept_visitor(expr_to_binding_expr);

        group_vars.push_back(var);

        having_exprs.push_back(std::move(expr_to_binding_expr.current_binding_expr));
    }

    if (having_exprs.size() > 0) {
        tmp = std::make_unique<Filter>(std::move(tmp), std::move(having_exprs), /* is_having_filter */  true);
    }
}


void BindingIterConstructor::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);

    assert(op_order_by.items.size() > 0);

    auto grouping_mode = (group_vars.size() > 0 || aggregations.size() > 0)
                       ? GroupingMode::Grouping
                       : GroupingMode::OptionalGrouping;
    std::set<VarId> group_vars_set;
    for (auto& var_id : group_vars) {
        group_vars_set.insert(var_id);
    }

    for (auto& item : op_order_by.items) {
        if (std::holds_alternative<VarId>(item)) {
            auto var = std::get<VarId>(item);
            order_vars.push_back(var);
            order_saved_vars.insert(var);
        } else {
            auto& expr = std::get<std::unique_ptr<Expr>>(item);
            auto var = get_query_ctx().get_internal_var();

            order_vars.push_back(var);
            order_saved_vars.insert(var);

            ExprToBindingExpr expr_to_binding_expr(
                safe_assigned_vars,
                possible_assigned_vars,
                group_vars_set,
                aggregations,
                projection_order_exprs,
                {},
                grouping_mode);

            expr->accept_visitor(expr_to_binding_expr);

            if (aggregations.size() > 0) {
                if (grouping_mode == GroupingMode::OptionalGrouping) {
                    grouping_mode = GroupingMode::Grouping;
                } else if (grouping_mode == GroupingMode::NoGrouping) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            } else {
                if (grouping_mode == GroupingMode::OptionalGrouping) {
                    grouping_mode = GroupingMode::NoGrouping;
                } else if (grouping_mode == GroupingMode::Grouping) {
                    throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
                }
            }

            projection_order_exprs.emplace_back(
                var,
                std::move(expr_to_binding_expr.current_binding_expr),
                grouping_mode == GroupingMode::Grouping);
        }
    }

    order_ascending = std::move(op_order_by.ascending_order);
}


void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    std::vector<std::unique_ptr<Plan>> base_plans;

    for (auto& op_triple : op_basic_graph_pattern.triples) {
        base_plans.push_back(std::make_unique<TriplePlan>(
            op_triple.subject,
            op_triple.predicate,
            op_triple.object
        ));
    }
    for (auto& op_path : op_basic_graph_pattern.paths) {
        auto path = op_path.path.get();
        base_plans.push_back(std::make_unique<SPARQL::PathPlan>(
            begin_at_left,
            op_path.var,
            op_path.subject,
            *path,
            op_path.object,
            op_path.semantic
        ));
    }

    assert(tmp == nullptr);

    const auto binding_size = get_query_ctx().get_var_size();

    // Set input vars
    for (auto& plan : base_plans) {
        plan->set_input_vars(safe_assigned_vars);
    }

    // try to use leapfrog if there is a join
    if (base_plans.size() > 1) {
        tmp = LeapfrogOptimizer::try_get_iter(base_plans, binding_size);
    }

    if (tmp == nullptr) {
        std::unique_ptr<Plan> root_plan = nullptr;
        root_plan = GreedyOptimizer::get_plan(base_plans);
        // std::cout << "\nPlan Generated:\n";
        // root_plan->print(std::cout, true);
        // std::cout << "\nestimated cost: " << root_plan->estimate_cost() << "\n";
        tmp = root_plan->get_binding_iter();
    }

    // Insert new assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            possible_assigned_vars.insert(var);
            safe_assigned_vars.insert(var);
        }
    }
}


void BindingIterConstructor::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);

    std::vector<std::unique_ptr<BindingExpr>> binding_exprs;

    ExprToBindingExpr expr_to_binding_expr(
        safe_assigned_vars,
        possible_assigned_vars
    );
    for (auto& expr : op_filter.filters) {
        expr->accept_visitor(expr_to_binding_expr);
        binding_exprs.push_back(std::move(expr_to_binding_expr.current_binding_expr));
    }

    tmp = std::make_unique<Filter>(std::move(tmp), std::move(binding_exprs), /* is_having_filter */ false);
}


void BindingIterConstructor::visit(OpUnion& op_union) {
    std::vector<std::unique_ptr<BindingIter>> union_iters;
    auto original_possible_assigned_vars = possible_assigned_vars;
    auto original_safe_assigned_vars     = safe_assigned_vars;

    auto acc_possible_assigned_vars = possible_assigned_vars;
    auto acc_safe_assigned_vars = std::set<VarId>();

    auto first = true;
    for (auto& op: op_union.unions) {
        if (first) {
            acc_safe_assigned_vars = op->get_safe_vars();
            first = false;
        }
        possible_assigned_vars = original_possible_assigned_vars;
        safe_assigned_vars     = set_intersection(original_safe_assigned_vars, op->get_fixable_vars());

        op->accept_visitor(*this);
        union_iters.push_back(std::move(tmp));

        acc_possible_assigned_vars = set_union(acc_possible_assigned_vars, possible_assigned_vars);
        acc_safe_assigned_vars     = set_intersection(acc_safe_assigned_vars, safe_assigned_vars);
    }
    possible_assigned_vars = std::move(acc_possible_assigned_vars);
    safe_assigned_vars     = set_union(original_safe_assigned_vars, acc_safe_assigned_vars);
    tmp = std::make_unique<Union>(std::move(union_iters));
}


void BindingIterConstructor::visit(OpSequence& op_sequence) {
    auto original_safe_assigned_vars = safe_assigned_vars;

    auto& op0 = op_sequence.ops[0];
    safe_assigned_vars = set_intersection(safe_assigned_vars, op0->get_fixable_vars());
    op0->accept_visitor(*this);
    safe_assigned_vars = set_union(original_safe_assigned_vars, safe_assigned_vars);

    auto old_tmp = std::move(tmp);

    auto acc_safe_assigned_vars = safe_assigned_vars;
    auto acc_scope_vars = op0->get_scope_vars();

    for (size_t i = 1; i < op_sequence.ops.size(); i++) {
        auto& op = op_sequence.ops[i];

        auto op_scope_vars = op->get_scope_vars();

        auto join_vars = set_intersection(acc_scope_vars, op_scope_vars);
        auto fixable_vars = set_intersection(acc_safe_assigned_vars, op->get_fixable_vars());
        auto unsafe_join_vars = set_difference(join_vars, fixable_vars);

        safe_assigned_vars = fixable_vars;
        op->accept_visitor(*this);

        if (unsafe_join_vars.size() == 0) {
            old_tmp = std::make_unique<IndexNestedLoopJoin>(std::move(old_tmp), std::move(tmp));
        } else {
            auto lhs_only_vars = set_difference(acc_scope_vars, join_vars);
            auto rhs_only_vars = set_difference(op_scope_vars, join_vars);
            old_tmp = std::make_unique<NestedLoopJoin>(
                std::move(old_tmp),
                std::move(tmp),
                set_to_vector(fixable_vars),
                set_to_vector(unsafe_join_vars),
                set_to_vector(original_safe_assigned_vars),
                set_to_vector(lhs_only_vars),
                set_to_vector(rhs_only_vars));
        }

        acc_safe_assigned_vars = set_union(acc_safe_assigned_vars, safe_assigned_vars);
        acc_scope_vars = set_union(acc_scope_vars, op_scope_vars);
    }

    safe_assigned_vars = std::move(acc_safe_assigned_vars);
    tmp = std::move(old_tmp);
}


JoinVars BindingIterConstructor::calculate_join_vars(Op& lhs, Op& rhs) {
    JoinVars vars;
    vars.parent_safe_vars = safe_assigned_vars;

    auto lhs_scope_vars = lhs.get_scope_vars();
    auto rhs_scope_vars = rhs.get_scope_vars();
    auto lhs_safe_vars  = lhs.get_safe_vars();

    vars.after_left_safe_vars = set_union(vars.parent_safe_vars, lhs_safe_vars);

    vars.common_vars = set_intersection(lhs_scope_vars, rhs_scope_vars);
    vars.join_vars = set_difference(vars.common_vars, vars.parent_safe_vars);
    vars.lhs_only_vars = set_difference(set_difference(lhs_scope_vars, vars.join_vars), vars.parent_safe_vars);
    vars.rhs_only_vars = set_difference(set_difference(rhs_scope_vars, vars.join_vars), vars.parent_safe_vars);

    vars.lhs_fixable_vars = set_intersection(vars.parent_safe_vars, lhs.get_fixable_vars());
    vars.rhs_fixable_vars = set_intersection(vars.after_left_safe_vars, rhs.get_fixable_vars());

    vars.safe_join_vars = set_intersection(set_intersection(vars.join_vars, lhs_safe_vars), vars.rhs_fixable_vars);
    vars.unsafe_join_vars = set_difference(vars.join_vars, vars.safe_join_vars);

    return vars;
}


void BindingIterConstructor::visit(OpOptional& op_optional) {
    auto vars = calculate_join_vars(*op_optional.lhs, *op_optional.rhs);

    safe_assigned_vars = std::move(vars.lhs_fixable_vars);
    op_optional.lhs->accept_visitor(*this);

    auto lhs_iter = std::move(tmp);

    safe_assigned_vars = std::move(vars.rhs_fixable_vars);
    op_optional.rhs->accept_visitor(*this);

    if (vars.unsafe_join_vars.size() == 0) {
        tmp = std::make_unique<IndexLeftOuterJoin>(
            std::move(lhs_iter),
            std::move(tmp),
            set_to_vector(vars.rhs_only_vars)
        );
    } else {
        tmp = std::make_unique<NestedLoopLeftJoin>(
            std::move(lhs_iter),
            std::move(tmp),
            set_to_vector(vars.safe_join_vars),
            set_to_vector(vars.unsafe_join_vars),
            set_to_vector(vars.parent_safe_vars),
            set_to_vector(vars.lhs_only_vars),
            set_to_vector(vars.rhs_only_vars)
        );
    }

    safe_assigned_vars = std::move(vars.after_left_safe_vars);
}


void BindingIterConstructor::visit(OpMinus& op_minus) {
    auto vars = calculate_join_vars(*op_minus.lhs, *op_minus.rhs);

    safe_assigned_vars = std::move(vars.lhs_fixable_vars);
    op_minus.lhs->accept_visitor(*this);

    auto lhs_iter = std::move(tmp);

    safe_assigned_vars = std::move(vars.rhs_fixable_vars);
    op_minus.rhs->accept_visitor(*this);

    if (vars.common_vars.size() == 0) {
        tmp = std::move(lhs_iter);
    } else if (vars.join_vars.size() == 0) {
        tmp = std::make_unique<NoFreeVariableMinus>(std::move(lhs_iter), std::move(tmp));
    } else {
        tmp = std::make_unique<NestedLoopAntiJoin<false>>(
            std::move(lhs_iter),
            std::move(tmp),
            set_to_vector(vars.safe_join_vars),
            set_to_vector(vars.unsafe_join_vars),
            set_to_vector(vars.parent_safe_vars),
            set_to_vector(vars.lhs_only_vars),
            set_to_vector(vars.rhs_only_vars)
        );
    }

    safe_assigned_vars     = std::move(vars.after_left_safe_vars);
    possible_assigned_vars = std::move(vars.after_left_possible_assigned_vars);
}


// This is the same as OpMinus, the only difference is the bool template parameter
void BindingIterConstructor::visit(OpNotExists& op_not_exists) {
    auto vars = calculate_join_vars(*op_not_exists.lhs, *op_not_exists.rhs);

    safe_assigned_vars = std::move(vars.lhs_fixable_vars);
    op_not_exists.lhs->accept_visitor(*this);

    auto lhs_iter = std::move(tmp);

    safe_assigned_vars = std::move(vars.rhs_fixable_vars);
    op_not_exists.rhs->accept_visitor(*this);

    if (vars.common_vars.size() == 0) {
        tmp = std::move(lhs_iter);
    } else if (vars.join_vars.size() == 0) {
        tmp = std::make_unique<NoFreeVariableMinus>(std::move(lhs_iter), std::move(tmp));
    } else {
        tmp = std::make_unique<NestedLoopAntiJoin<true>>(
            std::move(lhs_iter),
            std::move(tmp),
            set_to_vector(vars.safe_join_vars),
            set_to_vector(vars.unsafe_join_vars),
            set_to_vector(vars.parent_safe_vars),
            set_to_vector(vars.lhs_only_vars),
            set_to_vector(vars.rhs_only_vars)
        );
    }

    safe_assigned_vars     = std::move(vars.after_left_safe_vars);
    possible_assigned_vars = std::move(vars.after_left_possible_assigned_vars);
}


void BindingIterConstructor::visit(OpSemiJoin& op_semi_join) {
    auto vars = calculate_join_vars(*op_semi_join.lhs, *op_semi_join.rhs);

    safe_assigned_vars = std::move(vars.lhs_fixable_vars);
    op_semi_join.lhs->accept_visitor(*this);

    auto lhs_iter = std::move(tmp);

    safe_assigned_vars = std::move(vars.rhs_fixable_vars);
    op_semi_join.rhs->accept_visitor(*this);

    tmp = std::make_unique<NestedLoopSemiJoin>(
        std::move(lhs_iter),
        std::move(tmp),
        set_to_vector(vars.safe_join_vars),
        set_to_vector(vars.unsafe_join_vars),
        set_to_vector(vars.parent_safe_vars),
        set_to_vector(vars.lhs_only_vars),
        set_to_vector(vars.rhs_only_vars)
    );

    safe_assigned_vars     = std::move(vars.after_left_safe_vars);
    possible_assigned_vars = std::move(vars.after_left_possible_assigned_vars);
}


void BindingIterConstructor::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);

    ExprToBindingExpr expr_to_binding_expr(
        safe_assigned_vars,
        possible_assigned_vars
    );
    op_bind.expr->accept_visitor(expr_to_binding_expr);
    auto expr = std::move(expr_to_binding_expr.current_binding_expr);

    possible_assigned_vars.insert(op_bind.var);

    tmp = std::make_unique<Bind>(std::move(tmp), std::move(expr), op_bind.var);
}


void BindingIterConstructor::visit(OpUnitTable&) {
    tmp = std::make_unique<SingleResultBindingIter>();
}


void BindingIterConstructor::visit(OpEmpty&) {
    tmp = std::make_unique<EmptyBindingIter>();
}


void BindingIterConstructor::visit(OpService& op_service) {
    auto scope_vars = op_service.get_scope_vars();
    auto safe_vars = op_service.get_safe_vars();

    auto join_vars = scope_vars;
    if (std::holds_alternative<VarId>(op_service.var_or_iri)) {
        VarId var_id = std::get<VarId>(op_service.var_or_iri);
        join_vars.insert(var_id);
    }
    join_vars = set_union(possible_assigned_vars, join_vars);

    auto fixed_vars = set_intersection(safe_assigned_vars, op_service.get_fixable_vars());
    auto fixed_join_vars = set_intersection(join_vars, fixed_vars);

    tmp = std::make_unique<SparqlService>(
        op_service.silent,
        std::move(op_service.query),
        std::move(op_service.prefixes),
        std::move(op_service.var_or_iri),
        std::move(scope_vars),
        std::move(fixed_vars),
        std::move(fixed_join_vars)
    );

    safe_assigned_vars = set_union(safe_assigned_vars, safe_vars);
    possible_assigned_vars = set_union(possible_assigned_vars, scope_vars);
}


void BindingIterConstructor::visit(OpValues& op_values) {
    std::vector<ObjectId> values;
    values.reserve(op_values.values.size());

    for (auto& value : op_values.values) {
        values.push_back(value);
    }

    auto fixable_values = op_values.get_fixable_vars();

    std::vector<std::pair<VarId, bool>> vars;
    vars.reserve(op_values.vars.size());

    for (size_t idx = 0; idx < op_values.vars.size(); idx++) {
        auto& var = op_values.vars[idx];
        possible_assigned_vars.insert(var);

        vars.push_back({var, safe_assigned_vars.find(var) != safe_assigned_vars.end()
                              && fixable_values.find(var) != fixable_values.end()});

        if (!op_values.has_undef[idx]) {
            safe_assigned_vars.insert(var);
        }
    }

    tmp = std::make_unique<Values>(std::move(vars), std::move(values));
}
