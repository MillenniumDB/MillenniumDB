#include "edge_describe_streaming_executor.h"

#include "graph_models/quad_model/quad_model.h"
#include "network/server/protocol.h"

using namespace MQL;

EdgeDescribeStreamingExecutor::EdgeDescribeStreamingExecutor(
    std::unique_ptr<BindingIter> key_value_iter_,
    uint64_t properties_limit_,
    std::vector<VarId>&& virtual_vars_,
    ObjectId object_id_
) :
    key_value_iter { std::move(key_value_iter_) },
    properties_limit { properties_limit_ },
    object_id { object_id_ },
    virtual_vars { std::move(virtual_vars_) }
{
    projection_vars.push_back(get_query_ctx().get_or_create_var("object"));
    projection_vars.push_back(get_query_ctx().get_or_create_var("from"));
    projection_vars.push_back(get_query_ctx().get_or_create_var("to"));
    projection_vars.push_back(get_query_ctx().get_or_create_var("type"));

    // The projection vars are somewhat internal vars too, they are only used for the get_projections_vars method
    if (properties_limit > 0) {
        projection_vars.push_back(get_query_ctx().get_or_create_var("properties"));
    }
}

const std::vector<VarId>& EdgeDescribeStreamingExecutor::get_projection_vars() const
{
    return projection_vars;
}

uint64_t EdgeDescribeStreamingExecutor::execute(MDBServer::StreamingResponseWriter& response_writer)
{
    // The binding just needs to be big enough to hold all the internal vars
    Binding binding { get_query_ctx().get_var_size() };

    key_value_iter->begin(binding);

    const auto edge_id = object_id.id;

    bool interruption_requested = false;
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    min_ids[0] = edge_id;
    max_ids[0] = edge_id;

    for (size_t i = 1; i < 4; ++i) {
        min_ids[i] = 0;
        max_ids[i] = UINT64_MAX;
    }
    auto it = quad_model.edge_from_to_type->get_range(&interruption_requested, min_ids, max_ids);
    const Record<4>* record = it.next();

    if (record == nullptr) {
        return 0;
    }

    response_writer.write_map_header(2UL);
    response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
    response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

    response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
    response_writer.write_list_header(projection_vars.size());

    response_writer.write_object_id(object_id); // edge_id

    response_writer.write_object_id(ObjectId((*record)[1])); // from
    response_writer.write_object_id(ObjectId((*record)[2])); // to
    response_writer.write_object_id(ObjectId((*record)[3])); // type

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

    response_writer.seal();

    return 1;
}

void EdgeDescribeStreamingExecutor::analyze(std::ostream& os, bool /*print_stats*/, int indent) const
{
    os << std::string(indent, ' ') << "EdgeDescribeStreamingExecutor()\n";
}
