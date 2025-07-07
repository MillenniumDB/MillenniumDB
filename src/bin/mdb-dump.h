#pragma once

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "bin/common.h"
#include "graph_models/exceptions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "misc/fatal_error.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/not_exists.h"
#include "query/executor/binding_iter/scan_ranges/assigned_var.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
#include "query/executor/query_executor/mql/return_executor.h"
#include "query/executor/query_executor/mql/return_type.h"
#include "query/executor/query_executor/tsv_ostream_escape.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "system/system.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

namespace MdbBin {

enum class DumpFormat {
    QUAD,
    JSON,
    NT,
    TTL,
};

struct DumpConfig {
    std::string db_directory;
    std::string output_prefix_path;
    DumpFormat output_format;
};

inline DumpConfig parse_dump_config(const std::vector<std::string>& args)
{
    DumpConfig config;

    if (args.size() != 3) {
        FATAL_ERROR("Expected 3 args for mdb dump, received ", args.size());
    }

    config.db_directory = args[0];
    config.output_prefix_path = args[1];

    auto format = to_lower(args[2]);

    if (format == "qm") {
        config.output_format = DumpFormat::QUAD;
    } else if (format == "json") {
        config.output_format = DumpFormat::JSON;
    } else if (format == "nt") {
        config.output_format = DumpFormat::NT;
    } else if (format == "ttl") {
        config.output_format = DumpFormat::TTL;
    } else {
        FATAL_ERROR("Unsupported format for dump: ", format);
    }

    return config;
}

static std::string oid2str(uint64_t oid)
{
    return MQL::Conversions::to_lexical_str(ObjectId(oid));
}

static std::string oid2json_str(uint64_t oid)
{
    std::stringstream ss;

    bool need_quote;

    const auto mask = oid & ObjectId::TYPE_MASK;
    switch (mask) {
    case ObjectId::MASK_NULL:
    case ObjectId::MASK_NEGATIVE_INT:
    case ObjectId::MASK_POSITIVE_INT:
    case ObjectId::MASK_FLOAT:
    case ObjectId::MASK_BOOL:
    // strings already come with double quotes
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
    case ObjectId::MASK_STRING_SIMPLE_TMP:
        need_quote = false;
        break;
    default:
        need_quote = true;
        break;
    }

    if (need_quote)
        ss << '"';

    // We use TSV escape because its the same as JSON escapes
    MQL::ReturnExecutor<MQL::ReturnType::TSV>::print(ss, ObjectId(oid));
    if (need_quote)
        ss << '"';
    return ss.str();
}

// Dump all the nodes with their properties grouped by label in JSON
inline void dump_nodes_json(const std::string& path)
{
    bool interruption_requested = false;

    std::fstream fs(path, std::ios::out | std::ios::trunc);
    fs << "{";

    auto write_properties = [&](uint64_t object_id) -> void {
        auto object_key_value_it = quad_model.object_key_value->get_range(
            &interruption_requested,
            { object_id, 0, 0 },
            { object_id, UINT64_MAX, UINT64_MAX }
        );
        auto object_key_value = object_key_value_it.next();
        auto sep_property = "";
        while (object_key_value != nullptr) {
            fs << sep_property << oid2json_str((*object_key_value)[1]) << ":"
               << oid2json_str((*object_key_value)[2]);
            sep_property = ",";
            object_key_value = object_key_value_it.next();
        }
    };

    // Loop over all nodes sorted by label
    auto label_node_it = quad_model.label_node
                             ->get_range(&interruption_requested, { 0, 0 }, { UINT64_MAX, UINT64_MAX });
    auto label_node = label_node_it.next();
    auto sep_label = "";
    uint64_t prev_label = ObjectId::NULL_ID;
    while (label_node != nullptr) {
        if ((*label_node)[0] != prev_label) {
            // Handle first occurrence of label
            fs << sep_label << oid2json_str((*label_node)[0]) << ":{\n  ";
            fs << oid2json_str((*label_node)[1]) << ":{";

            sep_label = "\n},\n";
            prev_label = (*label_node)[0];
        } else {
            // Handle subsequent occurrence of label
            fs << ",\n  " << oid2json_str((*label_node)[1]) << ":{";
        }
        // Write out properties
        write_properties((*label_node)[1]);
        fs << "}";
        label_node = label_node_it.next();
    }
    if (prev_label != ObjectId::NULL_ID) {
        // There were labelled nodes
        fs << "\n}";
    }

    // Loop over all nodes with no label
    if (prev_label == ObjectId::NULL_ID) {
        // No labelled nodes, no anti-join needed/possible
        auto node_it = quad_model.nodes->get_range(&interruption_requested, { 0 }, { UINT64_MAX });
        auto node = node_it.next();
        bool output { false };
        if (node != nullptr) {
            // Handle first node
            fs << "\n\"\":{\n  " << oid2json_str((*node)[0]) << ":{";
            write_properties((*node)[0]);
            fs << "}";
            output = true;
            node = node_it.next();
        }
        while (node != nullptr) {
            fs << ",\n  " << oid2json_str((*node)[0]) << ":{";
            write_properties((*node)[0]);
            fs << "}";
            node = node_it.next();
        }
        if (output) {
            // There were unlabelled nodes
            fs << "\n}";
        }
    } else {
        // There were labelled nodes, anti-join needed
        VarId node_var { 0 };
        VarId label_var { 1 };
        Binding binding { 2 };

        std::array<std::unique_ptr<ScanRange>, 1> lhs_ranges = { std::make_unique<UnassignedVar>(node_var) };
        std::array<std::unique_ptr<ScanRange>, 2> rhs_ranges = { std::make_unique<AssignedVar>(node_var),
                                                                 std::make_unique<UnassignedVar>(label_var) };

        auto lhs = std::make_unique<IndexScan<1>>(*quad_model.nodes, std::move(lhs_ranges));
        auto rhs = std::make_unique<IndexScan<2>>(*quad_model.node_label, std::move(rhs_ranges));
        auto unlabelled_node_it = std::make_unique<NotExists>(
            std::move(lhs),
            std::move(rhs),
            std::vector<VarId> {}
        );

        unlabelled_node_it->begin(binding);
        uint64_t node_id { 0 };
        if (unlabelled_node_it->next()) {
            // Handle first unlabelled node
            fs << ",\n\"\":{\n  ";
            node_id = binding[node_var].id;
            fs << oid2json_str(node_id) << ":{";
            write_properties(node_id);
            fs << "}";
        }
        while (unlabelled_node_it->next()) {
            // Handle subsequent unlabelled nodes
            node_id = binding[node_var].id;
            fs << ",\n  " << oid2json_str(node_id) << ":{";
            write_properties(node_id);
            fs << "}";
        }

        if (node_id != 0) {
            // There were unlabelled nodes
            fs << "\n}";
        }
    }

    fs << "\n}\n";
}

// Dump all the edges with their properties grouped by type in JSON
inline void dump_edges_json(const std::string& path)
{
    bool interruption_requested = false;

    std::fstream fs(path, std::ios::out | std::ios::trunc);
    fs << "{";
    // Loop over all edges sorted by type
    auto type_from_to_edge_it = quad_model.type_from_to_edge->get_range(
        &interruption_requested,
        { 0, 0, 0 },
        { UINT64_MAX, UINT64_MAX, UINT64_MAX }
    );
    auto type_from_to_edge = type_from_to_edge_it.next();
    auto sep_type = "";
    uint64_t prev_type = ObjectId::NULL_ID;
    while (type_from_to_edge != nullptr) {
        if ((*type_from_to_edge)[0] != prev_type) {
            // Handle first occurrence of type
            fs << sep_type << oid2json_str((*type_from_to_edge)[0]) << ":[\n  {\"from\":";
            fs << oid2json_str((*type_from_to_edge)[1]) << ",\"to\":";
            fs << oid2json_str((*type_from_to_edge)[2]);
            sep_type = "\n],\n";
            prev_type = (*type_from_to_edge)[0];
        } else {
            // Handle subsequent occurrence of type
            fs << ",\n  {\"from\":" << oid2json_str((*type_from_to_edge)[1]);
            fs << ",\"to\":" << oid2json_str((*type_from_to_edge)[2]);
        }
        // Write out properties if any
        auto object_key_value_it = quad_model.object_key_value->get_range(
            &interruption_requested,
            { (*type_from_to_edge)[3], 0, 0 },
            { (*type_from_to_edge)[3], UINT64_MAX, UINT64_MAX }
        );
        auto object_key_value = object_key_value_it.next();
        if (object_key_value != nullptr) {
            fs << ",\"properties\":{";
            auto sep_property = "";
            while (object_key_value != nullptr) {
                fs << sep_property << oid2json_str((*object_key_value)[1]) << ":"
                   << oid2json_str((*object_key_value)[2]);
                sep_property = ",";
                object_key_value = object_key_value_it.next();
            }
            fs << "}";
        }
        fs << "}";

        type_from_to_edge = type_from_to_edge_it.next();
    }

    if (prev_type != ObjectId::NULL_ID) {
        // Only when there were edges
        fs << "\n]";
    }

    fs << "\n}\n";
}

inline void dump_graph_quad_model(const std::string& path)
{
    bool interruption_requested = false;

    std::fstream fs(path, std::ios::out | std::ios::trunc);

    // NODES
    auto node_it = quad_model.nodes->get_range(&interruption_requested, { 0 }, { UINT64_MAX });
    auto node = node_it.next();
    while (node != nullptr) {
        fs << oid2str((*node)[0]);
        // Handle labels
        auto node_label_it = quad_model.node_label->get_range(
            &interruption_requested,
            { (*node)[0], 0 },
            { (*node)[0], UINT64_MAX }
        );
        auto node_label = node_label_it.next();
        while (node_label != nullptr) {
            auto label_name = oid2str((*node_label)[1]);
            label_name = label_name.substr(1, label_name.length() - 2);
            fs << " :" << label_name;
            node_label = node_label_it.next();
        }
        // Handle properties
        auto object_key_value_it = quad_model.object_key_value->get_range(
            &interruption_requested,
            { (*node)[0], 0, 0 },
            { (*node)[0], UINT64_MAX, UINT64_MAX }
        );
        auto object_key_value = object_key_value_it.next();
        while (object_key_value != nullptr) {
            auto key_name = oid2str((*object_key_value)[1]);
            key_name = key_name.substr(1, key_name.length() - 2);
            fs << " " << key_name << ":" << oid2str((*object_key_value)[2]);
            object_key_value = object_key_value_it.next();
        }
        fs << '\n';
        node = node_it.next();
    }

    // EDGES
    auto from_to_type_edge_it = quad_model.from_to_type_edge->get_range(
        &interruption_requested,
        { 0, 0, 0, 0 },
        { UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX }
    );
    auto from_to_type_edge = from_to_type_edge_it.next();
    while (from_to_type_edge != nullptr) {
        // Handle edge and type
        fs << oid2str((*from_to_type_edge)[0]) << "->" << oid2str((*from_to_type_edge)[1]) << " :"
           << oid2str((*from_to_type_edge)[2]);
        // Handle properties
        auto object_key_value_it = quad_model.object_key_value->get_range(
            &interruption_requested,
            { (*from_to_type_edge)[3], 0, 0 },
            { (*from_to_type_edge)[3], UINT64_MAX, UINT64_MAX }
        );
        auto object_key_value = object_key_value_it.next();
        while (object_key_value != nullptr) {
            auto key_name = oid2str((*object_key_value)[1]);
            key_name = key_name.substr(1, key_name.length() - 2);
            fs << " " << key_name << ":" << oid2str((*object_key_value)[2]);
            object_key_value = object_key_value_it.next();
        }
        fs << '\n';
        from_to_type_edge = from_to_type_edge_it.next();
    }
}

template<bool IS_TTL>
inline std::ostream& rdf_print(std::ostream& os, std::ostream& escaped_os, ObjectId oid)
{
    switch (RDF_OID::get_type(oid)) {
    case RDF_OID::Type::BLANK_INLINED: {
        os << "_:b";
        os << SPARQL::Conversions::unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::STRING_SIMPLE_INLINE:
    case RDF_OID::Type::STRING_SIMPLE_EXTERN:
    case RDF_OID::Type::STRING_SIMPLE_TMP: {
        os << '"';
        SPARQL::Conversions::print_string(oid, escaped_os);
        os << '"';
        break;
    }
    case RDF_OID::Type::STRING_XSD_INLINE:
    case RDF_OID::Type::STRING_XSD_EXTERN:
    case RDF_OID::Type::STRING_XSD_TMP: {
        os << '"';
        SPARQL::Conversions::print_string(oid, escaped_os);

        if constexpr (IS_TTL) {
            os << "\"^^xsd:string";
        } else {
            os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        }
        break;
    }
    case RDF_OID::Type::INT56_INLINE:
    case RDF_OID::Type::INT64_EXTERN:
    case RDF_OID::Type::INT64_TMP: {
        os << SPARQL::Conversions::unpack_int(oid);
        break;
    }
    case RDF_OID::Type::FLOAT32: {
        float f = SPARQL::Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << '"' << float_buffer << '"';

        if constexpr (IS_TTL) {
            os << "^^xsd:float";
        } else {
            os << "^^<http://www.w3.org/2001/XMLSchema#float>";
        }
        break;
    }
    case RDF_OID::Type::DOUBLE64_EXTERN:
    case RDF_OID::Type::DOUBLE64_TMP: {
        double d = SPARQL::Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << '"' << double_buffer << '"';

        if constexpr (IS_TTL) {
            os << "^^xsd:double";
        } else {
            os << "^^<http://www.w3.org/2001/XMLSchema#double>";
        }
        break;
    }
    case RDF_OID::Type::BOOL: {
        os << (SPARQL::Conversions::unpack_bool(oid) ? "true" : "false");
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
        SPARQL::Conversions::print_iri(oid, os);
        os << '>';
        break;
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE:
    case RDF_OID::Type::STRING_DATATYPE_EXTERN:
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        auto&& [datatype, str] = SPARQL::Conversions::unpack_string_datatype(oid);
        os << '"';
        escaped_os << str;
        os << "\"^^<";
        os << datatype;
        os << ">";
        break;
    }
    case RDF_OID::Type::STRING_LANG_INLINE:
    case RDF_OID::Type::STRING_LANG_EXTERN:
    case RDF_OID::Type::STRING_LANG_TMP: {
        auto&& [lang, str] = SPARQL::Conversions::unpack_string_lang(oid);
        os << '"';
        escaped_os << str;
        os << "\"@";
        os << lang;
        break;
    }
    case RDF_OID::Type::DATE:
    case RDF_OID::Type::DATETIME:
    case RDF_OID::Type::TIME:
    case RDF_OID::Type::DATETIMESTAMP: {
        DateTime datetime = SPARQL::Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case RDF_OID::Type::DECIMAL_INLINE:
    case RDF_OID::Type::DECIMAL_EXTERN:
    case RDF_OID::Type::DECIMAL_TMP: {
        auto decimal = SPARQL::Conversions::unpack_decimal(oid);
        os << '"' << decimal << '"';

        if constexpr (IS_TTL) {
            os << "^^xsd:decimal";
        } else {
            os << "^^<http://www.w3.org/2001/XMLSchema#decimal>";
        }

        break;
    }
    case RDF_OID::Type::TENSOR_FLOAT_INLINE:
    case RDF_OID::Type::TENSOR_FLOAT_EXTERN:
    case RDF_OID::Type::TENSOR_FLOAT_TMP: {
        const auto tensor = SPARQL::Conversions::unpack_tensor<float>(oid);
        os << '"' << tensor.to_string() << '"';
        if constexpr (IS_TTL) {
            os << "^^mdbtype:tensorFloat";
        } else {
            os << "^^<https://mdb.imfd.cl/type#tensorFloat>";
        }
        break;
    }
    case RDF_OID::Type::TENSOR_DOUBLE_INLINE:
    case RDF_OID::Type::TENSOR_DOUBLE_EXTERN:
    case RDF_OID::Type::TENSOR_DOUBLE_TMP: {
        const auto tensor = SPARQL::Conversions::unpack_tensor<double>(oid);
        os << '"' << tensor.to_string() << '"';
        if constexpr (IS_TTL) {
            os << "^^mdbtype:tensorDouble";
        } else {
            os << "^^<https://mdb.imfd.cl/type#tensorDouble>";
        }
        break;
    }
    case RDF_OID::Type::NULL_ID:
    case RDF_OID::Type::PATH:
    case RDF_OID::Type::BLANK_TMP: {
        // not possible
        break;
    }
    }
    return os;
}

inline void dump_nt(const std::string& path)
{
    bool interruption_requested = false;

    std::fstream fs(path, std::ios::out | std::ios::trunc);

    TSVOstreamEscape tsv_ostream_escape(fs);
    std::ostream escaped_os(&tsv_ostream_escape);

    auto triples_it = rdf_model.spo->get_range(
        &interruption_requested,
        { 0, 0, 0 },
        { UINT64_MAX, UINT64_MAX, UINT64_MAX }
    );

    for (auto triple = triples_it.next(); triple != nullptr; triple = triples_it.next()) {
        ObjectId s((*triple)[0]);
        ObjectId p((*triple)[1]);
        ObjectId o((*triple)[2]);

        rdf_print<false>(fs, escaped_os, s);
        fs << " ";
        rdf_print<false>(fs, escaped_os, p);
        fs << " ";
        rdf_print<false>(fs, escaped_os, o);
        fs << " .\n";
    }
}

inline void dump_ttl(const std::string& path)
{
    bool interruption_requested = false;

    std::fstream fs(path, std::ios::out | std::ios::trunc);

    TSVOstreamEscape tsv_ostream_escape(fs);
    std::ostream escaped_os(&tsv_ostream_escape);

    auto triples_it = rdf_model.spo->get_range(
        &interruption_requested,
        { 0, 0, 0 },
        { UINT64_MAX, UINT64_MAX, UINT64_MAX }
    );

    fs << "@prefix xsd: <http://www.w3.org/2001/XMLSchema#> .\n"
          "@prefix mdbtype: <https://mdb.imfd.cl/type#> .\n"
          "\n";

    auto triple = triples_it.next();

    ObjectId last_s = ObjectId::get_null();
    ObjectId last_p = ObjectId::get_null();

    if (triple != nullptr) {
        last_s = ObjectId((*triple)[0]);
        last_p = ObjectId((*triple)[1]);
        auto o = ObjectId((*triple)[2]);

        rdf_print<true>(fs, escaped_os, last_s);
        fs << " ";
        rdf_print<true>(fs, escaped_os, last_p);
        fs << " ";
        rdf_print<true>(fs, escaped_os, o);
    }

    for (triple = triples_it.next(); triple != nullptr; triple = triples_it.next()) {
        ObjectId s((*triple)[0]);
        ObjectId p((*triple)[1]);
        ObjectId o((*triple)[2]);

        if (last_s == s) {
            if (last_p == p) {
                fs << ", ";
                rdf_print<true>(fs, escaped_os, o);
            } else {
                fs << " ;\n\t";
                rdf_print<true>(fs, escaped_os, p);
                fs << " ";
                rdf_print<true>(fs, escaped_os, o);
            }
        } else {
            fs << " .\n";
            rdf_print<true>(fs, escaped_os, s);
            fs << " ";
            rdf_print<true>(fs, escaped_os, p);
            fs << " ";
            rdf_print<true>(fs, escaped_os, o);
        }

        last_s = s;
        last_p = p;
    }

    if (!last_s.is_null()) {
        fs << " .\n";
    }
}

inline int mdb_dump(const DumpConfig& config)
{
    auto model_id = Catalog::get_model_id(config.db_directory);

    System system(
        config.db_directory,
        StringManager::DEFAULT_STATIC_BUFFER,
        StringManager::DEFAULT_DYNAMIC_BUFFER,
        BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE,
        TensorManager::DEFAULT_STATIC_BUFFER,
        TensorManager::DEFAULT_DYNAMIC_BUFFER,
        1
    );

    QueryContext qc;
    QueryContext::set_query_ctx(&qc);

    std::cout << "Loading database..." << std::endl;
    try {
        std::unique_ptr<ModelDestroyer> model_destroyer;
        switch (model_id) {
        case Catalog::ModelID::QUAD: {
            model_destroyer = QuadModel::init();
            if (config.output_format == DumpFormat::QUAD) {
                std::cout << "Writing a QuadModel dump..." << std::endl;
                dump_graph_quad_model(config.output_prefix_path + ".qm");
            } else if (config.output_format == DumpFormat::JSON) {
                std::cout << "Writing a JSON dump..." << std::endl;
                dump_nodes_json(config.output_prefix_path + ".nodes.json");
                dump_edges_json(config.output_prefix_path + ".edges.json");
            } else {
                FATAL_ERROR("Quad Model only supports json and qm as valid formats");
            }
            break;
        }
        case Catalog::ModelID::RDF: {
            model_destroyer = RdfModel::init();
            if (config.output_format == DumpFormat::NT) {
                std::cout << "Writing a NT dump..." << std::endl;
                dump_nt(config.output_prefix_path + ".nt");
            } else if (config.output_format == DumpFormat::TTL) {
                std::cout << "Writing a TTL dump..." << std::endl;
                dump_ttl(config.output_prefix_path + ".ttl");
            } else {
                FATAL_ERROR("Quad Model only supports json and qm as valid formats");
            }
            break;
        }
        case Catalog::ModelID::GQL: {
            model_destroyer = GQLModel::init();
            FATAL_ERROR("mdb dump does not support gql yet");

            break;
        }
        } // end switch
        std::cout << "Dump done" << std::endl;
    } catch (const WrongModelException& e) {
        FATAL_ERROR(e.what());
    } catch (const WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what());
    }

    return EXIT_SUCCESS;

    return 0;
}
} // namespace MdbBin
