#include "binding_iter_constructor.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "query/executor/binding_iter/distinct_hash.h"
#include "query/executor/binding_iter/empty_binding_iter.h"
#include "query/executor/binding_iter/filter.h"
#include "query/executor/binding_iter/index_left_outer_join.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/order_by_id.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.cc"
#include "query/executor/binding_iter/single_result_binding_iter.h"
// #include "query/optimizer/plan/plan/join/hash_join_plan.h"
#include "graph_models/quad_model/comparisons.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/optimizer/plan/join_order/greedy_optimizer.h"
#include "query/optimizer/plan/join_order/leapfrog_optimizer.h"
#include "query/optimizer/plan/join_order/selinger_optimizer.h"
#include "query/optimizer/quad_model/expr_to_binding_expr.h"
#include "query/optimizer/quad_model/plan/disjoint_object_plan.h"
#include "query/optimizer/quad_model/plan/edge_plan.h"
#include "query/optimizer/quad_model/plan/label_plan.h"
#include "query/optimizer/quad_model/plan/path_plan.h"
#include "query/optimizer/quad_model/plan/property_plan.h"
#include "query/parser/op/mql/op_where.h"
#include "query/parser/op/op_visitor.h"

using namespace MQL;

constexpr auto MAX_SELINGER_PLANS = 0;

// TODO: duplicated
static inline std::set<VarId> set_intersection(const std::set<VarId>& set1, const std::set<VarId>& set2) {
    std::set<VarId> intersection;

    std::set_intersection(
        set1.cbegin(), set1.cend(),
        set2.cbegin(), set2.cend(),
        std::inserter(intersection, intersection.begin())
    );

    return intersection;
}

// static inline std::set<VarId> set_union(const std::set<VarId>& set1, const std::set<VarId>& set2) {
//     std::set<VarId> res;

//     std::set_union(
//         set1.cbegin(), set1.cend(),
//         set2.cbegin(), set2.cend(),
//         std::inserter(res, res.begin())
//     );

//     return res;
// }

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

void BindingIterConstructor::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    // Process disjoint Terms
    // if a term is not found we can assume the MATCH result is empty
    for (auto& disjoint_term : op_basic_graph_pattern.disjoint_terms) {
        if ( !term_exists(disjoint_term.term) ) {
            tmp = std::make_unique<EmptyBindingIter>();
            return;
        }
    }

    std::vector<std::unique_ptr<Plan>> base_plans;

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
            for (auto&& [var, key, value] : where_property_equalities) {
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
                replace_setted_var(op_property.key),
                replace_setted_var(op_property.value)
            )
        );
    }
    // Push equalities from where into the basic graph pattern
    // TODO: mdb
    // que pasa con MATCH (?x)->(?y) WHERE ?x.name == "Joe" ?
    // En visit del where deberia añadirse a where_property_equalities?
    for (auto&& [var, key, value] : where_property_equalities) {
        base_plans.push_back(
            std::make_unique<PropertyPlan>(
                replace_setted_var(var),
                key,
                value
            )
        );
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
            // TODO: use begin_at_left to print the direction correctly?
            std::make_unique<PathPlan>(
                path.var,
                replace_setted_var(path.from),
                replace_setted_var(path.to),
                *path.path,
                path.semantic)
        );
    }

    if (base_plans.size() == 0) {
        tmp = std::make_unique<SingleResultBindingIter>();
        return;
    }

    assert(tmp == nullptr);

    // Set input vars
    for (auto& plan : base_plans) {
        plan->set_input_vars(safe_assigned_vars);
    }

    // try to use leapfrog if there is a join
    if (base_plans.size() > 1) {
        tmp = LeapfrogOptimizer::try_get_iter(
            base_plans,
            get_query_ctx().get_var_size()
        );
    }

    if (tmp == nullptr) {
        std::unique_ptr<Plan> root_plan = nullptr;
        if (base_plans.size() <= MAX_SELINGER_PLANS) {
            SelingerOptimizer selinger_optimizer(base_plans);
            root_plan = selinger_optimizer.get_plan();
        } else {
            root_plan = GreedyOptimizer::get_plan(base_plans);
        }

        // std::cout << "\nPlan Generated:\n";
        // root_plan->print(std::cout, true);
        // std::cout << "\nestimated cost: " << root_plan->estimate_cost() << "\n";

        tmp = root_plan->get_binding_iter();
    }

    // insert new safe_assigned_vars
    for (auto& plan : base_plans) {
        for (auto var : plan->get_vars()) {
            safe_assigned_vars.insert(var);
        }
    }
}


void BindingIterConstructor::visit(OpReturn& op_return) {
    for (auto&& [var, expr] : op_return.projection) {
        if (expr != nullptr) {
            // TODO: save something to process the expr after visiting op_return.op
        }

        // TODO: if var is something like ?x.key
        // save something in optional_properties

        projected_vars.push_back(var);
    }

    op_return.op->accept_visitor(*this);

    // // aggs.size will be 0 if GroupBy moved it
    // if (aggs.size()) {
    //     tmp = make_unique<Aggregation>(
    //         std::move(tmp),
    //         std::move(aggs),
    //         group_saved_vars,
    //         std::move(group_vars)
    //     );
    // }

    if (op_return.distinct) {
        // TODO: sometime we can use ordered values to avoid the hash
        tmp = std::make_unique<DistinctHash>(
            std::move(tmp),
            std::move(projected_vars)
        );
    }

    // TODO: mdb
    // tmp = make_unique<Return>(move(tmp), move(projection_vars), op_return.limit);
}


void BindingIterConstructor::visit(OpMatch& op_match) {
    op_match.op->accept_visitor(*this);

    auto parent_vars = op_match.op->get_all_vars();

    for (auto& property : op_match.optional_properties) {
        // TODO: ignore from where_property_equalities?

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
    // add corresponding var_properties ?
    // for (auto& var : op_where.expr->get_all_vars()) {
    //     const auto& var_name = get_query_ctx().get_var_name(var);
    //     auto pos = var_name.find('.'); // TODO: mal? ojo con variables internas
    //     if (pos != std::string::npos) {
    //         // we split something like "?x1.key1" into "?x" and "key1"
    //         auto var_without_property = var_name.substr(0, pos);
    //         auto var_key              = var_name.substr(pos + 1);
    //     //     var_properties.insert({ Var(var_without_property), var_key });
    //     }
    // }

    // TODO: quiero un visitor que saque todas las igualdades obligatorias
    // y rellene where_property_equalities
    // y ademas me deje un uptr<BindingExpr> con la expresion restante
    // (nullptr si se no queda nada)

    // TODO: push negation inside, simplify constant expressions

    // This visitor separates what comes in the Where and what goes into the join
    ExprToBindingExpr expr_to_binding_expr;
    op_where.expr->accept_visitor(expr_to_binding_expr);

    auto binding_expr = std::move(expr_to_binding_expr.tmp);
    // where_properties = std::move(expr_to_binding_expr.properties); // TODO:

    op_where.op->accept_visitor(*this);


    if (binding_expr != nullptr) {
        std::vector<std::unique_ptr<BindingExpr>> exprs;
        exprs.push_back(std::move(binding_expr));
        tmp = std::make_unique<Filter>(
            std::move(tmp),
            std::move(exprs),
            false
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
        auto join_vars = set_intersection(parent_vars, rhs_vars);
        auto rhs_only_vars = set_difference(rhs_vars, join_vars);

        tmp = std::make_unique<IndexLeftOuterJoin>(
            std::move(lhs),
            std::move(tmp),
            set_to_vector(rhs_only_vars)
        );
    }
}


void BindingIterConstructor::visit(OpOrderBy& op_order_by) {
    std::vector<VarId> order_vars;
    std::vector<bool> ascending;

    int i = 0;
    for (auto& order_item : op_order_by.items) {
        order_vars.push_back(order_item);
        order_saved_vars.insert(order_item);
        ascending.push_back(op_order_by.ascending_order[i]);
        i++;
    }

    for (auto& var : projected_vars) {
        order_saved_vars.insert(var);
    }

    op_order_by.op->accept_visitor(*this);

    tmp = std::make_unique<OrderById>(
        std::move(tmp),
        std::move(order_saved_vars),
        std::move(order_vars),
        std::move(ascending),
        &MQL::Comparisons::compare
    );
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
