#pragma once

#include "streaming_response_writer.h"

#include "graph_models/rdf_model/rdf_model.h"
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
        return RdfCatalog::VERSION;
    }

    std::string encode_string_lang(const std::string& str, const std::string& lang) const {
        std::string res;
        res += static_cast<char>(Protocol::DataType::STRING_LANG);
        res += encode_size(str.size());
        res += str;
        res += encode_size(lang.size());
        res += lang;
        return res;
    }

    std::string encode_string_datatype(const std::string& str, const std::string& datatype) const {
        std::string res;
        res += static_cast<char>(Protocol::DataType::STRING_DATATYPE);
        res += encode_size(str.size());
        res += str;
        res += encode_size(datatype.size());
        res += datatype;
        return res;
    }

    std::string encode_object_id(const ObjectId& oid) const override {
        const auto type  = RDF_OID::get_type(oid);
        const auto value = oid.get_value();
        switch (type) {
        case RDF_OID::Type::BLANK_INLINED: {
            return encode_string("_:b" + std::to_string(value), Protocol::DataType::ANON);
        }
        case RDF_OID::Type::BLANK_TMP: {
            return encode_string("_:c" + std::to_string(value), Protocol::DataType::ANON);
        }
        case RDF_OID::Type::STRING_SIMPLE_INLINE:
        case RDF_OID::Type::STRING_SIMPLE_EXTERN:
        case RDF_OID::Type::STRING_SIMPLE_TMP:
        case RDF_OID::Type::STRING_XSD_INLINE:
        case RDF_OID::Type::STRING_XSD_EXTERN:
        case RDF_OID::Type::STRING_XSD_TMP: {
            const auto str = SPARQL::Conversions::unpack_string(oid);
            return encode_string(str, Protocol::DataType::STRING);
        }
        case RDF_OID::Type::INT56_INLINE:
        case RDF_OID::Type::INT64_EXTERN:
        case RDF_OID::Type::INT64_TMP: {
            const int64_t i = SPARQL::Conversions::unpack_int(oid);
            return encode_int64(i);
        }
        case RDF_OID::Type::FLOAT32: {
            const float f = SPARQL::Conversions::unpack_float(oid);
            return encode_float(f);
        }
        case RDF_OID::Type::DOUBLE64_EXTERN:
        case RDF_OID::Type::DOUBLE64_TMP: {
            const double d = SPARQL::Conversions::unpack_double(oid);
            return encode_double(d);
        }
        case RDF_OID::Type::BOOL: {
            const auto b = SPARQL::Conversions::unpack_bool(oid);
            return encode_bool(b);
        }
        case RDF_OID::Type::PATH: {
            return encode_path(value);
        }
        case RDF_OID::Type::IRI_INLINE:
        case RDF_OID::Type::IRI_INLINE_INT_SUFFIX:
        case RDF_OID::Type::IRI_EXTERN:
        case RDF_OID::Type::IRI_TMP: {
            const auto iri = SPARQL::Conversions::unpack_iri(oid);
            return encode_string(iri, Protocol::DataType::IRI);
        }
        case RDF_OID::Type::STRING_DATATYPE_INLINE:
        case RDF_OID::Type::STRING_DATATYPE_EXTERN:
        case RDF_OID::Type::STRING_DATATYPE_TMP: {
            const auto&& [datatype, str] = SPARQL::Conversions::unpack_string_datatype(oid);
            return encode_string_datatype(str, datatype);
        }
        case RDF_OID::Type::STRING_LANG_INLINE:
        case RDF_OID::Type::STRING_LANG_EXTERN:
        case RDF_OID::Type::STRING_LANG_TMP: {
            const auto&& [lang, str] = SPARQL::Conversions::unpack_string_lang(oid);
            return encode_string_lang(str, lang);
        }
        case RDF_OID::Type::DATE: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            return encode_date(datetime);
        }
        case RDF_OID::Type::TIME: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            return encode_time(datetime);
        }
        case RDF_OID::Type::DATETIME:
        case RDF_OID::Type::DATETIMESTAMP: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            return encode_datetime(datetime);
        }
        case RDF_OID::Type::DECIMAL_INLINE:
        case RDF_OID::Type::DECIMAL_EXTERN:
        case RDF_OID::Type::DECIMAL_TMP: {
            const Decimal dec = SPARQL::Conversions::unpack_decimal(oid);
            return encode_string(dec.to_string(), Protocol::DataType::DECIMAL);
        }
        case RDF_OID::Type::NULL_ID: {
            return encode_null();
        }
        default:
            throw std::logic_error("Unmanaged type in RdfResponseWriter::encode_object_id: "
                                   + std::to_string(static_cast<uint8_t>(type)));
        }
    }
};
} // namespace MDBServer
