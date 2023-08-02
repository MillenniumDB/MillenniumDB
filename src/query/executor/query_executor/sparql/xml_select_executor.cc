#include "xml_select_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "graph_models/rdf_model/datatypes/decimal_inlined.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/query_executor/xml_ostream_escape.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"
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

void print_datatype_rdf_xml(std::ostream& os, uint64_t datatype_id) {
    if ((datatype_id & ObjectId::MASK_TAG_MANAGER) == 0) {
        os << rdf_model.catalog().datatypes[datatype_id];
    } else {
        tmp_manager.print_dtt(os, (datatype_id & (~ObjectId::MASK_TAG_MANAGER)));
    }
}


void print_language_rdf_xml(std::ostream& os, uint64_t language_id) {
    if ((language_id & ObjectId::MASK_TAG_MANAGER) == 0) {
        os << rdf_model.catalog().languages[language_id];
    } else {
        tmp_manager.print_lan(os, (language_id & (~ObjectId::MASK_TAG_MANAGER)));
    }
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


void XMLSelectExecutor::print(std::ostream& os, std::ostream& escaped_os, ObjectId object_id) {
    const auto mask        = object_id.id & ObjectId::TYPE_MASK;
    const auto unmasked_id = object_id.id & ObjectId::VALUE_MASK;
    switch (mask) {
    case ObjectId::MASK_ANON_INLINED: {
        os << "<bnode>b"
           << unmasked_id
           << "</bnode>";
        break;
    }
    case ObjectId::MASK_ANON_TMP: {
        os << "<bnode>c"
           << unmasked_id
           << "</bnode>";
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        os << "<literal>";
        string_manager.print(escaped_os, unmasked_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        os << "<literal>";
        Inliner::print_string_inlined<7>(escaped_os, unmasked_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        os << "<literal>";
        tmp_manager.print_str(escaped_os, unmasked_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_POSITIVE_INT: {
        int64_t i = unmasked_id;
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#integer\">";
        os << i;
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_NEGATIVE_INT: {
        int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#integer\">";
        os << (i*-1);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_FLOAT: {
        float f;
        uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
        dest[0] =  object_id.id        & 0xFF;
        dest[1] = (object_id.id >> 8)  & 0xFF;
        dest[2] = (object_id.id >> 16) & 0xFF;
        dest[3] = (object_id.id >> 24) & 0xFF;

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#float\">";
        os << float_buffer;
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_DOUBLE_EXTERN:
    case ObjectId::MASK_DOUBLE_TMP: {
        double d = Conversions::unpack_double(object_id);
        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#float\">";
        os << double_buffer;
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_BOOL: {
        os << "<boolean>";
        os << (object_id.get_value() == 0 ? "false" : "true");
        os << "</boolean>";
        break;
    }
    case ObjectId::MASK_PATH: {
        os << "<path>";
        path_manager.print(os, unmasked_id, &print_path_node, &print_path_edge);
        os << "</path>";
        break;
    }
    case ObjectId::MASK_IRI_EXTERN: {
        uint64_t iri_id = unmasked_id & ObjectId::MASK_IRI_CONTENT;
        uint8_t prefix_id = (unmasked_id & ObjectId::MASK_IRI_PREFIX) >> 48;

        os << "<uri>";
        os << rdf_model.catalog().prefixes[prefix_id];
        string_manager.print(os, iri_id);
        os << "</uri>";
        break;
    }
    case ObjectId::MASK_IRI_INLINED: {
        uint8_t prefix_id = (object_id.id & ObjectId::MASK_IRI_PREFIX) >> (8*ObjectId::IRI_INLINE_BYTES);
        os << "<uri>";
        os << rdf_model.catalog().prefixes[prefix_id];
        Inliner::print_string_inlined<6>(os, unmasked_id);
        os << "</uri>";
        break;
    }
    case ObjectId::MASK_IRI_TMP: {
        uint8_t prefix_id = (unmasked_id & ObjectId::MASK_IRI_PREFIX) >> 48;
        uint64_t iri_id = unmasked_id & ObjectId::MASK_IRI_CONTENT;

        os << "<uri>";
        os << rdf_model.catalog().prefixes[prefix_id];
        tmp_manager.print_str(os, iri_id); // gets string from id
        os << "</uri>";
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_INLINED: {
        int prefix_shift_size = 8 * ObjectId::STR_DT_INLINE_BYTES;
        uint16_t datatype_id = (object_id.id & ObjectId::MASK_LITERAL_TAG) >> prefix_shift_size;

        os << "<literal datatype=\"";
        print_datatype_rdf_xml(os, datatype_id);
        os << "\">";
        Inliner::print_string_inlined<5>(escaped_os, unmasked_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_EXTERN: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint16_t datatype_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        os << "<literal datatype=\"";
        print_datatype_rdf_xml(os, datatype_id);
        os << "\">";
        string_manager.print(escaped_os, str_id);
        os << "</literal>";

        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_TMP: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint64_t datatype_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        os << "<literal datatype=\"";
        print_datatype_rdf_xml(os, datatype_id);
        os << "\">";
        tmp_manager.print_str(escaped_os, str_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_STRING_LANG_INLINED: {
        int prefix_shift_size = 8 * ObjectId::STR_LANG_INLINE_BYTES;
        uint16_t language_id = (object_id.id & ObjectId::MASK_LITERAL_TAG) >> prefix_shift_size;

        os << "<literal xml:lang=\"";
        print_language_rdf_xml(os, language_id);
        os << "\">";
        Inliner::print_string_inlined<5>(escaped_os, unmasked_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_STRING_LANG_EXTERN: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint16_t language_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        os << "<literal xml:lang=\"";
        print_language_rdf_xml(os, language_id);
        os << "\">";
        string_manager.print(escaped_os, str_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_STRING_LANG_TMP: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint64_t language_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        os << "<literal xml:lang=\"";
        print_language_rdf_xml(os, language_id);
        os << "\">";

        tmp_manager.print_str(escaped_os, str_id);
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_DT_DATE:
    case ObjectId::MASK_DT_DATETIME:
    case ObjectId::MASK_DT_TIME:
    case ObjectId::MASK_DT_DATETIMESTAMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        DateTime datetime(object_id);
        os << datetime.get_value_string();
        os << "\",\"datatype\":\"";
        os << datetime.get_datatype_string() << "\"}";
        break;
    }
    case ObjectId::MASK_DECIMAL_EXTERN: {
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#decimal\">";
        std::stringstream ss;
        string_manager.print(ss, unmasked_id);
        os << Decimal::from_external(ss.str());
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_DECIMAL_INLINED: {
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#decimal\">";
        DecimalInlined decimal_inlined(unmasked_id);
        os << decimal_inlined.get_value_string();
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_DECIMAL_TMP: {
        os << "<literal datatype=\"http://www.w3.org/2001/XMLSchema#decimal\">";
        std::ostringstream ss;
        tmp_manager.print_str(ss, unmasked_id);
        os << Decimal::from_external(ss.str());
        os << "</literal>";
        break;
    }
    case ObjectId::MASK_NULL: {
        // executor should not call print with NULL
        break;
    }

    default:
        throw std::logic_error("Unmanaged mask in XMLSelectExecutor::print: "
            + std::to_string(mask));
    }
}


void XMLSelectExecutor::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "XMLSelectExecutor(";
    for (size_t i = 0; i < projection_vars.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << '?' << get_query_ctx().get_var_name(projection_vars[i]);
    }
    os << ")\n";
    root->analyze(os, indent + 2);
}
