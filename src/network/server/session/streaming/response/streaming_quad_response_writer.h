#include "streaming_response_writer.h"

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_catalog.h"

namespace MDBServer {

class StreamingQuadResponseWriter : public StreamingResponseWriter {
public:
    StreamingQuadResponseWriter(StreamingSession& session) : StreamingResponseWriter(session) { }

    uint64_t get_model_id() const override {
        return QuadCatalog::MODEL_ID;
    }

    uint64_t get_catalog_version() const override {
        return QuadCatalog::VERSION;
    }

    std::string encode_object_id(const ObjectId& oid) const override {
        const auto type  = oid.get_type();
        const auto value = oid.get_value();
        switch (type) {
        case ObjectId::MASK_NULL: {
            return encode_null();
        }
        case ObjectId::MASK_ANON_INLINED: {
            const auto anon_id = MQL::Conversions::unpack_blank(oid);
            return encode_string("_b" + std::to_string(anon_id), Protocol::DataType::ANON);
        }
        case ObjectId::MASK_ANON_TMP: {
            const auto anon_id = MQL::Conversions::unpack_blank(oid);
            return encode_string("_c" + std::to_string(anon_id), Protocol::DataType::ANON);
        }
        case ObjectId::MASK_NAMED_NODE_INLINED:
        case ObjectId::MASK_NAMED_NODE_EXTERN:
        case ObjectId::MASK_NAMED_NODE_TMP: {
            const auto str = MQL::Conversions::unpack_named_node(oid);
            return encode_string(str, Protocol::DataType::NAMED_NODE);
        }
        case ObjectId::MASK_STRING_SIMPLE_INLINED:
        case ObjectId::MASK_STRING_SIMPLE_EXTERN:
        case ObjectId::MASK_STRING_SIMPLE_TMP: {
            const auto str = MQL::Conversions::unpack_string(oid);
            return encode_string(str, Protocol::DataType::STRING);
        }
        case ObjectId::MASK_NEGATIVE_INT:
        case ObjectId::MASK_POSITIVE_INT: {
            const int64_t i = MQL::Conversions::unpack_int(oid);
            return encode_int64(i);
        }
        case ObjectId::MASK_FLOAT: {
            const float f = MQL::Conversions::unpack_float(oid);
            return encode_float(f);
        }
        case ObjectId::MASK_BOOL: {
            return encode_bool(value != 0);
        }
        case ObjectId::MASK_EDGE: {
            const auto edge_id = MQL::Conversions::unpack_edge(oid);
            return encode_string("_e" + std::to_string(edge_id), Protocol::DataType::EDGE);
        }
        case ObjectId::MASK_DT_DATE: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            return encode_date(datetime);
        }
        case ObjectId::MASK_DT_TIME: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            return encode_time(datetime);
        }
        case ObjectId::MASK_DT_DATETIME:
        case ObjectId::MASK_DT_DATETIMESTAMP: {
            const DateTime datetime = MQL::Conversions::unpack_datetime(oid);
            return encode_datetime(datetime);
        }
        case ObjectId::MASK_PATH: {
            return encode_path(value);
        }
        default:
            throw std::logic_error("Unmanaged type in QuadResponseWriter::encode_object_id: " + std::to_string(type));
        }
    }
};
} // namespace MDBServer
