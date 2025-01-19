#include "executor_constructor.h"

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "system/path_manager.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/executor/binding_iter/scan_ranges/term.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
#include "query/executor/query_executor/mql/describe_executor.h"
#include "query/executor/query_executor/mql/return_executor.h"
#include "query/executor/query_executor/mql/show_executor.h"
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

    path_manager.begin(std::move(visitor.begin_at_left));

    if (ret == MQL::ReturnType::CSV) {
        executor = std::make_unique<MQL::ReturnExecutor<MQL::ReturnType::CSV>>(std::move(visitor.tmp),
                                                                               std::move(set_vars),
                                                                               std::move(projection_vars));
    } else {
        executor = std::make_unique<MQL::ReturnExecutor<MQL::ReturnType::TSV>>(std::move(visitor.tmp),
                                                                               std::move(set_vars),
                                                                               std::move(projection_vars));
    }
}

void ExecutorConstructor::visit(OpShow& op_show) {
    if (ret == MQL::ReturnType::CSV) {
        if (op_show.type == MQL::OpShow::Type::TENSOR_STORE) {
            executor = std::make_unique<MQL::ShowExecutor<MQL::ReturnType::CSV, MQL::OpShow::Type::TENSOR_STORE>>();
        } else if (op_show.type == MQL::OpShow::Type::TEXT_SEARCH_INDEX) {
            executor = std::make_unique<MQL::ShowExecutor<MQL::ReturnType::CSV, MQL::OpShow::Type::TEXT_SEARCH_INDEX>>();
        } else {
            throw std::runtime_error("Invalid type");
        }
    } else {
        if (op_show.type == MQL::OpShow::Type::TENSOR_STORE) {
            executor = std::make_unique<MQL::ShowExecutor<MQL::ReturnType::TSV, MQL::OpShow::Type::TENSOR_STORE>>();
        } else if (op_show.type == MQL::OpShow::Type::TEXT_SEARCH_INDEX) {
            executor = std::make_unique<MQL::ShowExecutor<MQL::ReturnType::TSV, MQL::OpShow::Type::TEXT_SEARCH_INDEX>>();
        } else {
            throw std::runtime_error("Invalid type");
        }
    }
}
