#pragma once

#include "graph_models/gql/conversions.h"
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
        return GQLCatalog::MAJOR_VERSION;
    }

    std::string encode_path_edge(ObjectId edge_id, ObjectId direction_id) const
    {
        std::string direction;

        switch (direction_id.id) {
        case ObjectId::DIRECTION_LEFT:
            direction = "left";
            break;
        case ObjectId::DIRECTION_RIGHT:
            direction = "right";
            break;
        case ObjectId::DIRECTION_UNDIRECTED:
            direction = "undirected";
            break;
        }

        std::string res;
        res += encode_size(direction.size());
        res += direction;
        res += encode_object_id(edge_id);
        return res;
    }

    std::string encode_gql_path(const ObjectId& oid) const
    {
        std::vector<ObjectId> path;
        GQL::Conversions::unpack_path(oid, path);

        GQL::Conversions::debug_print(std::cout, oid);
        std::cout << std::endl;

        std::string res;
        uint32_t path_length = path.size() / 3;

        res += static_cast<char>(Protocol::DataType::PATH);
        res += encode_size(path_length);

        auto it = path.begin();
        res += encode_object_id(*it);
        it++;

        while (it != path.end()) {
            ObjectId edge_id = *it;
            it++;
            ObjectId direction_id = *it;
            it++;
            res += encode_path_edge(edge_id, direction_id);
            res += encode_object_id(*it);
            it++;
        }
        return res;
    }

    std::string encode_gql_list(const ObjectId& oid) const
    {
        std::vector<ObjectId> oid_list;
        GQL::Conversions::unpack_list(oid, oid_list);

        std::string res;
        res += static_cast<char>(Protocol::DataType::LIST);
        res += encode_size(oid_list.size());

        for (auto it = oid_list.begin(); it != oid_list.end(); ++it) {
            res += encode_object_id(*it);
        }
        return res;
    }

    std::string encode_object_id(const ObjectId& oid) const override
    {
        const auto type = GQL_OID::get_type(oid);
        const auto value = oid.get_value();

        switch (type) {
        case GQL_OID::Type::NULL_ID: {
            return encode_null();
        }
        case GQL_OID::Type::STRING_SIMPLE_INLINE:
        case GQL_OID::Type::STRING_SIMPLE_EXTERN:
        case GQL_OID::Type::STRING_SIMPLE_TMP: {
            return encode_string(GQL::Conversions::unpack_string(oid), Protocol::DataType::STRING);
        }
        case GQL_OID::Type::INT56_INLINE:
        case GQL_OID::Type::INT64_EXTERN:
        case GQL_OID::Type::INT64_TMP: {
            const int64_t i = GQL::Conversions::unpack_int(oid);
            return encode_int64(i);
        }
        case GQL_OID::Type::FLOAT32: {
            const int64_t f = GQL::Conversions::unpack_float(oid);
            return encode_float(f);
        }
        case GQL_OID::Type::DOUBLE64_EXTERN:
        case GQL_OID::Type::DOUBLE64_TMP: {
            const double d = GQL::Conversions::unpack_double(oid);
            return encode_double(d);
        }
        case GQL_OID::Type::DECIMAL_INLINE:
        case GQL_OID::Type::DECIMAL_EXTERN:
        case GQL_OID::Type::DECIMAL_TMP: {
            const Decimal dec = GQL::Conversions::unpack_decimal(oid);
            return encode_string(dec.to_string(), Protocol::DataType::DECIMAL);
        }
        case GQL_OID::Type::DATE: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            return encode_date(datetime);
        }
        case GQL_OID::Type::TIME: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            return encode_time(datetime);
        }
        case GQL_OID::Type::DATETIME:
        case GQL_OID::Type::DATETIMESTAMP: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            return encode_datetime(datetime);
        }
        case GQL_OID::Type::BOOL: {
            const auto b = GQL::Conversions::unpack_bool(oid);
            return encode_bool(b);
        }
        case GQL_OID::Type::PATH: {
            return encode_gql_path(oid);
        }
        case GQL_OID::Type::NODE: {
            return encode_string("_n" + std::to_string(value), Protocol::DataType::NAMED_NODE);
        }
        case GQL_OID::Type::DIRECTED_EDGE: {
            return encode_string("_e" + std::to_string(value), Protocol::DataType::EDGE);
        }
        case GQL_OID::Type::UNDIRECTED_EDGE: {
            return encode_string("_u" + std::to_string(value), Protocol::DataType::EDGE);
        }
        case GQL_OID::Type::NODE_LABEL: {
            return encode_string(gql_model.catalog.node_labels_str[value], Protocol::DataType::STRING);
        }
        case GQL_OID::Type::EDGE_LABEL: {
            return encode_string(gql_model.catalog.edge_labels_str[value], Protocol::DataType::STRING);
        }
        case GQL_OID::Type::NODE_KEY: {
            return encode_string(gql_model.catalog.node_keys_str[value], Protocol::DataType::STRING);
        }
        case GQL_OID::Type::EDGE_KEY: {
            return encode_string(gql_model.catalog.edge_keys_str[value], Protocol::DataType::STRING);
        }
        case GQL_OID::Type::LIST: {
            return encode_gql_list(oid);
        }
        default: {
            return encode_null();
        }
        }
    }
};
} // namespace MDBServer
