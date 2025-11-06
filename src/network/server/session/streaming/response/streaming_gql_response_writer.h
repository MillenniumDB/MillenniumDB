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

    void write_dictionary_key(const ObjectId& oid) override
    {
        std::stringstream ss;
        ss << oid;
        write_typed_string(ss.str(), Protocol::DataType::STRING);
    }

    void write_gql_path_edge(ObjectId edge_id, ObjectId direction_id)
    {
        switch (direction_id.id) {
        case ObjectId::DIRECTION_LEFT:
            write_string_raw("left");
            break;
        case ObjectId::DIRECTION_RIGHT:
            write_string_raw("right");
            break;
        case ObjectId::DIRECTION_UNDIRECTED:
            write_string_raw("undirected");
            break;
        }

        write_object_id(edge_id);
    }

    void write_gql_path(const ObjectId& oid)
    {
        std::vector<ObjectId> path;
        GQL::Conversions::unpack_path(oid, path);

        const uint32_t path_length = path.size() / 3;

        response_ostream.put(static_cast<char>(Protocol::DataType::PATH));
        write_size(path_length);

        auto it = path.begin();
        write_object_id(*it);
        it++;

        while (it != path.end()) {
            ObjectId edge_id = *it;
            it++;
            ObjectId direction_id = *it;
            it++;
            write_gql_path_edge(edge_id, direction_id);
            write_object_id(*it);
            it++;
        }
    }

    void write_object_id(const ObjectId& oid) override
    {
        const auto type = GQL_OID::get_type(oid);
        const auto value = oid.get_value();

        switch (type) {
        case GQL_OID::Type::NULL_ID: {
            write_null();
            break;
        }
        case GQL_OID::Type::STRING_SIMPLE_INLINE:
        case GQL_OID::Type::STRING_SIMPLE_EXTERN:
        case GQL_OID::Type::STRING_SIMPLE_TMP: {
            write_typed_string(GQL::Conversions::unpack_string(oid), Protocol::DataType::STRING);
            break;
        }
        case GQL_OID::Type::INT56_INLINE:
        case GQL_OID::Type::INT64_EXTERN:
        case GQL_OID::Type::INT64_TMP: {
            const int64_t i = GQL::Conversions::unpack_int(oid);
            write_int64(i);
            break;
        }
        case GQL_OID::Type::FLOAT32: {
            const int64_t f = GQL::Conversions::unpack_float(oid);
            write_float(f);
            break;
        }
        case GQL_OID::Type::DOUBLE64_EXTERN:
        case GQL_OID::Type::DOUBLE64_TMP: {
            const double d = GQL::Conversions::unpack_double(oid);
            write_double(d);
            break;
        }
        case GQL_OID::Type::DECIMAL_INLINE:
        case GQL_OID::Type::DECIMAL_EXTERN:
        case GQL_OID::Type::DECIMAL_TMP: {
            const Decimal dec = GQL::Conversions::unpack_decimal(oid);
            write_typed_string(dec.to_string(), Protocol::DataType::DECIMAL);
            break;
        }
        case GQL_OID::Type::DATE: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            write_date(datetime);
            break;
        }
        case GQL_OID::Type::TIME: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            write_time(datetime);
            break;
        }
        case GQL_OID::Type::DATETIME:
        case GQL_OID::Type::DATETIMESTAMP: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            write_datetime(datetime);
            break;
        }
        case GQL_OID::Type::BOOL: {
            const auto b = GQL::Conversions::unpack_bool(oid);
            write_bool(b);
            break;
        }
        case GQL_OID::Type::PATH: {
            write_gql_path(oid);
            break;
        }
        case GQL_OID::Type::NODE: {
            write_typed_string("_n" + std::to_string(value), Protocol::DataType::NAMED_NODE);
            break;
        }
        case GQL_OID::Type::DIRECTED_EDGE:
        case GQL_OID::Type::UNDIRECTED_EDGE: {
            write_edge(value);
            break;
        }
        case GQL_OID::Type::NODE_LABEL: {
            write_typed_string(gql_model.catalog.node_labels_str[value], Protocol::DataType::STRING);
            break;
        }
        case GQL_OID::Type::EDGE_LABEL: {
            write_typed_string(gql_model.catalog.edge_labels_str[value], Protocol::DataType::STRING);
            break;
        }
        case GQL_OID::Type::NODE_KEY: {
            write_typed_string(gql_model.catalog.node_keys_str[value], Protocol::DataType::STRING);
            break;
        }
        case GQL_OID::Type::EDGE_KEY: {
            write_typed_string(gql_model.catalog.edge_keys_str[value], Protocol::DataType::STRING);
            break;
        }
        case GQL_OID::Type::DICTIONARY: {
            std::unique_ptr<Dictionary> dictionary;
            Common::Conversions::unpack_dictionary(oid, dictionary);
            write_dictionary(*dictionary);
            break;
        }
        case GQL_OID::Type::LIST: {
            std::vector<ObjectId> oid_list;
            GQL::Conversions::unpack_list(oid, oid_list);
            write_list(oid_list);
            break;
        }
        default: {
            throw std::logic_error(
                "Unmanaged type in StreamingGQLResponseWriter::encode_object_id: "
                + std::to_string(static_cast<uint8_t>(type))
            );
        }
        }
    }
};
} // namespace MDBServer
