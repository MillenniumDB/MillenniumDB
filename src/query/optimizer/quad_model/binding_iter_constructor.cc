#include "binding_iter_constructor.h"

#include <cassert>
#include <memory>

#include "graph_models/quad_model/comparisons.h"
#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "misc/set_operations.h"
#include "query/executor/binding_iters.h"
#include "storage/index/tensor_store/lsh/binding_iters/project_tensor_similarity.h"
#include "query/executor/binding_iter/aggregation.h"
#include "query/executor/binding_iter/distinct_hash.h"
#include "query/executor/binding_iter/empty_binding_iter.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/hash_join/bgp/in_memory/join_1_var.h"
#include "query/executor/binding_iter/cross_product.h"
#include "query/executor/binding_iter/index_left_outer_join.h"
#include "query/executor/binding_iter/index_nested_loop_join.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/order_by.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/executor/binding_iter/single_result_binding_iter.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_optimizer.h"
#include "query/optimizer/plan/join_order/selinger_optimizer.h"
#include "query/optimizer/quad_model/expr_to_binding_expr.h"
#include "query/optimizer/quad_model/plan/disjoint_object_plan.h"
#include "query/optimizer/quad_model/plan/edge_plan.h"
#include "query/optimizer/quad_model/plan/label_plan.h"
#include "query/optimizer/quad_model/plan/path_plan.h"
#include "query/optimizer/quad_model/plan/property_plan.h"
#include "query/optimizer/quad_model/plan/similarity_search_plan.h"
#include "query/parser/op/mql/op_where.h"
#include "query/parser/op/mql/ops.h"
#include "query/parser/op/op_visitor.h"

using namespace MQL;

constexpr auto MAX_SELINGER_PLANS = 0;

BindingIterConstructor::BindingIterConstructor(
    std::map<VarId, ObjectId>& setted_vars
) :
    setted_vars (setted_vars)
{
    begin_at_left.resize(get_query_ctx().get_var_size());
}

void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    if (op_basic_graph_pattern.similarity_searches.size() > 1) {
        throw NotSupportedException("Multiple similarity searches are not supported yet");
    }

    // Process disjoint Terms
    // if a term is not found we can assume the MATCH result is empty
    for (auto& disjoint_term : op_basic_graph_pattern.disjoint_terms) {
        if ( !term_exists(disjoint_term.term) ) {
            tmp = std::make_unique<EmptyBindingIter>();
            return;
        }
    }

    std::vector<std::unique_ptr<Plan>> base_plans;

    std::vector<std::unique_ptr<SimilaritySearchPlan>> similarity_search_plans;

    // Process disjoint Vars
    for (auto& disjoint_var : op_basic_graph_pattern.disjoint_vars) {
        auto setted_var_found = setted_vars.find(disjoint_var.var);
        if (setted_var_found != setted_vars.end()) {
            if ( !term_exists(setted_var_found->second) ) {
                tmp = std::make_unique<EmptyBindingIter>();
                return;
            }
        } else {
            // we could have something like: MATCH (?x) WHERE ?x.age == 1.
            // ?x is not really an disjoint var
            bool join_with_where_property_equality = false;
            for (auto&& [var, key, value, value_var] : fixed_properties) {
                if (var == disjoint_var.var) {
                    join_with_where_property_equality = true;
                    break;
                }
            }
            if (!join_with_where_property_equality) {
                base_plans.push_back(
                    std::make_unique<DisjointObjectPlan>(disjoint_var.var)
                );
            }
        }
    }

    // Process Labels
    for (auto& op_label : op_basic_graph_pattern.labels) {
        base_plans.push_back(
            std::make_unique<LabelPlan>(
                replace_setted_var(op_label.node),
                replace_setted_var(op_label.label)
            )
        );
    }

    // Process properties (value is fixed)
    for (auto& op_property : op_basic_graph_pattern.properties) {
        base_plans.push_back(
            std::make_unique<PropertyPlan>(
                replace_setted_var(op_property.node),
                op_property.key,
                replace_setted_var(op_property.value)
            )
        );
    }
    // Push equalities from where into the basic graph pattern
    if (!pushed_fixed_properties) {
        pushed_fixed_properties = true;
        for (auto&& [var, key, value, value_var] : fixed_properties) {
            base_plans.push_back(
                std::make_unique<PropertyPlan>(
                    replace_setted_var(var),
                    key,
                    value
                )
            );
            setted_vars.insert({value_var, value});
        }
    }

    // Process connections
    for (auto& op_edge : op_basic_graph_pattern.edges) {
        base_plans.push_back(
            std::make_unique<EdgePlan>(
                replace_setted_var(op_edge.from),
                replace_setted_var(op_edge.to),
                replace_setted_var(op_edge.type),
                replace_setted_var(op_edge.edge)
            )
        );
    }

    // Process property paths
    for (auto& path : op_basic_graph_pattern.paths) {
        base_plans.push_back(
            std::make_unique<PathPlan>(
                begin_at_left,
                path.direction,
                path.var,
                replace_setted_var(path.from),
                replace_setted_var(path.to),
                *path.path,
                path.semantic
            )
        );
    }

    // Process similarity searches
    for (auto& similarity_search : op_basic_graph_pattern.similarity_searches) {
        similarity_search_plans.push_back(
            std::make_unique<SimilaritySearchPlan>(
                similarity_search.object_var,
                similarity_search.similarity_var,
                similarity_search.tensor_store_name,
                similarity_search.k,
                similarity_search.search_k,
                similarity_search.query_tensor,
                similarity_search.query_object
            )
        );
    }

    if (base_plans.size() == 0) {
        if (similarity_search_plans.empty()) {
            tmp = std::make_unique<SingleResultBindingIter>();
        } else {
            tmp = similarity_search_plans[0]->get_binding_iter();
        }
        return;
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
                tmp = LeapfrogOptimizer::try_get_iter_with_assigned(
                    base_plans,
                    get_query_ctx().get_var_size()
                );
            } else {
                tmp = LeapfrogOptimizer::try_get_iter_without_assigned(
                    base_plans,
                    get_query_ctx().get_var_size()
                );
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

    // Set an OrderBy for the current binding iter (tmp) by order_var (ascending)
    // auto set_order_by_var = [&](VarId order_var) {
    //     std::set<VarId>    similarity_saved_vars = get_query_ctx().get_all_vars();
    //     std::vector<VarId> similarity_order_vars = { order_var };
    //     std::vector<bool>  similarity_ascending  = { true };
    //     tmp = std::make_unique<OrderBy>(
    //         std::move(tmp),
    //         std::move(similarity_saved_vars),
    //         std::move(similarity_order_vars),
    //         std::move(similarity_ascending),
    //         &MQL::Comparisons::compare
    //     );
    // };

    if (op_basic_graph_pattern.similarity_searches.size() > 0) {
        // Handle similarity search
        auto sim_iter = similarity_search_plans[0]->get_binding_iter();

        // Check if there is a join between the base plans and the similarity search
        std::set<VarId> base_plan_vars;
        for (auto& plan : base_plans) {
            auto vars = plan->get_vars();
            base_plan_vars.insert(vars.begin(), vars.end());
        }
        auto object_var = similarity_search_plans[0]->get_object_var();

        if (base_plan_vars.find(object_var) != base_plan_vars.end()) {
            // Handle join

            // 1. HashJoin(build=bgp, probe=sim)
            build_bgp_iter();
            std::vector<VarId> build_vars;
            std::vector<VarId> probe_vars = { similarity_search_plans[0]->get_similarity_var() };
            for (auto& var : base_plan_vars) {
                if (var != object_var)
                    build_vars.push_back(var);
            }
            tmp = std::make_unique<HashJoin::BGP::InMemory::Join1Var>(
                std::move(tmp),
                std::move(sim_iter),
                object_var,
                std::move(build_vars),
                std::move(probe_vars)
            );


            // 2. OrderBy(HashJoin(build=sim, probe=bgp), sim.similarity_var)
            /*
            build_bgp_iter();
            std::vector<VarId> build_vars = { similarity_search_plans[0]->get_similarity_var() };
            std::vector<VarId> probe_vars;
            for (auto& var : base_plan_vars) {
                if (var != object_var)
                    probe_vars.push_back(var);
            }
            tmp = std::make_unique<HashJoin::BGP::InMemory::Join1Var>(
                std::move(sim_iter),
                std::move(tmp),
                object_var,
                std::move(build_vars),
                std::move(probe_vars)
            );
            auto similarity_var = similarity_search_plans[0]->get_similarity_var();
            set_order_by_var(similarity_var);
            */


            // 3. IndexNestedLoopJoin(lhs=sim, rhs=bgp)
            /*
            safe_assigned_vars.insert(similarity_search_plans[0]->get_object_var());
            build_bgp_iter();
            tmp = std::make_unique<IndexNestedLoopJoin>(
                std::move(sim_iter),
                std::move(tmp)
            );
            */


            // 4. OrderBy(LeapfrogJoin({ ...base_plans, similarity_search_plan }), similarity)
            /*
            auto similarity_var = similarity_search_plans[0]->get_similarity();
            base_plans.push_back(std::move(similarity_search_plans[0]));
            build_bgp_iter();
            set_order_by_var(similarity_var);
            */
        }
        else {
            // Handle cross product
            build_bgp_iter();
            tmp = std::make_unique<CrossProduct>(
                std::move(tmp),
                std::move(sim_iter)
            );

        }
    }
    else {
        // No similarity search
        build_bgp_iter();
    }

    // insert new safe_assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            safe_assigned_vars.insert(var);
        }
    }
}


void BindingIterConstructor::visit(OpMatch& op_match) {
    fixed_properties = std::move(op_match.fixed_properties);
    op_match.op->accept_visitor(*this);

    for (auto& property : op_match.optional_properties) {
        // ignore if it is in mandatory_properties
        bool skip = false;
        for (auto&& [fp_var, fp_key, fp_value, fp_value_var] : fixed_properties) {
            if (property.node.is_var()
                && property.node.get_var() == fp_var
                && property.key == fp_key)
            {
                skip = true;
                break;
            }
        }
        if (skip) {
            continue;
        }

        std::array<std::unique_ptr<ScanRange>, 3> ranges {
            ScanRange::get(property.node, true),
            ScanRange::get(property.key),
            ScanRange::get(property.value, false)
        };
        auto index_scan = std::make_unique<IndexScan<3>>(
            *quad_model.object_key_value,
            std::move(ranges)
        );
        std::vector<VarId> rhs_only_vars = { property.value.get_var() };
        tmp = std::make_unique<IndexLeftOuterJoin>(
            std::move(tmp),
            std::move(index_scan),
            std::move(rhs_only_vars)
        );
    }

}

void BindingIterConstructor::visit(OpWhere& op_where) {
    ExprToBindingExpr expr_to_binding_expr;
    op_where.expr->accept_visitor(expr_to_binding_expr);

    auto binding_expr = std::move(expr_to_binding_expr.tmp);

    op_where.op->accept_visitor(*this);

    if (binding_expr != nullptr) {
        std::vector<std::unique_ptr<BindingExpr>> exprs;
        exprs.push_back(std::move(binding_expr));
        tmp = std::make_unique<Filter>(
            &MQL::Conversions::to_boolean,
            std::move(tmp),
            std::move(exprs)
        );
    }
}


void BindingIterConstructor::visit(OpOptional& op_optional) {
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


void BindingIterConstructor::visit(OpGroupBy& op_group_by) {
    grouping = true;
    op_group_by.op->accept_visitor(*this);

    for (auto& var : op_group_by.vars) {
        group_vars.insert(var);
        group_saved_vars.insert(var);
        group_vars_vector.push_back(var);
    }
}


void BindingIterConstructor::visit(OpOrderBy& op_order_by) {
    int i = 0;
    for (auto&& [var, expr] : op_order_by.items) {
        if (expr != nullptr) {
            if (expr->has_aggregation()) {
                grouping = true;
            }

            ExprToBindingExpr expr_to_binding_expr(
                this,
                var
            );
            expr->accept_visitor(expr_to_binding_expr);
        }
        order_by_vars.push_back(var);
        order_by_saved_vars.insert(var);
        order_by_ascending.push_back(op_order_by.ascending_order[i]);
        i++;
    }

    for (auto& var : projected_vars) {
        order_by_saved_vars.insert(var);
    }

    op_order_by.op->accept_visitor(*this);
}


void BindingIterConstructor::visit(OpProjectSimilarity& op_project_similarity) {
    op_project_similarity.op->accept_visitor(*this);

    assert((op_project_similarity.query_tensor.empty() ^ (op_project_similarity.query_object.id == ObjectId::NULL_ID))
           && "Exactly one of the following must be non-null: query_tensor, query_object");

    if (!quad_model.catalog().name2tensor_store.contains(op_project_similarity.tensor_store_name))
        throw QueryException("Tensor store \"" + op_project_similarity.tensor_store_name + "\" does not exist");

    TensorStore& tensor_store = *quad_model.catalog().name2tensor_store.at(op_project_similarity.tensor_store_name);

    // Like SimilaritySearchPlan constructor
    std::vector<float> query_tensor;
    if (op_project_similarity.query_tensor.empty()) {
        // It is necessary to read from the tensor store
        query_tensor.resize(tensor_store.tensors_dim);
        bool found = tensor_store.get(op_project_similarity.query_object.id, query_tensor);
        if (!found)
            throw QueryException("ObjectId for " + MQL::Conversions::to_lexical_str(op_project_similarity.query_object) + " not found in tensor store");
    } else {
        query_tensor = std::move(op_project_similarity.query_tensor);
    }

    if (tensor_store.tensors_dim != query_tensor.size())
        throw QueryException("Input tensor must have dimension " + std::to_string(tensor_store.tensors_dim));

    tmp = std::make_unique<LSH::ProjectTensorSimilarity>(
        std::move(tmp),
        op_project_similarity.object_var,
        op_project_similarity.similarity_var,
        std::move(query_tensor),
        tensor_store,
        op_project_similarity.metric_type
    );
}


void BindingIterConstructor::visit(OpReturn& op_return) {
    for (auto&& [var, expr] : op_return.projection) {
        if (expr != nullptr && expr->has_aggregation()) {
            grouping = true;
            break;
        }
    }
    op_return.op->accept_visitor(*this);

    for (auto&& [var, expr] : op_return.projection) {
        if (expr != nullptr) {
            ExprToBindingExpr expr_to_binding_expr(
                this,
                var
            );
            expr->accept_visitor(expr_to_binding_expr);
        } else {
            if (grouping && group_vars.find(var) == group_vars.end()) {
                throw QuerySemanticException(
                    "Invalid use of var \""
                    + get_query_ctx().get_var_name(var)
                    + "\" in RETURN");
            }
        }
        order_by_saved_vars.insert(var); // may be unused if there is no ORDER BY
        projected_vars.push_back(var);
    }

    if (group_vars.size() > 0) {
        std::vector<bool> ascending(group_vars.size(), true);

        tmp = std::make_unique<OrderBy>(
            std::move(tmp),
            std::move(group_saved_vars),
            std::move(group_vars_vector),
            std::move(ascending),
            &MQL::Comparisons::compare
        );
    }

    if (aggregations.size() > 0) {
        tmp = std::make_unique<Aggregation>(
            std::move(tmp),
            std::move(aggregations),
            std::move(group_vars)
        );
    }

    if (order_by_vars.size() > 0) {
        tmp = std::make_unique<OrderBy>(
            std::move(tmp),
            std::move(order_by_saved_vars),
            std::move(order_by_vars),
            std::move(order_by_ascending),
            &MQL::Comparisons::compare
        );
    }

    if (op_return.distinct) {
        // TODO: if everything is ordered having the distinct variables at first we can avoid the hash
        tmp = std::make_unique<DistinctHash>(
            std::move(tmp),
            std::move(projected_vars)
        );
    }

    if (op_return.offset != Op::DEFAULT_OFFSET || op_return.limit != Op::DEFAULT_LIMIT || quad_model.MAX_LIMIT != Op::DEFAULT_LIMIT) {
        auto limit = op_return.limit;
        if (op_return.limit > quad_model.MAX_LIMIT) {
            limit = quad_model.MAX_LIMIT;
        }
        tmp = std::make_unique<Slice>(std::move(tmp), op_return.offset, limit);
    }
}


Id BindingIterConstructor::replace_setted_var(Id id) const {
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


bool BindingIterConstructor::term_exists(ObjectId term) const {
    if (term.is_not_found()) {
        return false;
    } else if ((term.id & ObjectId::TYPE_MASK) == ObjectId::MASK_ANON_INLINED) {
        auto anon_id = term.id & ObjectId::VALUE_MASK;
        return anon_id <= quad_model.catalog().anonymous_nodes_count;
    } else if ((term.id & ObjectId::TYPE_MASK) == ObjectId::MASK_EDGE) {
        auto conn_id = term.id & ObjectId::VALUE_MASK;
        return conn_id <= quad_model.catalog().connections_count;
    } else {
        // search in nodes
        Record<1> r = { term.id };
        bool interruption_requested = false;
        auto it = quad_model.nodes->get_range(&interruption_requested, r, r);
        return it.next() != nullptr;
    }
}
