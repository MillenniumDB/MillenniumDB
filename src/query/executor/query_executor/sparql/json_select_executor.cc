#include "json_select_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/query_executor/json_ostream_escape.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"
#include "system/path_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

uint64_t JsonSelectExecutor::execute_empty_binding(std::ostream& os)
{
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

uint64_t JsonSelectExecutor::execute(std::ostream& os)
{
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

void JsonSelectExecutor::print_path_node(ObjectId node_id, std::ostream& os)
{
    JsonOstreamEscape ostream_escape(os);
    std::ostream escaped_os(&ostream_escape);

    os << "{\"node\":";
    print(os, escaped_os, node_id);
    os << "}";
}

void JsonSelectExecutor::print_path_edge(ObjectId edge_id, bool inverse, std::ostream& os)
{
    os << ",{\"edge\":";
    print(os, os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << ",\"inverse\":" << (inverse ? "true" : "false") << "},";
}

void JsonSelectExecutor::print(std::ostream& os, std::ostream& escaped_os, ObjectId oid)
{
    switch (oid.subtype()) {
    case ObjectSubType::Anon: {
        if (oid.type() == ObjectType::AnonInl) {
            os << "{\"type\":\"bnode\",\"value\":\"_:b";
        } else {
            os << "{\"type\":\"bnode\",\"value\":\"_:c";
        }
        os << Conversions::unpack_blank(oid);
        os << "\"}";
        break;
    }
    case ObjectSubType::String: {
        os << "{\"type\":\"literal\",\"value\":\"";
        Conversions::print_string(oid, escaped_os);
        os << "\"}";
        break;
    }
    case ObjectSubType::StringXsd: {
        os << "{\"type\":\"literal\",\"value\":\"";
        Conversions::print_string(oid, escaped_os);
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#string\"}";
        break;
    }
    case ObjectSubType::Int: {
        os << "{\"type\":\"literal\",\"value\":\"";
        os << Conversions::unpack_int(oid);
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#integer\"}";
        break;
    }
    case ObjectSubType::Float: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << "{\"type\":\"literal\",\"value\":\"";
        os << float_buffer;
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#float\"}";
        break;
    }
    case ObjectSubType::Double: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << "{\"type\":\"literal\",\"value\":\"";
        os << double_buffer;
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#double\"}";
        break;
    }
    case ObjectSubType::Bool: {
        os << "{\"type\":\"literal\",\"value\":\"" << (Conversions::unpack_bool(oid) ? "true" : "false")
           << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#boolean\"}";
        break;
    }
    case ObjectSubType::Path: {
        using namespace std::placeholders;
        os << "{\"type\":\"path\",\"value\":[";
        path_manager.for_each(
            Conversions::get_path_id(oid),
            [&](ObjectId oid) {
                print_path_node(oid, os);
            },
            [&](ObjectId oid, bool reverse) {
                print_path_edge(oid, reverse, os);
            }
        );
        os << "]}";
        break;
    }
    case ObjectSubType::Iri: {
        os << "{\"type\":\"uri\",\"value\":\"";
        Conversions::print_iri(oid, os);
        os << "\"}";
        break;
    }
    case ObjectSubType::StringDatatype: {
        auto&& [datatype, str] = Conversions::unpack_string_datatype(oid);
        os << "{\"type\":\"literal\",\"value\":\"";
        escaped_os << str;
        os << "\",\"datatype\":\"";
        os << datatype;
        os << "\"}";
        break;
    }
    case ObjectSubType::StringLang: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);
        os << "{\"type\":\"literal\",\"value\":\"";
        escaped_os << str;
        os << "\",\"xml:lang\":\"";
        os << lang;
        os << "\"}";
        break;
    }
    case ObjectSubType::TemporalLiteral: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << "{\"type\":\"literal\",\"value\":\"";
        os << datetime.get_value_string();
        os << "\",\"datatype\":\"" << datetime.get_datatype_string() << "\"}";
        break;
    }
    case ObjectSubType::Decimal: {
        auto decimal = Conversions::unpack_decimal(oid);

        os << "{\"type\":\"literal\",\"value\":\"";
        os << decimal;
        os << "\",\"datatype\":\"http://www.w3.org/2001/XMLSchema#decimal\"}";
        break;
    }
    case ObjectSubType::TensorFloat: {
        os << "{\"type\":\"literal\",\"value\":\"";
        os << Conversions::unpack_tensor<float>(oid);
        os << "\",\"datatype\":\"" << MDBExtensions::Type::TENSOR_FLOAT_IRI << "\"}";
        break;
    }
    case ObjectSubType::TensorDouble: {
        os << "{\"type\":\"literal\",\"value\":\"";
        os << Conversions::unpack_tensor<double>(oid);
        os << "\",\"datatype\":\"" << MDBExtensions::Type::TENSOR_DOUBLE_IRI << "\"}";
        break;
    }
    case ObjectSubType::Null: {
        // executor should not call print with NULL
        break;
    }
    // Not expected in RDF Model
    case ObjectSubType::NamedNode:
    case ObjectSubType::Dictionary:
    case ObjectSubType::List:
    case ObjectSubType::Edge:
    case ObjectSubType::NotFound:
        break;
    }
}

void JsonSelectExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    os << std::string(indent, ' ');
    os << "JsonSelectExecutor(";
    for (size_t i = 0; i < projection_vars.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << '?' << get_query_ctx().get_var_name(projection_vars[i]);
    }
    os << ")\n";

    root->print(os, indent + 2, print_stats);
}
