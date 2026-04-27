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

    void write_object_id(ObjectId oid) override
    {
        const auto value = oid.get_value();

        switch (oid.type()) {
        case ObjectType::Null: {
            write_null();
            break;
        }
        case ObjectType::StringInl:
        case ObjectType::StringExt:
        case ObjectType::StringTmp: {
            write_typed_string(GQL::Conversions::unpack_string(oid), Protocol::DataType::STRING);
            break;
        }
        case ObjectType::PositiveInt56:
        case ObjectType::NegativeInt56: {
            const int64_t i = GQL::Conversions::unpack_int(oid);
            write_int64(i);
            break;
        }
        case ObjectType::Float: {
            const int64_t f = GQL::Conversions::unpack_float(oid);
            write_float(f);
            break;
        }
        case ObjectType::DoubleExt:
        case ObjectType::DoubleTmp: {
            const double d = GQL::Conversions::unpack_double(oid);
            write_double(d);
            break;
        }
        case ObjectType::DecimalInl:
        case ObjectType::DecimalExt:
        case ObjectType::DecimalTmp: {
            const Decimal dec = GQL::Conversions::unpack_decimal(oid);
            write_typed_string(dec.to_string(), Protocol::DataType::DECIMAL);
            break;
        }
        case ObjectType::Date: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            write_date(datetime);
            break;
        }
        case ObjectType::Time: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            write_time(datetime);
            break;
        }
        case ObjectType::Datetime:
        case ObjectType::Datetimestamp: {
            const DateTime datetime = GQL::Conversions::unpack_date(oid);
            write_datetime(datetime);
            break;
        }
        case ObjectType::Bool: {
            const auto b = GQL::Conversions::unpack_bool(oid);
            write_bool(b);
            break;
        }
        case ObjectType::Path: {
            write_gql_path(oid);
            break;
        }
        case ObjectType::AnonInl: {
            write_typed_string("_n" + std::to_string(value), Protocol::DataType::NAMED_NODE);
            break;
        }
        case ObjectType::DirectedEdge:
        case ObjectType::UndirectedEdge: {
            write_edge(value); // TODO: should differentiate directed and undirected
            break;
        }
        case ObjectType::NodeLabel: {
            write_typed_string(gql_model.catalog.node_labels_str[value], Protocol::DataType::STRING);
            break;
        }
        case ObjectType::EdgeLabel: {
            write_typed_string(gql_model.catalog.edge_labels_str[value], Protocol::DataType::STRING);
            break;
        }
        case ObjectType::NodeKey: {
            write_typed_string(gql_model.catalog.node_keys_str[value], Protocol::DataType::STRING);
            break;
        }
        case ObjectType::EdgeKey: {
            write_typed_string(gql_model.catalog.edge_keys_str[value], Protocol::DataType::STRING);
            break;
        }
        case ObjectType::DictionaryExt:
        case ObjectType::DictionaryTmp: {
            std::unique_ptr<Dictionary> dictionary;
            Common::Conversions::unpack_dictionary(oid, dictionary);
            write_dictionary(*dictionary);
            break;
        }
        case ObjectType::ListExt:
        case ObjectType::ListTmp: {
            std::vector<ObjectId> oid_list;
            GQL::Conversions::unpack_list(oid, oid_list);
            write_list(oid_list);
            break;
        }
        case ObjectType::TensorFloatExt:
        case ObjectType::TensorFloatTmp:
        case ObjectType::TensorDoubleExt:
        case ObjectType::TensorDoubleTmp:
        case ObjectType::AnonTmp:
        case ObjectType::StringXsdInl:
        case ObjectType::StringXsdExt:
        case ObjectType::StringXsdTmp:
        case ObjectType::StringLangInl:
        case ObjectType::StringLangExt:
        case ObjectType::StringLangTmp:
        case ObjectType::StringDatatypeInl:
        case ObjectType::StringDatatypeExt:
        case ObjectType::StringDatatypeTmp:
        case ObjectType::IriInl:
        case ObjectType::IriExt:
        case ObjectType::IriTmp:
        case ObjectType::IriUuidLowerTmp:
        case ObjectType::IriUuidLowerExt:
        case ObjectType::IriUuidUpperTmp:
        case ObjectType::IriUuidUpperExt:
        case ObjectType::IriHexLowerTmp:
        case ObjectType::IriHexLowerExt:
        case ObjectType::IriHexUpperTmp:
        case ObjectType::IriHexUpperExt:
        case ObjectType::NamedNodeInl:
        case ObjectType::NamedNodeExt:
        case ObjectType::NamedNodeTmp:
        case ObjectType::NamedNodeHexExt:
        case ObjectType::NamedNodeHexTmp:
        case ObjectType::NamedNodeUuidExt:
        case ObjectType::NamedNodeUuidTmp:
        case ObjectType::Direction:
        case ObjectType::NotFound:
            break;
        }
    }
};
} // namespace MDBServer
