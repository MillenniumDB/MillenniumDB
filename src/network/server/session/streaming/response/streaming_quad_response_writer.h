#include "streaming_response_writer.h"

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "graph_models/rdf_model/conversions.h"

namespace MDBServer {

class StreamingQuadResponseWriter : public StreamingResponseWriter {
public:
    StreamingQuadResponseWriter(StreamingSession& session) :
        StreamingResponseWriter(session)
    { }

    uint64_t get_model_id() const override
    {
        return QuadCatalog::MODEL_ID;
    }

    uint64_t get_catalog_version() const override
    {
        return QuadCatalog::MAJOR_VERSION;
    }

    void write_dictionary_key(const ObjectId& oid) override
    {
        const auto str = MQL::Conversions::to_lexical_str(oid);
        write_typed_string(str, Protocol::DataType::STRING);
    }

    void write_object_id(ObjectId oid) override
    {
        const auto value = oid.get_value();
        switch (oid.subtype()) {
        case ObjectSubType::Null: {
            write_null();
            break;
        }
        case ObjectSubType::Anon: {
            const auto anon_id = MQL::Conversions::unpack_anon(oid);
            write_anon(anon_id);
            break;
        }
        case ObjectSubType::NamedNode: {
            const auto str = MQL::Conversions::unpack_named_node(oid);
            write_typed_string(str, Protocol::DataType::NAMED_NODE);
            break;
        }
        case ObjectSubType::String: {
            const auto str = MQL::Conversions::unpack_string(oid);
            write_typed_string(str, Protocol::DataType::STRING);
            break;
        }
        case ObjectSubType::Int: {
            const int64_t i = MQL::Conversions::unpack_int(oid);
            write_int64(i);
            break;
        }
        case ObjectSubType::Decimal: {
            const Decimal dec = SPARQL::Conversions::unpack_decimal(oid);
            write_typed_string(dec.to_string(), Protocol::DataType::DECIMAL);
            break;
        }
        case ObjectSubType::Float: {
            write_float(MQL::Conversions::unpack_float(oid));
            break;
        }
        case ObjectSubType::Double: {
            write_double(MQL::Conversions::unpack_double(oid));
            break;
        }

        case ObjectSubType::Bool: {
            write_bool(value != 0);
            break;
        }
        case ObjectSubType::Edge: {
            write_edge(MQL::Conversions::unpack_edge(oid));
            break;
        }
        case ObjectSubType::TemporalLiteral: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            switch (oid.type()) {
            case ObjectType::Date:
                write_date(datetime);
                break;
            case ObjectType::Datetime:
            case ObjectType::Datetimestamp:
                write_datetime(datetime);
                break;
            case ObjectType::Time:
                write_time(datetime);
                break;
            default:
                assert(false);
                break;
            }
            break;
        }
        case ObjectSubType::Path: {
            write_path(value);
            break;
        }
        case ObjectSubType::TensorFloat: {
            const auto tensor = Common::Conversions::unpack_tensor<float>(oid);
            write_tensor<float>(tensor);
            break;
        }
        case ObjectSubType::TensorDouble: {
            const auto tensor = Common::Conversions::unpack_tensor<double>(oid);
            write_tensor<double>(tensor);
            break;
        }
        case ObjectSubType::Dictionary: {
            const auto dictionary = Common::Conversions::unpack_dictionary(oid);
            return write_dictionary(*dictionary);
        }
        case ObjectSubType::List: {
            const auto list = MQL::Conversions::unpack_list(oid);
            return write_list(list);
        }
        case ObjectSubType::StringXsd:
        case ObjectSubType::StringLang:
        case ObjectSubType::StringDatatype:
        case ObjectSubType::Iri:
        case ObjectSubType::NotFound:
        case ObjectSubType::Invalid:
            assert(false);
            break;
        }
    }
};
} // namespace MDBServer
