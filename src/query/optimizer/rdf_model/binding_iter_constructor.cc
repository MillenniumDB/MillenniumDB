#include "binding_iter_constructor.h"

#include <cassert>
#include <cstdint>
#include <sys/types.h>

#include "graph_models/rdf_model/comparisons.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/set_operations.h"
#include "query/exceptions.h"
#include "query/executor/binding_iters.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_optimizer.h"
#include "query/optimizer/rdf_model/expr_to_binding_expr.h"
#include "query/optimizer/rdf_model/plan/path_plan.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;
using namespace misc;

BindingIterConstructor::BindingIterConstructor()
{
    // all path variables must exist in the query context at this time
    begin_at_left.resize(get_query_ctx().get_var_size());
}

std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>
get_non_redundant_exprs(std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>>&& exprs) {
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

bool BindingIterConstructor::is_aggregation_or_group_var(VarId var) const
{
    if (aggregations.find(var) != aggregations.end()) {
        return true;
    }
    if (grouping) {
        for (const auto& [expr_var, expr] : projection_order_exprs) {
            if (expr_var == var) {
                return true;
            }
        }
    }
    if (group_vars.find(var) != group_vars.end()) {
        return true;
    }

    return false;
}

void BindingIterConstructor::make_solution_modifier_operators(bool is_root_query, bool distinct, uint64_t offset, uint64_t limit) {
    // process order expressions if we have them
    // ORDER BY expressions must come after SELECT expressions.
    std::vector<VarId> order_vars;
    if (op_order_by) {
        for (auto& item : op_order_by->items) {
            if (std::holds_alternative<VarId>(item)) {
                auto var = std::get<VarId>(item);
                order_vars.push_back(var);
                order_saved_vars.insert(var);
                group_saved_vars.insert(var);
            } else {
                auto& expr = std::get<std::unique_ptr<Expr>>(item);
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
    }

    std::vector<std::unique_ptr<BindingExpr>> having_exprs;
    if (op_having) {
        having_exprs.reserve(op_having->exprs.size());

        // need to do this for EXIST/NOT EXISTS expressions inside HAVING
        safe_assigned_vars.clear();

        for (auto& expr: op_having->exprs) {
            ExprToBindingExpr expr_to_binding_expr(this, {});
            expr->accept_visitor(expr_to_binding_expr);
            having_exprs.push_back(std::move(expr_to_binding_expr.tmp));
        }
    }

    bool need_order_agg = false;
    for (auto&& [var, agg] : aggregations) {
        if (!agg->is_pipelineble())
            need_order_agg = true;
    }

    if (op_group_by) {
        std::vector<bool> ascending(group_vars.size(), true); // TODO: avoid?

        auto non_redundant_expr_eval = get_non_redundant_exprs(std::move(group_exprs));
        if (non_redundant_expr_eval.size() > 0) {
            tmp = std::make_unique<ExprEvaluator>(
                std::move(tmp),
                std::move(non_redundant_expr_eval)
            );
        }

        std::vector<VarId> group_vars_vector;
        for (auto& var : group_vars) {
            group_vars_vector.push_back(var);
        }

        if (need_order_agg) {
            // TODO: use special order for group by?
            tmp = std::make_unique<OrderBy>(
                std::move(tmp),
                std::move(group_saved_vars),
                std::move(group_vars_vector),
                std::move(ascending),
                &OrderBy::internal_compare
            );
        }
        op_group_by = nullptr;
    }

    // Create the Aggregation if necessary.
    if (aggregations.size() > 0 || group_vars.size() > 0) {
        if (need_order_agg) {
            tmp = std::make_unique<Aggregation>(
                std::move(tmp),
                std::move(aggregations),
                std::move(group_vars)
            );
        } else {
            tmp = std::make_unique<HybridAggregation>(
                std::move(tmp),
                std::move(aggregations),
                std::move(group_vars),
                std::move(group_saved_vars)
            );
        }
    }

    if (having_exprs.size() > 0) {
        tmp = std::make_unique<Filter>(
            &SPARQL::Conversions::to_boolean,
            std::move(tmp),
            std::move(having_exprs)
        );
        op_having = nullptr; // important for subqueries
    }

    assert(tmp != nullptr);
    auto non_redundant_expr_eval = get_non_redundant_exprs(std::move(projection_order_exprs));
    if (non_redundant_expr_eval.size() > 0) {
        tmp = std::make_unique<ExprEvaluator>(
            std::move(tmp),
            std::move(non_redundant_expr_eval)
        );
    }

    const bool has_limit = limit != Op::DEFAULT_LIMIT || rdf_model.MAX_LIMIT != Op::DEFAULT_LIMIT;
    const bool has_offset = offset != Op::DEFAULT_OFFSET;

    // If there is an op_order_by with a limit (and no offset), we can use OrderByWithLimit
    // This will keep a priority queue with max_size=limit so it is not necessary to sort the results afterwards.
    if (op_order_by && has_limit && !has_offset) {
        // OrderBy with priority queue
        if (distinct) {
            auto projection_vars_copy = projection_vars;
            tmp = std::make_unique<DistinctHash>(std::move(tmp), std::move(projection_vars_copy));
        }

        if (limit > rdf_model.MAX_LIMIT && is_root_query) {
            limit = rdf_model.MAX_LIMIT;
        }

        for (const auto& var : projection_vars) {
            order_saved_vars.insert(var);
        }

        tmp = std::make_unique<OrderByWithLimit>(
            std::move(tmp),
            std::move(order_saved_vars),
            std::move(order_vars),
            std::move(op_order_by->ascending_order),
            &SPARQL::Comparisons::compare,
            limit
        );
        op_order_by = nullptr; // important for subqueries
        return;
    }

    if (op_order_by) {
        for (auto& var : projection_vars) {
            order_saved_vars.insert(var);
        }

        tmp = std::make_unique<OrderBy>(
            std::move(tmp),
            std::move(order_saved_vars),
            std::move(order_vars),
            std::move(op_order_by->ascending_order),
            &SPARQL::Comparisons::compare
        );
        op_order_by = nullptr; // important for subqueries
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

    if (has_offset || has_limit) {
        if (limit > rdf_model.MAX_LIMIT && is_root_query) {
            limit = rdf_model.MAX_LIMIT;
        }
        tmp = std::make_unique<Slice>(std::move(tmp), offset, limit);
    }
}


void BindingIterConstructor::visit(OpDescribe& op_describe) {
    op_describe.op->accept_visitor(*this);

     if (group_vars.size() > 0 || aggregations.size() > 0) {
        for (auto& var : op_describe.vars) {;
            if (!is_aggregation_or_group_var(var)) {
                throw QuerySemanticException("Mixing aggregations and non-aggregations is not allowed");
            }
        }
    }

    // Describe *
    if (op_describe.vars.size() == 0 && op_describe.iris.size() == 0) {
        for (auto var : op_describe.get_all_vars()) {
            op_describe.vars.push_back(var);
        }
    }

    // This is to make sure that the describe_vars are saved
    // when building the solution modifier operators.
    for (auto var : op_describe.vars) {
        order_saved_vars.insert(var);
        group_saved_vars.insert(var);
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
            group_saved_vars.insert(var);
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
    for (auto& expr : op_select.vars_exprs) {
        if (expr != nullptr && expr->has_aggregation()) {
            grouping = true;
            break;
        }
    }

    op_select.op->accept_visitor(*this);

    for (size_t i = 0; i < op_select.vars.size(); i++) {
        auto var   = op_select.vars[i];
        auto& expr = op_select.vars_exprs[i];
        projection_vars.push_back(var);

        if (expr) {
            ExprToBindingExpr expr_to_binding_expr(this, {var});
            expr->accept_visitor(expr_to_binding_expr);

            projection_order_exprs.emplace_back(
                var,
                std::move(expr_to_binding_expr.tmp)
            );
        } else {
            if (grouping) {
                if (!is_aggregation_or_group_var(var)) {
                    throw QuerySemanticException(
                        "Invalid use of var \""
                        + get_query_ctx().get_var_name(var)
                        + "\" in SELECT, cannot mix agg expressions with non-agg expressions");
                }
            }
            group_saved_vars.insert(var);
        }
    }

    make_solution_modifier_operators(!op_select.is_sub_select, // is_root_query
                                     op_select.distinct,
                                     op_select.offset,
                                     op_select.limit);
}

void BindingIterConstructor::visit(OpSelect& op_select)
{
    if (op_select.is_sub_select) {
        auto old_grouping = grouping;
        grouping = false;
        handle_select(op_select);
        grouping = old_grouping;

        tmp = std::make_unique<SubSelect>(std::move(tmp), op_select.vars, set_to_vector(safe_assigned_vars));
        return;
    } else {
        handle_select(op_select);
    }
}

void BindingIterConstructor::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    grouping = true;
    this->op_group_by = &op_group_by;

    for (auto&& [expr, alias] : op_group_by.items) {
        ExprVar* casted = dynamic_cast<ExprVar*>(expr.get());
        if (casted != nullptr && !alias.has_value()) {
            if (alias) {
                ExprToBindingExpr expr_to_binding_expr;
                expr->accept_visitor(expr_to_binding_expr);

                group_vars.insert(*alias);
                group_saved_vars.insert(*alias);
                group_exprs.push_back({ *alias, std::move(expr_to_binding_expr.tmp) });
            } else {
                VarId var = casted->var;
                group_vars.insert(var);
                group_saved_vars.insert(var);
            }
        } else {
            ExprToBindingExpr expr_to_binding_expr;
            expr->accept_visitor(expr_to_binding_expr);

            if (alias) {
                group_vars.insert(*alias);
                group_saved_vars.insert(*alias);
                group_exprs.push_back({ *alias, std::move(expr_to_binding_expr.tmp) });
            } else {
                VarId var = get_query_ctx().get_internal_var();
                group_vars.insert(var);
                group_saved_vars.insert(var);
                group_exprs.push_back({ var, std::move(expr_to_binding_expr.tmp) });
            }
        }
    }
}

void BindingIterConstructor::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);
    this->op_having = &op_having;
}

void BindingIterConstructor::visit(OpOrderBy& op_order_by)
{
    for (const auto& item : op_order_by.items) {
        if (std::holds_alternative<std::unique_ptr<Expr>>(item)) {
            auto& expr = std::get<std::unique_ptr<Expr>>(item);
            if (expr->has_aggregation()) {
                grouping = true;
            }
        }
    }
    op_order_by.op->accept_visitor(*this);

    assert(op_order_by.items.size() > 0);

    this->op_order_by = &op_order_by;
}

void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
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
        base_plans.push_back(std::make_unique<PathPlan>(
            begin_at_left,
            op_path.var,
            op_path.subject,
            *path,
            op_path.object,
            op_path.semantic
        ));
    }

    assert(tmp == nullptr);

    const auto build_bgp_iter = [&]() {
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
            root_plan = GreedyOptimizer::get_plan(base_plans);
            tmp = root_plan->get_binding_iter();
        }
    };

    if (base_plans.size() > 0) {
        build_bgp_iter();
    }

    // Insert new assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            safe_assigned_vars.insert(var);
        }
    }
}

void BindingIterConstructor::visit(OpProcedure& op_procedure)
{
    std::vector<std::unique_ptr<BindingExpr>> argument_binding_exprs;

    ExprToBindingExpr expr_to_binding_expr;
    for (std::size_t i = 0; i < op_procedure.argument_exprs.size(); ++i) {
        op_procedure.argument_exprs[i]->accept_visitor(expr_to_binding_expr);
        argument_binding_exprs.emplace_back(std::move(expr_to_binding_expr.tmp));
    }

    switch (op_procedure.procedure_type) {
    case OpProcedure::ProcedureType::HNSW_TOP_K:
        tmp = std::make_unique<Procedure::HNSWTopK>(
            std::move(argument_binding_exprs),
            std::move(op_procedure.binding_vars),
            rdf_model.catalog.hnsw_index_manager,
            &SPARQL::Conversions::unpack_string,
            &SPARQL::Conversions::unpack_int
        );
        break;
    case OpProcedure::ProcedureType::HNSW_SCAN:
        tmp = std::make_unique<Procedure::HNSWScan>(
            std::move(argument_binding_exprs),
            std::move(op_procedure.binding_vars),
            rdf_model.catalog.hnsw_index_manager,
            &SPARQL::Conversions::unpack_string,
            &SPARQL::Conversions::unpack_int
        );
        break;
    case OpProcedure::ProcedureType::TEXT_SEARCH:
        tmp = std::make_unique<Procedure::TextSearchMultiScan>(
            std::move(argument_binding_exprs),
            std::move(op_procedure.binding_vars),
            rdf_model.catalog.text_index_manager,
            &SPARQL::Conversions::unpack_string
        );
        break;
    default:
        throw NotSupportedException(
            "BindingIterConstructor::visit(OpProcedure&): Not implemented procedure with procedure_type: "
            + std::to_string(static_cast<uint8_t>(op_procedure.procedure_type))
        );
    }

    for (const auto& yield_var : op_procedure.binding_vars) {
        safe_assigned_vars.insert(yield_var);
    }
}

void BindingIterConstructor::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    std::vector<std::unique_ptr<BindingExpr>> binding_exprs;

    ExprToBindingExpr expr_to_binding_expr;
    for (auto& expr : op_filter.filters) {
        expr->accept_visitor(expr_to_binding_expr);
        binding_exprs.push_back(std::move(expr_to_binding_expr.tmp));
    }

    tmp = std::make_unique<Filter>(
        &SPARQL::Conversions::to_boolean,
        std::move(tmp),
        std::move(binding_exprs)
    );
}

void BindingIterConstructor::visit(OpUnion& op_union)
{
    std::vector<std::unique_ptr<BindingIter>> union_iters;
    auto original_safe_assigned_vars = safe_assigned_vars;

    auto acc_safe_assigned_vars = std::set<VarId>();

    auto first = true;
    for (auto& op : op_union.unions) {
        if (first) {
            acc_safe_assigned_vars = op->get_safe_vars();
            first = false;
        }
        safe_assigned_vars = set_intersection(original_safe_assigned_vars, op->get_fixable_vars());

        op->accept_visitor(*this);
        union_iters.push_back(std::move(tmp));

        acc_safe_assigned_vars = set_intersection(acc_safe_assigned_vars, safe_assigned_vars);
    }
    safe_assigned_vars = set_union(original_safe_assigned_vars, acc_safe_assigned_vars);
    tmp = std::make_unique<Union>(std::move(union_iters));
}

void BindingIterConstructor::visit(OpSequence& op_sequence)
{
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

JoinVars BindingIterConstructor::calculate_join_vars(Op& lhs, Op& rhs)
{
    JoinVars vars;
    vars.parent_safe_vars = safe_assigned_vars;

    auto lhs_scope_vars = lhs.get_scope_vars();
    auto rhs_scope_vars = rhs.get_scope_vars();
    auto lhs_safe_vars = lhs.get_safe_vars();

    vars.after_left_safe_vars = set_union(vars.parent_safe_vars, lhs_safe_vars);

    vars.common_vars = set_intersection(lhs_scope_vars, rhs_scope_vars);
    vars.join_vars = set_difference(vars.common_vars, vars.parent_safe_vars);
    vars.lhs_only_vars = set_difference(
        set_difference(lhs_scope_vars, vars.join_vars),
        vars.parent_safe_vars
    );
    vars.rhs_only_vars = set_difference(
        set_difference(rhs_scope_vars, vars.join_vars),
        vars.parent_safe_vars
    );

    vars.lhs_fixable_vars = set_intersection(vars.parent_safe_vars, lhs.get_fixable_vars());
    vars.rhs_fixable_vars = set_intersection(vars.after_left_safe_vars, rhs.get_fixable_vars());

    vars.safe_join_vars = set_intersection(
        set_intersection(vars.join_vars, lhs_safe_vars),
        vars.rhs_fixable_vars
    );
    vars.unsafe_join_vars = set_difference(vars.join_vars, vars.safe_join_vars);

    return vars;
}

void BindingIterConstructor::visit(OpOptional& op_optional)
{
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

void BindingIterConstructor::visit(OpMinus& op_minus)
{
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
        tmp = std::make_unique<Minus>(
            std::move(lhs_iter),
            std::move(tmp),
            set_to_vector(vars.safe_join_vars),
            set_to_vector(vars.unsafe_join_vars),
            set_to_vector(vars.parent_safe_vars)
        );
    }

    safe_assigned_vars = std::move(vars.after_left_safe_vars);
}

void BindingIterConstructor::visit(OpNotExists& op_not_exists)
{
    auto vars = calculate_join_vars(*op_not_exists.lhs, *op_not_exists.rhs);

    safe_assigned_vars = std::move(vars.lhs_fixable_vars);
    op_not_exists.lhs->accept_visitor(*this);

    auto lhs_iter = std::move(tmp);

    safe_assigned_vars = std::move(vars.rhs_fixable_vars);
    op_not_exists.rhs->accept_visitor(*this);

    tmp = std::make_unique<NotExists>(
        std::move(lhs_iter),
        std::move(tmp),
        set_to_vector(vars.unsafe_join_vars)
    );

    safe_assigned_vars = std::move(vars.after_left_safe_vars);
}

void BindingIterConstructor::visit(OpSemiJoin& op_semi_join)
{
    auto vars = calculate_join_vars(*op_semi_join.lhs, *op_semi_join.rhs);

    safe_assigned_vars = std::move(vars.lhs_fixable_vars);
    op_semi_join.lhs->accept_visitor(*this);

    auto lhs_iter = std::move(tmp);

    safe_assigned_vars = std::move(vars.rhs_fixable_vars);
    op_semi_join.rhs->accept_visitor(*this);

    tmp = std::make_unique<NestedLoopSemiJoin>(
        std::move(lhs_iter),
        std::move(tmp),
        set_to_vector(vars.unsafe_join_vars)
    );

    safe_assigned_vars = std::move(vars.after_left_safe_vars);
}

void BindingIterConstructor::visit(OpBind& op_bind)
{
    op_bind.op->accept_visitor(*this);

    ExprToBindingExpr expr_to_binding_expr;
    op_bind.expr->accept_visitor(expr_to_binding_expr);
    auto expr = std::move(expr_to_binding_expr.tmp);

    tmp = std::make_unique<Bind>(std::move(tmp), std::move(expr), op_bind.var);
}

void BindingIterConstructor::visit(OpUnitTable&)
{
    tmp = std::make_unique<SingleResultBindingIter>();
}

void BindingIterConstructor::visit(OpEmpty&)
{
    tmp = std::make_unique<EmptyBindingIter>();
}

void BindingIterConstructor::visit(OpService& op_service)
{
    auto scope_vars = op_service.get_scope_vars();
    auto safe_vars = op_service.get_safe_vars();

    auto join_vars = scope_vars; // maybe join_vars is a bad name for this case
    if (std::holds_alternative<VarId>(op_service.var_or_iri)) {
        VarId var_id = std::get<VarId>(op_service.var_or_iri);
        join_vars.insert(var_id);
    }

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
}

void BindingIterConstructor::visit(OpValues& op_values)
{
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

        vars.push_back({ var,
                         safe_assigned_vars.find(var) != safe_assigned_vars.end()
                             && fixable_values.find(var) != fixable_values.end() });

        if (!op_values.has_undef[idx]) {
            safe_assigned_vars.insert(var);
        }
    }

    tmp = std::make_unique<Values>(std::move(vars), std::move(values));
}
