#include "tsv_select_executor.h"

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/query_executor/tsv_ostream_escape.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"
#include "system/path_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

uint64_t TSVSelectExecutor::execute_empty_binding(std::ostream& os)
{
    uint64_t result_count = 0;
    os << "\n"; // header

    while (root->next()) {
        result_count++;
        os << "\n";
    }
    return result_count;
}

uint64_t TSVSelectExecutor::execute(std::ostream& os)
{
    TSVOstreamEscape tsv_ostream_escape(os);
    std::ostream escaped_os(&tsv_ostream_escape);

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
        os << '\t' << get_query_ctx().get_var_name(*it);
    }
    os << '\n';

    while (root->next()) {
        result_count++;
        auto sep = ""; // first time is empty, then will be a tab
        for (auto it = projection_vars.cbegin(); it != projection_vars.cend(); ++it) {
            auto value = (*binding)[*it];
            os << sep;
            sep = "\t";
            if (!value.is_null()) {
                print(os, escaped_os, value);
            }
        }
        os << '\n';
    }
    return result_count;
}

void TSVSelectExecutor::print_path_node(ObjectId node_id, std::ostream& os)
{
    TSVOstreamEscape xml_ostream_escape(os);
    std::ostream escaped_os(&xml_ostream_escape);

    print(os, escaped_os, node_id);
}

void TSVSelectExecutor::print_path_edge(ObjectId edge_id, bool inverse, std::ostream& os)
{
    os << ' ';
    if (inverse) {
        os << '^';
    }
    print(os, os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << ' ';
}

void TSVSelectExecutor::print(std::ostream& os, std::ostream& escaped_os, ObjectId oid)
{
    switch (oid.subtype()) {
    case ObjectSubType::Anon: {
        if (oid.type() == ObjectType::AnonInl) {
            os << "_:b";
        } else {
            os << "_:c";
        }
        os << Conversions::unpack_blank(oid);
        break;
    }
    case ObjectSubType::String: {
        os << '"';
        Conversions::print_string(oid, escaped_os);
        os << '"';
        break;
    }
    case ObjectSubType::StringXsd: {
        os << '"';
        Conversions::print_string(oid, escaped_os);
        os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        break;
    }
    case ObjectSubType::Int: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case ObjectSubType::Float: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << float_buffer;
        break;
    }
    case ObjectSubType::Double: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case ObjectSubType::Bool: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false")
           << "^^<http://www.w3.org/2001/XMLSchema#boolean>";
        break;
    }
    case ObjectSubType::Path: {
        using namespace std::placeholders;
        os << '[';
        path_manager.for_each(
            Conversions::get_path_id(oid),
            [&](ObjectId oid) {
                print_path_node(oid, os);
            },
            [&](ObjectId oid, bool reverse) {
                print_path_edge(oid, reverse, os);
            }
        );
        os << ']';
        break;
    }
    case ObjectSubType::Iri: {
        os << '<';
        Conversions::print_iri(oid, os);
        os << '>';
        break;
    }
    case ObjectSubType::StringDatatype: {
        auto&& [datatype, str] = Conversions::unpack_string_datatype(oid);
        os << '"';
        escaped_os << str;
        os << "\"^^<";
        os << datatype;
        os << ">";
        break;
    }
    case ObjectSubType::StringLang: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);
        os << '"';
        escaped_os << str;
        os << "\"@";
        os << lang;
        break;
    }
    case ObjectSubType::TemporalLiteral: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case ObjectSubType::Decimal: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case ObjectSubType::TensorFloat: {
        const auto tensor = Conversions::unpack_tensor<float>(oid);
        os << '"' << tensor.to_string();
        os << "\"^^<" << MDBExtensions::Type::TENSOR_FLOAT_IRI << ">";
        break;
    }
    case ObjectSubType::TensorDouble: {
        const auto tensor = Conversions::unpack_tensor<double>(oid);
        os << '"' << tensor.to_string();
        os << "\"^^<" << MDBExtensions::Type::TENSOR_DOUBLE_IRI << ">";
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
    case ObjectSubType::PGMetaData:
    case ObjectSubType::NotFound:
        break;
    }
}

void TSVSelectExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    os << std::string(indent, ' ');
    os << "TSVSelectExecutor(";
    for (size_t i = 0; i < projection_vars.size(); i++) {
        if (i != 0) {
            os << ", ";
        }
        os << '?' << get_query_ctx().get_var_name(projection_vars[i]);
    }
    os << ")\n";

    root->print(os, indent + 2, print_stats);
}
