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

    void write_object_id(const ObjectId& oid) override
    {
        const auto type = oid.get_type();
        const auto value = oid.get_value();
        switch (type) {
        case ObjectId::MASK_NULL: {
            write_null();
            break;
        }
        case ObjectId::MASK_ANON_INLINED:
        case ObjectId::MASK_ANON_TMP: {
            const auto anon_id = MQL::Conversions::unpack_blank(oid);
            write_anon(anon_id);
            break;
        }
        case ObjectId::MASK_NAMED_NODE_INLINED:
        case ObjectId::MASK_NAMED_NODE_EXTERN:
        case ObjectId::MASK_NAMED_NODE_TMP: {
            const auto str = MQL::Conversions::unpack_named_node(oid);
            write_typed_string(str, Protocol::DataType::NAMED_NODE);
            break;
        }
        case ObjectId::MASK_STRING_SIMPLE_INLINED:
        case ObjectId::MASK_STRING_SIMPLE_EXTERN:
        case ObjectId::MASK_STRING_SIMPLE_TMP: {
            const auto str = MQL::Conversions::unpack_string(oid);
            write_typed_string(str, Protocol::DataType::STRING);
            break;
        }
        case ObjectId::MASK_NEGATIVE_INT:
        case ObjectId::MASK_POSITIVE_INT: {
            const int64_t i = MQL::Conversions::unpack_int(oid);
            write_int64(i);
            break;
        }
        case ObjectId::MASK_DECIMAL_INLINED:
        case ObjectId::MASK_DECIMAL_EXTERN:
        case ObjectId::MASK_DECIMAL_TMP: {
            const Decimal dec = SPARQL::Conversions::unpack_decimal(oid);
            write_typed_string(dec.to_string(), Protocol::DataType::DECIMAL);
            break;
        }
        case ObjectId::MASK_FLOAT: {
            const float f = MQL::Conversions::unpack_float(oid);
            write_float(f);
            break;
        }
        case ObjectId::MASK_BOOL: {
            write_bool(value != 0);
            break;
        }
        case ObjectId::MASK_EDGE: {
            const auto edge_id = MQL::Conversions::unpack_edge(oid);
            write_edge(edge_id);
            break;
        }
        case ObjectId::MASK_DT_DATE: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            write_date(datetime);
            break;
        }
        case ObjectId::MASK_DT_TIME: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            write_time(datetime);
            break;
        }
        case ObjectId::MASK_DT_DATETIME:
        case ObjectId::MASK_DT_DATETIMESTAMP: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            write_datetime(datetime);
            break;
        }
        case ObjectId::MASK_PATH: {
            write_path(value);
            break;
        }
        case ObjectId::MASK_TENSOR_FLOAT_INLINED:
        case ObjectId::MASK_TENSOR_FLOAT_EXTERN:
        case ObjectId::MASK_TENSOR_FLOAT_TMP: {
            const auto tensor = Common::Conversions::unpack_tensor<float>(oid);
            write_tensor<float>(tensor);
            break;
        }
        case ObjectId::MASK_TENSOR_DOUBLE_INLINED:
        case ObjectId::MASK_TENSOR_DOUBLE_EXTERN:
        case ObjectId::MASK_TENSOR_DOUBLE_TMP: {
            const auto tensor = Common::Conversions::unpack_tensor<double>(oid);
            write_tensor<double>(tensor);
            break;
        }
        case ObjectId::MASK_DICTIONARY:
        case ObjectId::MASK_DICTIONARY_TMP: {
            const auto dictionary = Common::Conversions::unpack_dictionary(oid);
            return write_dictionary(*dictionary);
        }
        case ObjectId::MASK_LIST:
        case ObjectId::MASK_LIST_EXTERN:
        case ObjectId::MASK_LIST_TMP: {
            const auto list = MQL::Conversions::unpack_list(oid);
            return write_list(list);
        }
        default:
            throw std::logic_error(
                "Unmanaged type in StreamingQuadResponseWriter::encode_object_id: " + std::to_string(type)
            );
        }
    }
};
} // namespace MDBServer
