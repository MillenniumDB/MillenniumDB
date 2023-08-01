#include "ttl_writer.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "storage/unified_storage.h"
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
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_NULL:
        os << "null";
        break;
    case ObjectId::MASK_ANON:
        os << "_:"
           << (oid.get_mod() == ObjectId::MOD_INLINE ? 'b' : 'c')
           << std::to_string(oid.get_value());
        break;
    case ObjectId::MASK_IRI:
        os << '<';
        escape(os, Conversions::unpack_iri(oid));
        os << '>';
        break;
    case ObjectId::MASK_STRING_SIMPLE:
        os << '"';
        escape(os, Conversions::unpack_string_simple(oid));
        os << '"';
        break;
    case ObjectId::MASK_STRING_XSD:
        os << '"';
        escape(os, Conversions::unpack_string_xsd(oid));
        os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        break;
    case ObjectId::MASK_STRING_LANG: {
        auto [lang, str] = Conversions::unpack_string_lang(oid);
        os << '"';
        escape(os, str);
        os << "\"@";
        UnifiedStorage::print_language(os, lang);
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE:{
        auto [datatype, str] = Conversions::unpack_string_datatype(oid);
        os << '"';
        escape(os, str);
        os << "\"^^<";
        UnifiedStorage::print_datatype(os, datatype);
        os << '>';
        break;
    }
    case ObjectId::MASK_INT:
        os << Conversions::unpack_int(oid);
        break;
    case ObjectId::MASK_DECIMAL:
        os << Conversions::unpack_decimal(oid).to_string();
        break;
    case ObjectId::MASK_FLOAT: {
        char buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        auto num = Conversions::unpack_float(oid);
        jkj::dragonbox::to_chars(num, buffer);
        std::string str {buffer};
        os << '"' << buffer <<"\"^^<http://www.w3.org/2001/XMLSchema#float>";
        break;
    }
    case ObjectId::MASK_DOUBLE: {
        char buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        auto num = Conversions::unpack_double(oid);
        jkj::dragonbox::to_chars(num, buffer);
        os << buffer;
        break;
    }
    case ObjectId::MASK_DT_DATE:
    case ObjectId::MASK_DT_DATETIME:
    case ObjectId::MASK_DT_TIME:
    case ObjectId::MASK_DT_DATETIMESTAMP: {
        auto dt = DateTime(oid);
        os << '"' << dt.get_value_string() << "\"^^<" << dt.get_datatype_string() << '>';
        break;
    }
    case ObjectId::MASK_BOOL: {
        if (oid.get_value() == 1) {
            os << "true";
        } else {
            os << "false";
        }
        break;
    }
    default:
        throw NotSupportedException("to_string is not implemented for ObjectId of sub-type " + std::to_string(oid.get_sub_type()));
    }
}