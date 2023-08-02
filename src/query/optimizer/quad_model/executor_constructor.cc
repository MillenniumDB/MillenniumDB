#include "executor_constructor.h"

// #include "query/executor/binding_iter/distinct_hash.h"
#include "query/executor/binding_iter/index_scan.h"
// #include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/binding_iter/scan_ranges/assigned_var.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/executor/binding_iter/scan_ranges/term.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
// #include "query/executor/binding_iter/aggregation.h"
// #include "query/executor/binding_iter/describe.h"
// #include "query/executor/binding_iter/distinct_ordered.h"
// #include "query/executor/binding_iter/distinct_hash.h"
// #include "query/executor/binding_iter/match.h"
// #include "query/executor/binding_iter/group_by.h"
// #include "query/executor/binding_iter/order_by.h"
// #include "query/executor/binding_iter/return.h"
// #include "query/executor/binding_iter/where.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/executor/query_executor/mql/csv_return_executor.h"
#include "query/executor/query_executor/mql/describe_executor.h"
#include "query/optimizer/quad_model/binding_iter_constructor.h"

using namespace MQL;

void ExecutorConstructor::visit(OpDescribe& op_describe) {
    std::unique_ptr<BindingIter> labels;
    std::unique_ptr<BindingIter> properties;
    std::unique_ptr<BindingIter> outgoing_connections;
    std::unique_ptr<BindingIter> incoming_connections;

    ObjectId object_id = op_describe.node;

    VarId label_var(0);
    VarId key_var(0);
    VarId value_var(1);
    VarId from_var(0);
    VarId to_var(0);
    VarId type_var(1);
    VarId edge_var(2);

    {
        std::array<std::unique_ptr<ScanRange>, 2> ranges;
        ranges[0] = std::make_unique<Term>(object_id);
        ranges[1] = std::make_unique<UnassignedVar>(label_var);
        labels = std::make_unique<IndexScan<2>>(
            *quad_model.node_label,
            std::move(ranges)
        );
    }

    {
        std::array<std::unique_ptr<ScanRange>, 3> ranges;
        ranges[0] = std::make_unique<Term>(object_id);
        ranges[1] = std::make_unique<UnassignedVar>(key_var);
        ranges[2] = std::make_unique<UnassignedVar>(value_var);
        properties = std::make_unique<IndexScan<3>>(
            *quad_model.object_key_value,
            std::move(ranges)
        );
    }

    {
        std::array<std::unique_ptr<ScanRange>, 4> ranges;
        ranges[0] = std::make_unique<Term>(object_id);
        ranges[1] = std::make_unique<UnassignedVar>(to_var);
        ranges[2] = std::make_unique<UnassignedVar>(type_var);
        ranges[3] = std::make_unique<UnassignedVar>(edge_var);
        outgoing_connections = std::make_unique<IndexScan<4>>(
            *quad_model.from_to_type_edge,
            std::move(ranges)
        );
    }

    {
        std::array<std::unique_ptr<ScanRange>, 4> ranges;
        ranges[0] = std::make_unique<Term>(object_id);
        ranges[1] = std::make_unique<UnassignedVar>(type_var);
        ranges[2] = std::make_unique<UnassignedVar>(from_var);
        ranges[3] = std::make_unique<UnassignedVar>(edge_var);
        incoming_connections = std::make_unique<IndexScan<4>>(
            *quad_model.to_type_from_edge,
            std::move(ranges)
        );
    }

    executor = std::make_unique<MQL::DescribeExecutor>(
        std::move(labels),
        std::move(properties),
        std::move(outgoing_connections),
        std::move(incoming_connections),
        op_describe.labels_limit,
        op_describe.properties_limit,
        op_describe.outgoing_limit,
        op_describe.incoming_limit
    );
}


void ExecutorConstructor::visit(OpSet& op_set) {
    for (auto& set_item : op_set.set_items) {
        set_vars.insert({ set_item.first, set_item.second });
    }
    op_set.op->accept_visitor(*this);
}


void ExecutorConstructor::visit(OpReturn& op_return) {
    BindingIterConstructor visitor(set_vars);
    op_return.accept_visitor(visitor);

    std::vector<VarId> projection_vars;
    projection_vars.reserve(op_return.projection.size());
    for (auto&& [var, _] : op_return.projection) {
        projection_vars.push_back(var);
    }

    auto var_size = get_query_ctx().get_var_size();
    // TODO: correctly set begin_at_left
    std::vector<bool> begin_at_left(var_size, true);
    path_manager.begin(var_size, std::move(begin_at_left), false);

    executor = std::make_unique<MQL::CSVReturnExecutor>(
        std::move(visitor.tmp),
        std::move(set_vars),
        std::move(projection_vars),
        op_return.limit
    );
    // save the return items to be able to push optional properties from RETURN to MATCH
    // TODO: mdb
    // ReturnItemVisitorImpl return_item_visitor(*this);
    // for (const auto& return_item : op_return.return_items) {
    //     return_item->accept_visitor(return_item_visitor);
    //     auto var = return_item->get_var();
    //     auto var_id = get_var_id(var);
    //     projection_vars.push_back({ var, var_id });
    // }

    // distinct_into_id = op_return.distinct; // OpWhere may change this value when accepting visitor

    // op_return.op->accept_visitor(*this);

    // // aggs.size will be 0 if GroupBy moved it
    // if (aggs.size()) {
    //     tmp = make_unique<Aggregation>(
    //         std::move(tmp),
    //         std::move(aggs),
    //         group_saved_vars,
    //         std::move(group_vars)
    //     );
    // }

    // if (op_return.distinct) {
    //     if (distinct_ordered_possible) {
    //         tmp = make_unique<DistinctOrdered>(std::move(tmp), projection_vars);
    //     } else {
    //         if (!distinct_into_id) {
    //             tmp = make_unique<DistinctHash>(std::move(tmp), projection_vars);
    //         }
    //         // else DistinctHash was created when visiting OpMatch
    //     }
    // }

    // TODO: mdb
    // tmp = make_unique<Return>(move(tmp), move(projection_vars), op_return.limit);
}


// void BindingIterVisitor1::visit(OpWhere& op_where) {
//     distinct_into_id = false;

    // add corresponding var_properties
    // TODO: mdb
    // for (auto& var : op_where.expr->get_all_vars()) {
    //     auto pos = var.name.find('.');
    //     if (pos != string::npos) {
    //         // we split something like "?x1.key1" into "?x" and "key1"
    //         auto var_without_property = var.name.substr(0, pos);
    //         auto var_key              = var.name.substr(pos + 1);
    //         var_properties.insert({ Var(var_without_property), var_key });
    //     }
    // }

    // TODO: push negation inside, simplify constant expressions

    // This visitor separates what comes in the Where and what goes into the join
    // ExprToBindingExpr expr_to_binding_expr(get_query_ctx());
    // op_where.expr->accept_visitor(expr_to_binding_expr);

    // auto binding_expr = std::move(expr_to_binding_expr.current_binding_expr);
    // where_properties  = std::move(expr_to_binding_expr.properties);

    // op_where.op->accept_visitor(*this);

    // if (binding_expr != nullptr) {
    //     tmp = make_unique<Where>(move(tmp), move(binding_expr));
    // }
// }


// void BindingIterVisitor1::visit(OpMatch& op_match) {
    // BindingIterVisitor1 id_visitor(get_query_ctx(), fixed_vars, where_properties);
    // op_match.op->accept_visitor(id_visitor);

    // unique_ptr<BindingIter> binding_iter_current_root = move(id_visitor.tmp);

    // const auto binding_size = get_query_ctx().get_var_size();

    // std::vector<bool> begin_at_left(binding_size); // TODO: set with proper values

    // // TODO: set need_materialize_paths if needed, for now it is always false
    // path_manager.begin(binding_size, std::move(begin_at_left), need_materialize_paths);

    // vector<unique_ptr<BindingIter>> optional_children;

    // Push properties from Select/Where/OrderBy into MATCH as optional children
    // TODO: mdb
    // for (const auto& [var, prop_name] : var_properties) {
    //     auto obj_var_id = get_var_id(var);
    //     auto value_var  = get_var_id(Var(var.name + '.' + prop_name));
    //     auto key_id     = quad_model.get_object_id(QueryElement(prop_name));

    //     // Check value_var does not have a constant value
    //     if (fixed_vars.find(value_var) == fixed_vars.end()) {
    //         array<unique_ptr<ScanRange>, 3> ranges {
    //             ScanRange::get(obj_var_id, true),
    //             ScanRange::get(key_id, true),
    //             ScanRange::get(value_var, false)
    //         };
    //         auto index_scan = make_unique<IndexScan<3>>(*quad_model.object_key_value, get_query_ctx().thread_info, move(ranges));
    //         optional_children.push_back(move(index_scan));
    //     }
    // }
    // if (optional_children.size() > 0) {
    //     binding_iter_current_root = make_unique<OptionalNode>(move(binding_iter_current_root),
    //                                                              move(optional_children));
    // }
    // if (distinct_into_id && aggs.size() == 0) {
    //     std::vector<VarId> projected_var_ids;
    //     for (const auto& [var, var_id] : projection_vars) {
    //         projected_var_ids.push_back(var_id);
    //     }

    //     binding_iter_current_root = make_unique<DistinctHash>(move(binding_iter_current_root), move(projected_var_ids));
    // }

    // tmp = make_unique<Match>(move(binding_iter_current_root), binding_size, fixed_vars);
// }


// void BindingIterVisitor1::visit(OpOrderBy& op_order_by) {
//     std::vector<VarId> order_vars;
//     std::set<VarId> saved_vars;

//     ReturnItemVisitorImpl return_item_visitor(*this);

//     for (auto& order_item : op_order_by.items) {
//         order_item->accept_visitor(return_item_visitor);
//         auto var = order_item->get_var();

//         auto pos = var.name.find('.');
//         if (pos != string::npos) {
//             // we split something like "?x1.key1" into "?x" and "key1"
//             auto var_without_property = var.name.substr(0, pos);
//             auto var_key              = var.name.substr(pos + 1);
//             var_properties.insert({ Var(var_without_property), var_key });
//         }
//         const auto var_id = get_var_id(var);
//         order_vars.push_back(var_id);
//         saved_vars.insert(var_id);
//     }

//     for (auto& [var, var_id] : projection_vars) {
//         saved_vars.insert(var_id);
//     }

//     // TODO: we could set distinct_ordered_possible=true if the projection vars are in the begining
//     // e.g. if we have ORDER BY ?x, ?z, ?y RETURN DISTINCT ?x, ?y we can't use DistinctOrdered

//     op_order_by.op->accept_visitor(*this);
//     tmp = make_unique<OrderBy>(thread_info, move(tmp), saved_vars, order_vars, op_order_by.ascending_order);
// }


// void BindingIterVisitor1::visit(OpGroupBy& op_group_by) {
//     group = true;

//     std::vector<VarId> group_order_vars;
//     std::vector<bool> ascending_order;

//     for (auto& var : op_group_by.items) {
//         auto var_id = get_var_id(var);

//         group_vars.push_back(var_id);
//         group_order_vars.push_back(var_id);
//         ascending_order.push_back(true);
//         group_saved_vars.insert(var_id);
//     }

//     op_group_by.op->accept_visitor(*this);

//     tmp = make_unique<OrderBy>(thread_info, move(tmp), group_saved_vars, move(group_order_vars), move(ascending_order));
//     tmp = make_unique<Aggregation>(move(tmp), move(aggs), group_saved_vars, move(group_vars));
// }
