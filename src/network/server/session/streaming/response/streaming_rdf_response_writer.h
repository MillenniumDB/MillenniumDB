#pragma once

#include "streaming_response_writer.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_catalog.h"

namespace MDBServer {

class StreamingRdfResponseWriter : public StreamingResponseWriter {
public:
    StreamingRdfResponseWriter(StreamingSession& session) :
        StreamingResponseWriter(session)
    { }

    uint64_t get_model_id() const override
    {
        return RdfCatalog::MODEL_ID;
    }

    uint64_t get_catalog_version() const override
    {
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

    void write_object_id(ObjectId oid) override
    {
        // const auto type = oid.type();
        const auto value = oid.get_value();
        switch (oid.subtype()) {
        case ObjectSubType::Anon: {
            const auto blank_id = SPARQL::Conversions::unpack_blank(oid);
            write_anon(blank_id);
            break;
        }
        case ObjectSubType::String:
        case ObjectSubType::StringXsd: {
            const auto str = SPARQL::Conversions::unpack_string(oid);
            write_typed_string(str, Protocol::DataType::STRING);
            break;
        }
        case ObjectSubType::Int: {
            const int64_t i = SPARQL::Conversions::unpack_int(oid);
            write_int64(i);
            break;
        }
        case ObjectSubType::Float: {
            const float f = SPARQL::Conversions::unpack_float(oid);
            write_float(f);
            break;
        }
        case ObjectSubType::Double: {
            const double d = SPARQL::Conversions::unpack_double(oid);
            write_double(d);
            break;
        }
        case ObjectSubType::Bool: {
            const auto b = SPARQL::Conversions::unpack_bool(oid);
            write_bool(b);
            break;
        }
        case ObjectSubType::Path: {
            write_path(value);
            break;
        }
        case ObjectSubType::Iri: {
            const auto iri = SPARQL::Conversions::unpack_iri(oid);
            write_typed_string(iri, Protocol::DataType::IRI);
            break;
        }
        case ObjectSubType::StringDatatype: {
            const auto&& [datatype, str] = SPARQL::Conversions::unpack_string_datatype(oid);
            write_string_datatype(str, datatype);
            break;
        }
        case ObjectSubType::StringLang: {
            const auto&& [lang, str] = SPARQL::Conversions::unpack_string_lang(oid);
            write_string_lang(str, lang);
            break;
        }
        case ObjectSubType::TemporalLiteral: {
            const DateTime datetime = SPARQL::Conversions::unpack_date(oid);
            write_datetime(datetime);
            break;
        }
        case ObjectSubType::Decimal: {
            const Decimal dec = SPARQL::Conversions::unpack_decimal(oid);
            write_typed_string(dec.to_string(), Protocol::DataType::DECIMAL);
            break;
        }
        case ObjectSubType::Null: {
            write_null();
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
        case ObjectSubType::NamedNode:
        case ObjectSubType::Dictionary:
        case ObjectSubType::List:
        case ObjectSubType::Edge:
        case ObjectSubType::NotFound:
        case ObjectSubType::Invalid:
            assert(false);
            break;
        }
    }
};
} // namespace MDBServer
