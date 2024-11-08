#include "streaming_executor_constructor.h"

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "system/path_manager.h"
#include "query/executor/binding_iter/scan_ranges/term.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
#include "query/executor/query_executor/mql/describe_streaming_executor.h"
#include "query/executor/query_executor/mql/return_streaming_executor.h"
#include "query/optimizer/quad_model/binding_iter_constructor.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void StreamingExecutorConstructor::visit(OpDescribe& op_describe) {
    std::unique_ptr<BindingIter> labels;
    std::unique_ptr<BindingIter> properties;
    std::unique_ptr<BindingIter> outgoing_connections;
    std::unique_ptr<BindingIter> incoming_connections;

    ObjectId object_id = op_describe.node;

    // We need at least three virtual variables for a DESCRIBE query
    auto var0 = get_query_ctx().get_internal_var();
    auto var1 = get_query_ctx().get_internal_var();
    auto var2 = get_query_ctx().get_internal_var();

    {
        std::array<std::unique_ptr<ScanRange>, 2> ranges { ScanRange::get(object_id, true),
                                                           ScanRange::get(var0, false) };
        labels = std::make_unique<IndexScan<2>>(*quad_model.node_label, std::move(ranges));
    }

    {
        std::array<std::unique_ptr<ScanRange>, 3> ranges;
        ranges[0]  = std::make_unique<Term>(object_id);
        ranges[1]  = std::make_unique<UnassignedVar>(var0);
        ranges[2]  = std::make_unique<UnassignedVar>(var1);
        properties = std::make_unique<IndexScan<3>>(*quad_model.object_key_value, std::move(ranges));
    }

    {
        std::array<std::unique_ptr<ScanRange>, 4> ranges;
        ranges[0]            = std::make_unique<Term>(object_id);
        ranges[1]            = std::make_unique<UnassignedVar>(var0);
        ranges[2]            = std::make_unique<UnassignedVar>(var1);
        ranges[3]            = std::make_unique<UnassignedVar>(var2);
        outgoing_connections = std::make_unique<IndexScan<4>>(*quad_model.from_to_type_edge, std::move(ranges));
    }

    {
        std::array<std::unique_ptr<ScanRange>, 4> ranges;
        ranges[0]            = std::make_unique<Term>(object_id);
        ranges[1]            = std::make_unique<UnassignedVar>(var0);
        ranges[2]            = std::make_unique<UnassignedVar>(var1);
        ranges[3]            = std::make_unique<UnassignedVar>(var2);
        incoming_connections = std::make_unique<IndexScan<4>>(*quad_model.to_type_from_edge, std::move(ranges));
    }

    std::vector<VarId> virtual_vars = { var0, var1, var2 };

    executor = std::make_unique<MQL::DescribeStreamingExecutor>(std::move(labels),
                                                                std::move(properties),
                                                                std::move(outgoing_connections),
                                                                std::move(incoming_connections),
                                                                op_describe.labels_limit,
                                                                op_describe.properties_limit,
                                                                op_describe.outgoing_limit,
                                                                op_describe.incoming_limit,
                                                                std::move(virtual_vars),
                                                                object_id);
}

void StreamingExecutorConstructor::visit(OpReturn& op_return) {
    BindingIterConstructor visitor(set_vars);
    op_return.accept_visitor(visitor);

    std::vector<VarId> projection_vars;
    projection_vars.reserve(op_return.projection.size());
    for (const auto& [var_id, _] : op_return.projection) {
        projection_vars.emplace_back(var_id);
    }

    path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<MQL::ReturnStreamingExecutor>(std::move(visitor.tmp),
                                                              std::move(set_vars),
                                                              std::move(projection_vars));
}

void StreamingExecutorConstructor::visit(OpSet& op_set) {
    for (auto& set_item : op_set.set_items) {
        set_vars.insert(set_item);
    }
    op_set.op->accept_visitor(*this);
}