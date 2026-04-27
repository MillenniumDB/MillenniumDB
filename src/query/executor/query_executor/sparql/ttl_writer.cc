#include "ttl_writer.h"

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

void escape(std::ostream& os, const std::string& string) {
    for (auto ch : string) {
        switch (ch) {
        case '\t': os << '\\'; os << 't';  break;
        case '\b': os << '\\'; os << 'b';  break;
        case '\n': os << '\\'; os << 'n';  break;
        case '\r': os << '\\'; os << 'e';  break;
        case '\f': os << '\\'; os << 'f';  break;
        case '\"': os << '\\'; os << '"';  break;
        case '\'': os << '\\'; os << '\''; break;
        case '\\': os << '\\'; os << '\\'; break;
        default: os << ch;
        }
    }
}

void write_and_escape_ttl(std::ostream& os, ObjectId oid) {
    switch (oid.subtype()) {
    case ObjectSubType::Null: {
        os << "null";
        break;
    }
    case ObjectSubType::Anon: {
        if (oid.type() == ObjectType::AnonInl) {
            os << "_:b";
        } else {
            os << "_:c";
        }
        os << Conversions::unpack_blank(oid);
        break;
    }
    case ObjectSubType::Iri: {
        os << '<';
        escape(os, Conversions::unpack_iri(oid));
        os << '>';
        break;
    }
    case ObjectSubType::String: {
        os << '"';
        escape(os, Conversions::unpack_string(oid));
        os << '"';
        break;
    }
    case ObjectSubType::StringXsd:{
        os << '"';
        escape(os, Conversions::unpack_string(oid));
        os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        break;
    }
    case ObjectSubType::StringDatatype: {
        auto&& [dtt, str] = Conversions::unpack_string_datatype(oid);

        os << '"';
        escape(os, str);
        os << "\"^^<";
        os << dtt;
        os << '>';
        break;
    }
    case ObjectSubType::StringLang: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);
        os << '"';
        escape(os, str);
        os << "\"@";
        os << lang;
        break;
    }
    case ObjectSubType::Int: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case ObjectSubType::Decimal: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case ObjectSubType::Float: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << '"' << float_buffer <<"\"^^<http://www.w3.org/2001/XMLSchema#float>";
        break;
    }
    case ObjectSubType::Double: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case ObjectSubType::TemporalLiteral: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case ObjectSubType::Bool: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        break;
    }
    case ObjectSubType::TensorFloat: {
        os << '"' << Conversions::unpack_tensor<float>(oid);
        os << "\"^^<" << MDBExtensions::Type::TENSOR_FLOAT_IRI << ">";
        break;
    }
    case ObjectSubType::TensorDouble: {
        os << '"' << Conversions::unpack_tensor<double>(oid);
        os << "\"^^<" << MDBExtensions::Type::TENSOR_DOUBLE_IRI << ">";
        break;
    }
    case ObjectSubType::Path:
        // paths are not defined in TTL
    case ObjectSubType::Dictionary:
    case ObjectSubType::List:
    case ObjectSubType::NamedNode:
    case ObjectSubType::Edge:
    case ObjectSubType::PGMetaData:
    case ObjectSubType::NotFound:
        break;
    }
}
