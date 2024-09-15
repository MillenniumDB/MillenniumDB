#include "describe_streaming_executor.h"

#include "graph_models/quad_model/quad_model.h"
#include "network/server/protocol.h"
#include "storage/index/bplus_tree/bplus_tree.h"

using namespace MQL;

DescribeStreamingExecutor::DescribeStreamingExecutor(std::unique_ptr<BindingIter> node_label_iter_,
                                                     std::unique_ptr<BindingIter> key_value_iter_,
                                                     std::unique_ptr<BindingIter> from_to_type_edge_iter_,
                                                     std::unique_ptr<BindingIter> to_type_from_edge_iter_,
                                                     uint64_t                     labels_limit_,
                                                     uint64_t                     properties_limit_,
                                                     uint64_t                     outgoing_limit_,
                                                     uint64_t                     incoming_limit_,
                                                     std::vector<VarId>&&         virtual_vars_,
                                                     ObjectId                     object_id_) :
    node_label_iter { std::move(node_label_iter_) },
    key_value_iter { std::move(key_value_iter_) },
    from_to_type_edge_iter { std::move(from_to_type_edge_iter_) },
    to_type_from_edge_iter { std::move(to_type_from_edge_iter_) },
    labels_limit { labels_limit_ },
    properties_limit { properties_limit_ },
    outgoing_limit { outgoing_limit_ },
    incoming_limit { incoming_limit_ },
    virtual_vars { std::move(virtual_vars_) },
    object_id { object_id_ } {
    // The projection vars are somewhat internal vars too, they are only used for the get_projections_vars method
    if (labels_limit > 0) {
        projection_vars.push_back(get_query_ctx().get_or_create_var("labels"));
    }
    if (properties_limit > 0) {
        projection_vars.push_back(get_query_ctx().get_or_create_var("properties"));
    }
    if (outgoing_limit > 0) {
        projection_vars.push_back(get_query_ctx().get_or_create_var("outgoing"));
    }
    if (incoming_limit > 0) {
        projection_vars.push_back(get_query_ctx().get_or_create_var("incoming"));
    }
}


const std::vector<VarId>& DescribeStreamingExecutor::get_projection_vars() const {
    return projection_vars;
}


uint64_t DescribeStreamingExecutor::execute(MDBServer::StreamingResponseWriter& response_writer) {
    // The binding just needs to be big enough to hold all the internal vars
    Binding binding { get_query_ctx().get_var_size() };

    node_label_iter->begin(binding);
    key_value_iter->begin(binding);
    from_to_type_edge_iter->begin(binding);
    to_type_from_edge_iter->begin(binding);

    auto node_iter = quad_model.nodes->get_range(&get_query_ctx().thread_info.interruption_requested,
                                                 { object_id.id },
                                                 { object_id.id });

    const auto node_exists = node_iter.next() != nullptr;
    if (!node_exists) {
        return 0;
    }

    response_writer.write_map_header(2UL);
    response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
    response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

    response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
    response_writer.write_list_header(projection_vars.size());

    // Labels
    if (labels_limit > 0) {
        std::vector<ObjectId> labels;

        while (labels_limit > 0 && node_label_iter->next()) {
            labels.push_back(binding[virtual_vars[0]]);
            --labels_limit;
        }

        response_writer.write_list_header(labels.size());
        for (const auto& label : labels) {
            response_writer.write_object_id(label);
        }
    }

    // Properties
    if (properties_limit > 0) {
        std::vector<std::pair<ObjectId, ObjectId>> key_values;

        while (properties_limit > 0 && key_value_iter->next()) {
            key_values.push_back({ binding[virtual_vars[0]], binding[virtual_vars[1]] });
            --properties_limit;
        }

        response_writer.write_map_header(key_values.size());
        for (const auto& [key, value] : key_values) {
            response_writer.write_object_id(key);
            response_writer.write_object_id(value);
        }
    }

    // Outgoing
    if (outgoing_limit > 0) {
        std::vector<std::tuple<ObjectId, ObjectId, ObjectId, ObjectId>> from_to_type_edge;

        while (outgoing_limit > 0 && from_to_type_edge_iter->next()) {
            from_to_type_edge.push_back(
              { object_id, binding[virtual_vars[0]], binding[virtual_vars[1]], binding[virtual_vars[2]] });
            --outgoing_limit;
        }

        response_writer.write_list_header(from_to_type_edge.size());
        for (const auto& [from, to, type, edge] : from_to_type_edge) {
            response_writer.write_map_header(4);
            response_writer.write_string("from", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(from);
            response_writer.write_string("to", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(to);
            response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(type);
            response_writer.write_string("edge", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(edge);
        }
    }

    // Incoming
    if (incoming_limit > 0) {
        std::vector<std::tuple<ObjectId, ObjectId, ObjectId, ObjectId>> to_type_from_edge;

        while (incoming_limit > 0 && to_type_from_edge_iter->next()) {
            to_type_from_edge.push_back(
              { object_id, binding[virtual_vars[0]], binding[virtual_vars[1]], binding[virtual_vars[2]] });
            --incoming_limit;
        }

        response_writer.write_list_header(to_type_from_edge.size());
        for (const auto& [to, type, from, edge] : to_type_from_edge) {
            response_writer.write_map_header(4);
            response_writer.write_string("from", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(from);
            response_writer.write_string("to", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(to);
            response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(type);
            response_writer.write_string("edge", MDBServer::Protocol::DataType::STRING);
            response_writer.write_object_id(edge);
        }
    }

    response_writer.seal();

    return 1;
}


void DescribeStreamingExecutor::analyze(std::ostream& os, bool /*print_stats*/, int indent) const {
    os << std::string(indent, ' ') << "DescribeStreamingExecutor()";
}
