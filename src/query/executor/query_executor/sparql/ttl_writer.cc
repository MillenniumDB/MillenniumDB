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
    switch (RDF_OID::get_type(oid)) {
    case RDF_OID::Type::NULL_ID: {
        os << "null";
        break;
    }
    case RDF_OID::Type::BLANK_INLINED: {
        os << "_:b";
        os << Conversions::unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::BLANK_TMP: {
        os << "_:c";
        os << Conversions::unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::IRI_INLINE:
    case RDF_OID::Type::IRI_INLINE_INT_SUFFIX:
    case RDF_OID::Type::IRI_EXTERN:
    case RDF_OID::Type::IRI_TMP:
    case RDF_OID::Type::IRI_UUID_LOWER:
    case RDF_OID::Type::IRI_UUID_LOWER_TMP:
    case RDF_OID::Type::IRI_UUID_UPPER:
    case RDF_OID::Type::IRI_UUID_UPPER_TMP:
    case RDF_OID::Type::IRI_HEX_LOWER:
    case RDF_OID::Type::IRI_HEX_LOWER_TMP:
    case RDF_OID::Type::IRI_HEX_UPPER:
    case RDF_OID::Type::IRI_HEX_UPPER_TMP: {
        os << '<';
        escape(os, Conversions::unpack_iri(oid));
        os << '>';
        break;
    }
    case RDF_OID::Type::STRING_SIMPLE_INLINE:
    case RDF_OID::Type::STRING_SIMPLE_EXTERN:
    case RDF_OID::Type::STRING_SIMPLE_TMP: {
        os << '"';
        escape(os, Conversions::unpack_string(oid));
        os << '"';
        break;
    }
    case RDF_OID::Type::STRING_XSD_INLINE:
    case RDF_OID::Type::STRING_XSD_EXTERN:
    case RDF_OID::Type::STRING_XSD_TMP:{
        os << '"';
        escape(os, Conversions::unpack_string(oid));
        os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        break;
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE:
    case RDF_OID::Type::STRING_DATATYPE_EXTERN:
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        auto&& [dtt, str] = Conversions::unpack_string_datatype(oid);

        os << '"';
        escape(os, str);
        os << "\"^^<";
        os << dtt;
        os << '>';
        break;
    }
    case RDF_OID::Type::STRING_LANG_INLINE:
    case RDF_OID::Type::STRING_LANG_EXTERN:
    case RDF_OID::Type::STRING_LANG_TMP: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);
        os << '"';
        escape(os, str);
        os << "\"@";
        os << lang;
        break;
    }
    case RDF_OID::Type::INT56_INLINE:
    case RDF_OID::Type::INT64_EXTERN:
    case RDF_OID::Type::INT64_TMP: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case RDF_OID::Type::DECIMAL_INLINE:
    case RDF_OID::Type::DECIMAL_EXTERN:
    case RDF_OID::Type::DECIMAL_TMP: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case RDF_OID::Type::FLOAT32: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << '"' << float_buffer <<"\"^^<http://www.w3.org/2001/XMLSchema#float>";
        break;
    }
    case RDF_OID::Type::DOUBLE64_EXTERN:
    case RDF_OID::Type::DOUBLE64_TMP: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case RDF_OID::Type::DATE:
    case RDF_OID::Type::DATETIME:
    case RDF_OID::Type::TIME:
    case RDF_OID::Type::DATETIMESTAMP: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case RDF_OID::Type::BOOL: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        break;
    }
    case RDF_OID::Type::TENSOR_FLOAT_INLINE:
    case RDF_OID::Type::TENSOR_FLOAT_EXTERN:
    case RDF_OID::Type::TENSOR_FLOAT_TMP: {
        os << '"' << Conversions::unpack_tensor<float>(oid);
        os << "\"^^<" << MDBExtensions::Type::TENSOR_FLOAT_IRI << ">";
        break;
    }
    case RDF_OID::Type::TENSOR_DOUBLE_INLINE:
    case RDF_OID::Type::TENSOR_DOUBLE_EXTERN:
    case RDF_OID::Type::TENSOR_DOUBLE_TMP: {
        os << '"' << Conversions::unpack_tensor<double>(oid);
        os << "\"^^<" << MDBExtensions::Type::TENSOR_DOUBLE_IRI << ">";
        break;
    }
    case RDF_OID::Type::PATH:
        // paths are not defined in TTL
        break;
    }
}
