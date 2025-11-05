#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "network/server/session/streaming/request/streaming_request_reader.h"
#include "query/exceptions.h"

namespace MDBServer {

class StreamingRdfRequestReader : public StreamingRequestReader {
public:
    ObjectId read_object() override
    {
        const auto type = read_datatype();
        switch (type) {
        case Protocol::DataType::NULL_:
            return ObjectId::get_null();
        case Protocol::DataType::BOOL_FALSE:
            return ObjectId(ObjectId::BOOL_FALSE);
        case Protocol::DataType::BOOL_TRUE:
            return ObjectId(ObjectId::BOOL_TRUE);
        case Protocol::DataType::INT64: {
            const auto i = read_int64();
            return SPARQL::Conversions::pack_int(i);
        }
        case Protocol::DataType::FLOAT: {
            const auto f = read_float();
            return SPARQL::Conversions::pack_float(f);
        }
        case Protocol::DataType::STRING: {
            const auto str = read_string();
            return SPARQL::Conversions::pack_string_simple(str);
        }
        case Protocol::DataType::STRING_LANG: {
            const auto str = read_string();
            const auto lang = read_string();
            return SPARQL::Conversions::pack_string_lang(lang, str);
        }
        case Protocol::DataType::STRING_DATATYPE: {
            const auto str = read_string();
            const auto datatype = read_string();
            return SPARQL::Conversions::pack_string_datatype(datatype, str);
        }
        case Protocol::DataType::IRI: {
            const auto str = read_string();
            return SPARQL::Conversions::pack_iri(str);
        }
        case Protocol::DataType::ANON: {
            const auto anon_id = read_int64();
            return SPARQL::Conversions::pack_blank_tmp(anon_id);
        }
        case Protocol::DataType::TENSOR: {
            const auto tensor_type = read_datatype();
            switch (tensor_type) {
            case Protocol::DataType::FLOAT: {
                const auto tensor = read_tensor<float>();
                return Common::Conversions::pack_tensor(tensor);
            }
            case Protocol::DataType::DOUBLE: {
                const auto tensor = read_tensor<double>();
                return Common::Conversions::pack_tensor(tensor);
            }
            default: {
                throw QueryException(
                    "Unsupported tensor datatype received as parameter: "
                    + Protocol::datatype_to_string(tensor_type)
                );
            }
            }
        }
        default:
            throw QueryException(
                "Unsupported datatype received as parameter: " + Protocol::datatype_to_string(type)
            );
        };
    }
};

} // namespace MDBServer
