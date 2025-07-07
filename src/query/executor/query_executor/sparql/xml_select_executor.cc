#include "xml_select_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "system/path_manager.h"
#include "query/executor/query_executor/xml_ostream_escape.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

uint64_t XMLSelectExecutor::execute_empty_binding(std::ostream& os) {
    uint64_t result_count = 0;
    os << "<head></head>";
    os << "<results>";
    while (root->next()) {
        result_count++;
        os << "<result></result>";
    }
    os << "</results>";
    os << "</sparql>";
    return result_count;
}


uint64_t XMLSelectExecutor::execute(std::ostream& os) {
    XMLOstreamEscape xml_ostream_escape(os);
    std::ostream escaped_os(&xml_ostream_escape);

    uint64_t result_count = 0;
    binding = std::make_unique<Binding>(get_query_ctx().get_var_size());
    root->begin(*binding);

    os << "<?xml version=\"1.0\"?>";
    os << "<sparql xmlns=\"http://www.w3.org/2005/sparql-results#\">";

    auto it = projection_vars.cbegin();
    // Executes queries with empty projection differently, so we can assume later the projection is not empty
    if (it == projection_vars.cend()) {
        return execute_empty_binding(os);
    }

    // print header
    os << "<head>";
    do {
        os << "<variable name=\"" << get_query_ctx().get_var_name(*it) << "\"/>";
    } while (++it != projection_vars.cend());
    os << "</head>";

    os << "<results>";
    while (root->next()) {
        result_count++;
        os << "<result>";
        for (it = projection_vars.cbegin(); it != projection_vars.cend(); ++it) {
            auto value = (*binding)[*it];
            if (!value.is_null()) {
                os << "<binding name=\"" << get_query_ctx().get_var_name(*it) << "\">";
                print(os, escaped_os, value);
                os << "</binding>";
            }
        }
        os << "</result>";
    }
    os << "</results>";
    os << "</sparql>";
    return result_count;
}


void XMLSelectExecutor::print_path_node(std::ostream& os, ObjectId node_id) {
    XMLOstreamEscape xml_ostream_escape(os);
    std::ostream escaped_os(&xml_ostream_escape);

    os << "<node>";
    print(os, escaped_os, node_id);
    os << "</node>";
}


void XMLSelectExecutor::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse) {
    os << "<edge inverse=" << (inverse ? "true" : "false") << ">";
    print(os, os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << "</edge>";
}


void XMLSelectExecutor::print(std::ostream& os, std::ostream& escaped_os, ObjectId oid) {
    switch (RDF_OID::get_type(oid)) {
    case RDF_OID::Type::BLANK_INLINED: {
        os << "<bnode>b"
           << Conversions::unpack_blank(oid)
           << "</bnode>";
        break;
    }
    case RDF_OID::Type::BLANK_TMP: {
        os << "<bnode>c"
           << Conversions::unpack_blank(oid)
           << "</bnode>";
        break;
    }
    case RDF_OID::Type::STRING_SIMPLE_INLINE:
    case RDF_OID::Type::STRING_SIMPLE_EXTERN:
    case RDF_OID::Type::STRING_SIMPLE_TMP: {
        os << "<literal>";
        Conversions::print_string(oid, escaped_os);
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::STRING_XSD_INLINE:
    case RDF_OID::Type::STRING_XSD_EXTERN:
    case RDF_OID::Type::STRING_XSD_TMP:{
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#string\">";
        Conversions::print_string(oid, escaped_os);
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::INT56_INLINE:
    case RDF_OID::Type::INT64_EXTERN:
    case RDF_OID::Type::INT64_TMP: {
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#integer\">";
        os << Conversions::unpack_int(oid);
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::FLOAT32: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#float\">";
        os << float_buffer;
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::DOUBLE64_EXTERN:
    case RDF_OID::Type::DOUBLE64_TMP: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#float\">";
        os << double_buffer;
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::BOOL: {
        os << "<boolean>";
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        os << "</boolean>";
        break;
    }
    case RDF_OID::Type::PATH: {
        using namespace std::placeholders;
        os << "<path>";
        path_manager.print(os,
                           Conversions::get_path_id(oid),
                           std::bind(&XMLSelectExecutor::print_path_node, _1, _2),
                           std::bind(&XMLSelectExecutor::print_path_edge, _1, _2, _3));
        os << "</path>";
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
        os << "<uri>";
        Conversions::print_iri(oid, os);
        os << "</uri>";
        break;
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE:
    case RDF_OID::Type::STRING_DATATYPE_EXTERN:
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        auto&& [dtt, str] = Conversions::unpack_string_datatype(oid);

        os << "<literal datatype=\"";
        os << dtt;
        os << "\">";
        escaped_os << str;
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::STRING_LANG_INLINE:
    case RDF_OID::Type::STRING_LANG_EXTERN:
    case RDF_OID::Type::STRING_LANG_TMP: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);

        os << "<literal xml:lang=\"";
        os << lang;
        os << "\">";
        escaped_os << str;
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::DATE:
    case RDF_OID::Type::DATETIME:
    case RDF_OID::Type::TIME:
    case RDF_OID::Type::DATETIMESTAMP: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << "<literal datatype=\"" << datetime.get_datatype_string() << "\">";
        os << datetime.get_value_string();
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::DECIMAL_INLINE:
    case RDF_OID::Type::DECIMAL_EXTERN:
    case RDF_OID::Type::DECIMAL_TMP: {
        auto decimal = Conversions::unpack_decimal(oid);

        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#decimal\">";
        os << decimal;
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::TENSOR_FLOAT_INLINE:
    case RDF_OID::Type::TENSOR_FLOAT_EXTERN:
    case RDF_OID::Type::TENSOR_FLOAT_TMP: {
        os << "<literal datatype=\"" << MDBExtensions::Type::TENSOR_FLOAT_IRI << "\">";
        os << Conversions::unpack_tensor<float>(oid);
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::TENSOR_DOUBLE_INLINE:
    case RDF_OID::Type::TENSOR_DOUBLE_EXTERN:
    case RDF_OID::Type::TENSOR_DOUBLE_TMP: {
        os << "<literal datatype=\"" << MDBExtensions::Type::TENSOR_DOUBLE_IRI << "\">";
        os << Conversions::unpack_tensor<double>(oid);
        os << "</literal>";
        break;
    }
    case RDF_OID::Type::NULL_ID: {
        // executor should not call print
        break;
    }
    }
}


void XMLSelectExecutor::analyze(std::ostream& os, bool print_stats, int indent) const {
    os << std::string(indent, ' ');
    os << "XMLSelectExecutor(";
    for (size_t i = 0; i < projection_vars.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << '?' << get_query_ctx().get_var_name(projection_vars[i]);
    }
    os << ")\n";

    root->print(os, indent + 2, print_stats);
}
