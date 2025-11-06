#pragma once

#include "streaming_response_writer.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_catalog.h"
#include "graph_models/rdf_model/rdf_object_id.h"

namespace MDBServer {

class StreamingRdfResponseWriter : public StreamingResponseWriter {
public:
    StreamingRdfResponseWriter(StreamingSession& session) : StreamingResponseWriter(session) { }

    uint64_t get_model_id() const override {
        return RdfCatalog::MODEL_ID;
    }

    uint64_t get_catalog_version() const override {
        return RdfCatalog::MAJOR_VERSION;
    }

    void write_string_lang(const std::string& str, const std::string& lang)
    {
        write_typed_string(str, Protocol::DataType::STRING_LANG);
        write_string_raw(lang);
    }

    void write_string_datatype(const std::string& str, const std::string& datatype)
    {
        write_typed_string(str, Protocol::DataType::STRING_DATATYPE);
        write_string_raw(datatype);
    }

    void write_object_id(const ObjectId& oid) override
    {
        const auto type  = RDF_OID::get_type(oid);
        const auto value = oid.get_value();
        switch (type) {
        case RDF_OID::Type::BLANK_INLINED:
        case RDF_OID::Type::BLANK_TMP: {
            const auto blank_id = SPARQL::Conversions::unpack_blank(oid);
            write_anon(blank_id);
            break;
        }
        case RDF_OID::Type::STRING_SIMPLE_INLINE:
        case RDF_OID::Type::STRING_SIMPLE_EXTERN:
        case RDF_OID::Type::STRING_SIMPLE_TMP:
        case RDF_OID::Type::STRING_XSD_INLINE:
        case RDF_OID::Type::STRING_XSD_EXTERN:
        case RDF_OID::Type::STRING_XSD_TMP: {
            const auto str = SPARQL::Conversions::unpack_string(oid);
            write_typed_string(str, Protocol::DataType::STRING);
            break;
        }
        case RDF_OID::Type::INT56_INLINE:
        case RDF_OID::Type::INT64_EXTERN:
        case RDF_OID::Type::INT64_TMP: {
            const int64_t i = SPARQL::Conversions::unpack_int(oid);
            write_int64(i);
            break;
        }
        case RDF_OID::Type::FLOAT32: {
            const float f = SPARQL::Conversions::unpack_float(oid);
            write_float(f);
            break;
        }
        case RDF_OID::Type::DOUBLE64_EXTERN:
        case RDF_OID::Type::DOUBLE64_TMP: {
            const double d = SPARQL::Conversions::unpack_double(oid);
            write_double(d);
            break;
        }
        case RDF_OID::Type::BOOL: {
            const auto b = SPARQL::Conversions::unpack_bool(oid);
            write_bool(b);
            break;
        }
        case RDF_OID::Type::PATH: {
            write_path(value);
            break;
        }
        case RDF_OID::Type::IRI_INLINE:
        case RDF_OID::Type::IRI_INLINE_INT_SUFFIX:
        case RDF_OID::Type::IRI_EXTERN:
        case RDF_OID::Type::IRI_TMP: {
            const auto iri = SPARQL::Conversions::unpack_iri(oid);
            write_typed_string(iri, Protocol::DataType::IRI);
            break;
        }
        case RDF_OID::Type::STRING_DATATYPE_INLINE:
        case RDF_OID::Type::STRING_DATATYPE_EXTERN:
        case RDF_OID::Type::STRING_DATATYPE_TMP: {
            const auto&& [datatype, str] = SPARQL::Conversions::unpack_string_datatype(oid);
            write_string_datatype(str, datatype);
            break;
        }
        case RDF_OID::Type::STRING_LANG_INLINE:
        case RDF_OID::Type::STRING_LANG_EXTERN:
        case RDF_OID::Type::STRING_LANG_TMP: {
            const auto&& [lang, str] = SPARQL::Conversions::unpack_string_lang(oid);
            write_string_lang(str, lang);
            break;
        }
        case RDF_OID::Type::DATE: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            write_date(datetime);
            break;
        }
        case RDF_OID::Type::TIME: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            write_time(datetime);
            break;
        }
        case RDF_OID::Type::DATETIME:
        case RDF_OID::Type::DATETIMESTAMP: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            write_datetime(datetime);
            break;
        }
        case RDF_OID::Type::DECIMAL_INLINE:
        case RDF_OID::Type::DECIMAL_EXTERN:
        case RDF_OID::Type::DECIMAL_TMP: {
            const Decimal dec = SPARQL::Conversions::unpack_decimal(oid);
            write_typed_string(dec.to_string(), Protocol::DataType::DECIMAL);
            break;
        }
        case RDF_OID::Type::NULL_ID: {
            write_null();
            break;
        }
        case RDF_OID::Type::TENSOR_FLOAT_INLINE:
        case RDF_OID::Type::TENSOR_FLOAT_EXTERN:
        case RDF_OID::Type::TENSOR_FLOAT_TMP: {
            const auto tensor = Common::Conversions::unpack_tensor<float>(oid);
            write_tensor<float>(tensor);
            break;
        }
        case RDF_OID::Type::TENSOR_DOUBLE_INLINE:
        case RDF_OID::Type::TENSOR_DOUBLE_EXTERN:
        case RDF_OID::Type::TENSOR_DOUBLE_TMP: {
            const auto tensor = Common::Conversions::unpack_tensor<double>(oid);
            write_tensor<double>(tensor);
            break;
        }
        default:
            throw std::logic_error("Unmanaged type in RdfResponseWriter::encode_object_id: "
                                   + std::to_string(static_cast<uint8_t>(type)));
        }
    }
};
} // namespace MDBServer
