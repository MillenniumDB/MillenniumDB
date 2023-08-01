#include "json_select_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "graph_models/rdf_model/datatypes/decimal_inlined.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/query_executor/json_ostream_escape.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

uint64_t JsonSelectExecutor::execute_empty_binding(std::ostream& os) {
    uint64_t result_count = 0;
    os << "{\"head\":{\"vars\":[]},\"results\":{\"bindings\":[";
    if (root->next()) { // first case without comma
        result_count++;
        os << "{}";
    } else {
        os << "]}}";
        return 0;
    }

    while (root->next()) {
        result_count++;
        os << ",{}";
    }
    os << "]}}";
    return result_count;
}


uint64_t JsonSelectExecutor::execute(std::ostream& os) {
    JsonOstreamEscape json_ostream_escape(os);
    std::ostream escaped_os(&json_ostream_escape);

    uint64_t result_count = 0;
    binding = std::make_unique<Binding>(get_query_ctx().get_var_size());
    root->begin(*binding);

    auto it = projection_vars.cbegin();
    // Executes queries with empty projection differently, so we can assume later the projection is not empty
    if (it == projection_vars.cend()) {
        return execute_empty_binding(os);
    }

    // print header
    os << "{\"head\":{\"vars\":[\"" << get_query_ctx().get_var_name(*it) << '"';
    while (++it != projection_vars.cend()) {
        os << ",\"" << get_query_ctx().get_var_name(*it) << '"';
    }
    os << "]},\"results\":{\"bindings\":[";

    auto sep1 = ""; // first time is empty, then will be a comma
    while (root->next()) {
        result_count++;
        os << sep1 << "{";
        auto sep2 = "\0"; // first time is empty, then will be a comma
        for (auto it = projection_vars.cbegin(); it != projection_vars.cend(); ++it) {
            auto value = (*binding)[*it];
            if (!value.is_null()) {
                os << sep2 << "\"" << get_query_ctx().get_var_name(*it) << "\":";
                print(os, escaped_os, value);
                sep2 = ",";
            }
        }
        os << "}";
        sep1 = ",";
    }
    os << "]}}";
    return result_count;
}


void print_datatype_rdf_json(std::ostream& os, uint64_t datatype_id) {
    os << "\",\"datatype\":\"";
    if ((datatype_id & ObjectId::MASK_TAG_MANAGER) == 0) {
        os << rdf_model.catalog().datatypes[datatype_id];
    } else {
        tmp_manager.print_dtt(os, (datatype_id & (~ObjectId::MASK_TAG_MANAGER)));
    }
    os << "\"}";
}


void print_language_rdf_json(std::ostream& os, uint64_t language_id) {
    os << "\",\"xml:lang\":\"";
    if ((language_id & ObjectId::MASK_TAG_MANAGER) == 0) {
        os << rdf_model.catalog().languages[language_id];
    } else {
        tmp_manager.print_lan(os, (language_id & (~ObjectId::MASK_TAG_MANAGER)));
    }
    os << "\"}";
}


void JsonSelectExecutor::print_path_node(std::ostream& os, ObjectId node_id) {
    JsonOstreamEscape ostream_escape(os);
    std::ostream escaped_os(&ostream_escape);

    os << "{\"node\":";
    print(os, escaped_os, node_id);
    os << "}";
}


void JsonSelectExecutor::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse) {
    os << ",{\"edge\":";
    print(os, os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << ",\"inverse\":" << (inverse ? "true" : "false") << "},";
}


void JsonSelectExecutor::print(std::ostream& os, std::ostream& escaped_os, ObjectId object_id) {
    const auto mask        = object_id.id & ObjectId::TYPE_MASK;
    const auto unmasked_id = object_id.id & ObjectId::VALUE_MASK;
    switch (mask) {
    case ObjectId::MASK_ANON_INLINED: {
        os << "{\"type\":\"bnode\",\"value\":\"_:b";
        os << unmasked_id;
        os << "\"}";
        break;
    }
    case ObjectId::MASK_ANON_TMP: {
        os << "{\"type\":\"bnode\",\"value\":\"_:c";
        os << unmasked_id;
        os << "\"}";
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        os << "{\"type\":\"literal\",\"value\":\"";
        string_manager.print(escaped_os, unmasked_id);
        os << "\"}";
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        os << "{\"type\":\"literal\",\"value\":\"";
        Inliner::print_string_inlined<7>(escaped_os, unmasked_id);
        os << "\"}";
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        tmp_manager.print_str(escaped_os, unmasked_id);
        os << "\"}";
        break;
    }
    case ObjectId::MASK_STRING_XSD_EXTERN: {
        os << "{\"type\":\"literal\",\"value\":\"";
        string_manager.print(escaped_os, unmasked_id);
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#string\"}";
        break;
    }
    case ObjectId::MASK_STRING_XSD_INLINED: {
        os << "{\"type\":\"literal\",\"value\":\"";
        Inliner::print_string_inlined<7>(escaped_os, unmasked_id);
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#string\"}";
        break;
    }
    case ObjectId::MASK_STRING_XSD_TMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        tmp_manager.print_str(escaped_os, unmasked_id);
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#string\"}";
        break;
    }
    case ObjectId::MASK_POSITIVE_INT: {
        int64_t i = unmasked_id;
        os << "{\"type\":\"literal\",\"value\":\"";
        os << i;
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#integer\"}";
        break;
    }
    case ObjectId::MASK_NEGATIVE_INT: {
        int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
        os << "{\"type\":\"literal\",\"value\":\"";
        os << (i*-1);
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#integer\"}";
        break;
    }
    case ObjectId::MASK_FLOAT: {
        static_assert(sizeof(float) == 4, "float must be 4 bytes");
        os << "{\"type\":\"literal\",\"value\":\"";
        float f;
        uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
        dest[0] =  object_id.id        & 0xFF;
        dest[1] = (object_id.id >> 8)  & 0xFF;
        dest[2] = (object_id.id >> 16) & 0xFF;
        dest[3] = (object_id.id >> 24) & 0xFF;

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);
        os << float_buffer;
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#float\"}";
        break;
    }
    case ObjectId::MASK_DOUBLE_EXTERN:
    case ObjectId::MASK_DOUBLE_TMP: {
        static_assert(sizeof(double) == 8, "double must be 8 bytes");
        os << "{\"type\":\"literal\",\"value\":\"";
        double d = Conversions::unpack_double(object_id);
        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);
        os << double_buffer;
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#double\"}";
        break;
    }
    case ObjectId::MASK_BOOL: {
        os << "{\"type\":\"literal\",\"value\":\""
           << (object_id.get_value() == 0 ? "false" : "true")
           << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#boolean\"}";
        break;
    }
    case ObjectId::MASK_PATH: {
        os << "{\"type\":\"path\",\"value\":[";
        path_manager.print(os, unmasked_id, &print_path_node, &print_path_edge);
        os << "]}";
        break;
    }
    case ObjectId::MASK_IRI_EXTERN: {
        os << "{\"type\":\"uri\",\"value\":\"";
        uint64_t iri_id = unmasked_id & ObjectId::MASK_IRI_CONTENT;
        uint8_t prefix_id = (unmasked_id & ObjectId::MASK_IRI_PREFIX) >> 48;

        os << rdf_model.catalog().prefixes[prefix_id];
        string_manager.print(os, iri_id);
        os << "\"}";
        break;
    }
    case ObjectId::MASK_IRI_INLINED: {
        os << "{\"type\":\"uri\",\"value\":\"";
        uint8_t prefix_id = (object_id.id & ObjectId::MASK_IRI_PREFIX) >> (8*ObjectId::IRI_INLINE_BYTES);
        os << rdf_model.catalog().prefixes[prefix_id];
        Inliner::print_string_inlined<6>(os, unmasked_id);
        os << "\"}";
        break;
    }
    case ObjectId::MASK_IRI_TMP: {
        os << "{\"type\":\"uri\",\"value\":\"";
        uint8_t prefix_id = (unmasked_id & ObjectId::MASK_IRI_PREFIX) >> 48;
        os << rdf_model.catalog().prefixes[prefix_id];

        uint64_t iri_id = unmasked_id & ObjectId::MASK_IRI_CONTENT;
        tmp_manager.print_str(os, iri_id); // gets string from id
        os << "\"}";
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_INLINED: {
        os << "{\"type\":\"literal\",\"value\":\"";

        Inliner::print_string_inlined<5>(escaped_os, unmasked_id);

        int prefix_shift_size = 8 * ObjectId::STR_DT_INLINE_BYTES;
        uint16_t datatype_id = (object_id.id & ObjectId::MASK_LITERAL_TAG) >> prefix_shift_size;
        print_datatype_rdf_json(os, datatype_id);
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_EXTERN: {
        os << "{\"type\":\"literal\",\"value\":\"";
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        string_manager.print(escaped_os, str_id);

        uint16_t datatype_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;
        print_datatype_rdf_json(os, datatype_id);
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_TMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        tmp_manager.print_str(escaped_os, str_id);

        uint64_t datatype_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;
        print_datatype_rdf_json(os, datatype_id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_INLINED: {
        os << "{\"type\":\"literal\",\"value\":\"";

        Inliner::print_string_inlined<5>(escaped_os, unmasked_id);

        int prefix_shift_size = 8 * ObjectId::STR_LANG_INLINE_BYTES;
        uint16_t language_id = (object_id.id & ObjectId::MASK_LITERAL_TAG) >> prefix_shift_size;
        print_language_rdf_json(os, language_id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_EXTERN: {
        os << "{\"type\":\"literal\",\"value\":\"";
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        string_manager.print(escaped_os, str_id);

        uint16_t language_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;
        print_language_rdf_json(os, language_id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_TMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        tmp_manager.print_str(escaped_os, str_id);

        uint64_t language_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;
        print_language_rdf_json(os, language_id);
        break;
    }
    case ObjectId::MASK_DT_DATE:
    case ObjectId::MASK_DT_DATETIME:
    case ObjectId::MASK_DT_TIME:
    case ObjectId::MASK_DT_DATETIMESTAMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        DateTime datetime(object_id);
        os << datetime.get_value_string();
        os << "\",\"datatype\":\"" << datetime.get_datatype_string() << "\"}";
        break;
    }
    case ObjectId::MASK_DECIMAL_EXTERN: {
        os << "{\"type\":\"literal\",\"value\":\"";
        std::stringstream ss;
        string_manager.print(ss, unmasked_id);
        os << Decimal::from_external(ss.str());
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#decimal\"}";
        break;
    }
    case ObjectId::MASK_DECIMAL_INLINED: {
        os << "{\"type\":\"literal\",\"value\":\"";
        DecimalInlined decimal_inlined(unmasked_id);
        os << decimal_inlined.get_value_string();
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#decimal\"}";
        break;
    }
    case ObjectId::MASK_DECIMAL_TMP: {
        os << "{\"type\":\"literal\",\"value\":\"";
        std::ostringstream ss;
        tmp_manager.print_str(ss, unmasked_id);
        os << Decimal::from_external(ss.str());
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#decimal\"}";
        break;
    }
    case ObjectId::MASK_NULL: {
        // executor should not call print with NULL
        break;
    }

    default:
        throw std::logic_error("Unmanaged mask in JsonSelectExecutor::print: "
            + std::to_string(mask));
    }
}


void JsonSelectExecutor::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "JsonSelectExecutor(";
    for (size_t i = 0; i < projection_vars.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << '?' << get_query_ctx().get_var_name(projection_vars[i]);
    }
    os << ")\n";
    root->analyze(os, indent + 2);
}
