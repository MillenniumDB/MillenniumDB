#pragma once

#include "graph_models/gql/gql_model.h"
#include "streaming_response_writer.h"

namespace MDBServer {

class StreamingGQLResponseWriter : public StreamingResponseWriter {
public:
    StreamingGQLResponseWriter(StreamingSession& session) :
        StreamingResponseWriter(session)
    { }

    uint64_t get_model_id() const override
    {
        return GQLCatalog::MODEL_ID;
    }

    uint64_t get_catalog_version() const override
    {
        return GQLCatalog::VERSION;
    }

    std::string encode_object_id(const ObjectId& oid) const override
    {
        const auto type = oid.get_type();
        const auto value = oid.get_value();

        switch (type) {
        case ObjectId::MASK_NODE:
            return encode_string("_n" + std::to_string(value), Protocol::DataType::ANON);
        case ObjectId::MASK_DIRECTED_EDGE:
            return encode_string("_e" + std::to_string(value), Protocol::DataType::ANON);
        case ObjectId::MASK_UNDIRECTED_EDGE:
            return encode_string("_u" + std::to_string(value), Protocol::DataType::ANON);
        case ObjectId::MASK_NODE_LABEL:
            return encode_string(gql_model.catalog.node_labels_str[value], Protocol::DataType::STRING);
        case ObjectId::MASK_EDGE_LABEL:
            return encode_string(gql_model.catalog.edge_labels_str[value], Protocol::DataType::STRING);
        case ObjectId::MASK_NODE_KEY:
            return encode_string(gql_model.catalog.node_keys_str[value], Protocol::DataType::STRING);
        case ObjectId::MASK_EDGE_KEY:
            return encode_string(gql_model.catalog.edge_keys_str[value], Protocol::DataType::STRING);
        default:
            throw std::logic_error(
                "Unmanaged type in GQLResponseWriter::encode_object_id: "
                + std::to_string(static_cast<uint8_t>(type))
            );
        }
    }
};
} // namespace MDBServer
