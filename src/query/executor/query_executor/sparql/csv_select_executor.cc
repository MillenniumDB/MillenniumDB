#include "csv_select_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "system/path_manager.h"
#include "query/executor/query_executor/csv_ostream_escape.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

uint64_t CSVSelectExecutor::execute_empty_binding(std::ostream& os) {
    uint64_t result_count = 0;
    os << "\n"; // header

    while (root->next()) {
        result_count++;
        os << "\n";
    }
    return result_count;
}


uint64_t CSVSelectExecutor::execute(std::ostream& os) {
    CSVOstreamEscape csv_ostream_escape(os);
    std::ostream escaped_os(&csv_ostream_escape);

    uint64_t result_count = 0;
    binding = std::make_unique<Binding>(get_query_ctx().get_var_size());
    root->begin(*binding);

    auto it = projection_vars.cbegin();
    // Executes queries with empty projection differently, so we can assume later the projection is not empty
    if (it == projection_vars.cend()) {
        return execute_empty_binding(os);
    }

    // print header
    os << get_query_ctx().get_var_name(*it);
    while (++it != projection_vars.cend()) {
        os << ',' << get_query_ctx().get_var_name(*it);
    }
    os << '\n';

    while (root->next()) {
        result_count++;
        auto sep = ""; // first time is empty, then will be a comma
        for (auto it = projection_vars.cbegin(); it != projection_vars.cend(); ++it) {
            auto value = (*binding)[*it];
            os << sep;
            sep = ",";
            if (!value.is_null()) {
                print(os, escaped_os, value);
            }
        }
        os << '\n';
    }
    return result_count;
}


void CSVSelectExecutor::print_path_node(std::ostream& os, ObjectId node_id) {
    CSVOstreamEscape xml_ostream_escape(os);
    std::ostream escaped_os(&xml_ostream_escape);

    print(os, escaped_os, node_id);
}


void CSVSelectExecutor::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse) {
    os << ' ';
    if (inverse) {
        os << '^';
    }
    print(os, os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << ' ';
}


void CSVSelectExecutor::print(std::ostream& os, std::ostream& escaped_os, ObjectId oid) {
    switch (RDF_OID::get_type(oid)) {
    case RDF_OID::Type::BLANK_INLINED: {
        os << "_:b" << Conversions::unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::BLANK_TMP: {
        os << "_:c" << Conversions::unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::STRING_SIMPLE_INLINE:
    case RDF_OID::Type::STRING_SIMPLE_EXTERN:
    case RDF_OID::Type::STRING_SIMPLE_TMP:
    case RDF_OID::Type::STRING_XSD_INLINE:
    case RDF_OID::Type::STRING_XSD_EXTERN:
    case RDF_OID::Type::STRING_XSD_TMP: {
        os << '"';
        Conversions::print_string(oid, escaped_os);
        os << '"';
        break;
    }
    case RDF_OID::Type::INT56_INLINE:
    case RDF_OID::Type::INT64_EXTERN:
    case RDF_OID::Type::INT64_TMP: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case RDF_OID::Type::FLOAT32: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << float_buffer;
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
    case RDF_OID::Type::BOOL: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        break;
    }
    case RDF_OID::Type::PATH: {
        using namespace std::placeholders;
        os << '[';
        path_manager.print(os,
                           Conversions::get_path_id(oid),
                           std::bind(&CSVSelectExecutor::print_path_node, _1, _2),
                           std::bind(&CSVSelectExecutor::print_path_edge, _1, _2, _3));
        os << ']';
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
        Conversions::print_iri(oid, os);
        os << '>';
        break;
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE:
    case RDF_OID::Type::STRING_DATATYPE_EXTERN:
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        os << '"';
        Conversions::print_string_datatype(oid, escaped_os);
        os << '"';

        break;
    }
    case RDF_OID::Type::STRING_LANG_INLINE:
    case RDF_OID::Type::STRING_LANG_EXTERN:
    case RDF_OID::Type::STRING_LANG_TMP: {
        os << '"';
        Conversions::print_string_lang(oid, escaped_os);
        os << '"';

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
    case RDF_OID::Type::DECIMAL_INLINE:
    case RDF_OID::Type::DECIMAL_EXTERN:
    case RDF_OID::Type::DECIMAL_TMP: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case RDF_OID::Type::NULL_ID: {
        // executor should not call print with NULL
        break;
    }
    }
}


void CSVSelectExecutor::analyze(std::ostream& os, bool print_stats, int indent) const {
    os << std::string(indent, ' ');
    os << "CSVSelectExecutor(";
    for (size_t i = 0; i < projection_vars.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << '?' << get_query_ctx().get_var_name(projection_vars[i]);
    }
    os << ")\n";

    BindingIterPrinter printer(os, print_stats, indent + 2);
    root->accept_visitor(printer);
}
