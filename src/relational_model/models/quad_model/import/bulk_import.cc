#include "bulk_import.h"

#include <chrono>
#include <iostream>
#include <map>
#include <thread>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "base/parser/grammar/common/value_visitor.h"
#include "base/parser/grammar/import/import_ast_adapted.h"
#include "base/parser/grammar/import/import_ast.h"
#include "base/parser/grammar/import/import_def.h"
#include "base/parser/grammar/import/import.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;

BulkImport::BulkImport(const string& filename, QuadModel& model) :
    model                           (model),
    catalog                         (model.catalog()),
    nodes_ordered_file              (OrderedFile<1>("nodes_ordered_file")),
    labels_ordered_file             (OrderedFile<2>("labels_ordered_file")),
    properties_ordered_file         (OrderedFile<3>("properties_ordered_file")),
    connections_ordered_file        (OrderedFile<4>("connections_ordered_file")),
    equal_from_to_ordered_file      (OrderedFile<3>("equal_from_to_ordered_file")),
    equal_from_type_ordered_file    (OrderedFile<3>("equal_from_type_ordered_file")),
    equal_to_type_ordered_file      (OrderedFile<3>("equal_to_type_ordered_file")),
    equal_from_to_type_ordered_file (OrderedFile<2>("equal_from_to_type_ordered_file"))
{
    import_file = ifstream(filename);
    import_file.unsetf(std::ios::skipws);

    if (import_file.fail()) {
        std::cerr << "File: \"" << filename << "\" could not be opened. Exiting.\n";
        exit(-1);
    }
}


void BulkImport::start_import() {
    auto start = chrono::system_clock::now();
    auto line_number = 1;
    std::cout << "Reading files & writing ordered file...\n";

    boost::spirit::istream_iterator file_iter( import_file );
    boost::spirit::istream_iterator file_iter_end;

    vector<uint64_t> ids_stack;

    do {
        import::ast::ImportLine import_line;
        bool r = phrase_parse(file_iter, file_iter_end, import::import(), import::parser::skipper, import_line);
        if (r) {
            if (import_line.type() == typeid(import::ast::Node)) {
                auto node_id = process_node( boost::get<import::ast::Node>(import_line) );
                ids_stack.clear();
                ids_stack.push_back(node_id);
            }
            else if (import_line.type() == typeid(import::ast::Edge)) {
                auto edge_id = process_edge( boost::get<import::ast::Edge>(import_line) );
                ids_stack.clear();
                ids_stack.push_back(edge_id);
            }
            else if (import_line.type() == typeid(import::ast::ImplicitEdge)) {
                auto implicit_edge = boost::get<import::ast::ImplicitEdge>(import_line);
                const auto nesting_lvl = implicit_edge.nesting_level();

                if (ids_stack.size() == 0) {
                    throw logic_error("ERROR on line " + std::to_string(line_number)
                        + ": can't use implicit edge on undefined object");
                }
                else if (nesting_lvl < ids_stack.size()) {
                    auto edge_id = process_implicit_edge(implicit_edge, ids_stack[nesting_lvl-1] );
                    ids_stack.resize(nesting_lvl);
                    ids_stack.push_back(edge_id);
                }
                else if (nesting_lvl == ids_stack.size()) {
                    auto edge_id = process_implicit_edge(implicit_edge, ids_stack[nesting_lvl-1] );
                    ids_stack.push_back(edge_id);
                }
                else {
                    throw logic_error("ERROR on line " + std::to_string(line_number)
                        + ": undefined level of implicit edge");
                }
            }

            line_number++;
        } else {
            cerr << "ERROR: line " << line_number << " has wrong format in import file:\n";
            auto character_count = 0;
            while (*file_iter != '\n' && file_iter != file_iter_end) {
                ++character_count;
                if (character_count > 256) {
                    cerr << "...(line exceeded 256 characters)\n";
                    return;
                } else {
                    cerr << *file_iter;
                    ++file_iter;
                }
            }
            cerr << "\n";
            return;
        }
    } while(file_iter != file_iter_end);

    auto finish_reading_files = chrono::system_clock::now();
    chrono::duration<float, milli> phase1_duration = finish_reading_files - start;
    std::cout << "  done in " << phase1_duration.count() << " ms\n\n";

    std::cout << "Creating indexes...\n";

    std::vector<thread> threads;

    // // We can paralelize the indexing if they use different OrderedFiles
    // threads.push_back( thread(&BulkImport::index_labels, this) );
    // threads.push_back( thread(&BulkImport::index_properties, this) );
    // threads.push_back( thread(&BulkImport::index_connections, this) );
    // threads.push_back( thread(&BulkImport::index_special_cases, this) );

    // for (auto& thread : threads) {
    //     thread.join();
    // }

    // Index nodes
    nodes_ordered_file.order(std::array<uint_fast8_t, 1 >{ 0 });
    model.nodes->bulk_import(nodes_ordered_file);

    catalog.distinct_labels = catalog.label2total_count.size();

    index_labels();
    index_properties();
    index_connections();
    index_special_cases();

    catalog.save_changes();

    auto finish_creating_index = chrono::system_clock::now();
    chrono::duration<float, milli> phase2_duration = finish_creating_index - finish_reading_files;
    std::cout << "  done in " << phase2_duration.count() << " ms\n\n";

    catalog.print();
}


void BulkImport::index_connections() {
    // CONNECTIONS
    connections_ordered_file.order(std::array<uint_fast8_t, 4> { 0, 1, 2, 3 });
    model.from_to_type_edge->bulk_import(connections_ordered_file);

    // set catalog.distinct_from
    {
        uint64_t distinct_from = 0;
        uint64_t current_from  = 0;

        connections_ordered_file.begin_read();
        auto record = connections_ordered_file.next_record();
        while (record != nullptr) {
            if (record->ids[0] != current_from) {
                ++distinct_from;
                current_from = record->ids[0];
            }
            record = connections_ordered_file.next_record();
        }
        catalog.distinct_from = distinct_from;
    }

    connections_ordered_file.order(std::array<uint_fast8_t, 4> { 2, 0, 1, 3 });
    model.to_type_from_edge->bulk_import(connections_ordered_file);

    //set catalog.distinct_to
    {
        uint64_t distinct_to = 0;
        uint64_t current_to  = 0;

        connections_ordered_file.begin_read();
        auto record = connections_ordered_file.next_record();
        while (record != nullptr) {
            if (record->ids[0] != current_to) {
                ++distinct_to;
                current_to = record->ids[0];
            }
            record = connections_ordered_file.next_record();
        }
        catalog.distinct_to = distinct_to;
    }

    connections_ordered_file.order(std::array<uint_fast8_t, 4> { 2, 0, 1, 3 });
    model.type_from_to_edge->bulk_import(connections_ordered_file);

    connections_ordered_file.order(std::array<uint_fast8_t, 4> { 0, 2, 1, 3 });
    model.type_to_from_edge->bulk_import(connections_ordered_file);

    catalog.distinct_type = catalog.type2total_count.size();
}


void BulkImport::index_special_cases() {
    // FROM=TO
    equal_from_to_ordered_file.order(std::array<uint_fast8_t, 3> { 0, 1, 2 });
    model.equal_from_to->bulk_import(equal_from_to_ordered_file);

    equal_from_to_ordered_file.order(std::array<uint_fast8_t, 3> { 1, 0, 2 });
    model.equal_from_to_inverted->bulk_import(equal_from_to_ordered_file);

    // calling this after inverted, so type is at pos 0
    set_distinct_type_stats(equal_from_to_ordered_file, catalog.type2equal_from_to_count);

    // FROM=TYPE
    equal_from_type_ordered_file.order(std::array<uint_fast8_t, 3> { 0, 1, 2 });
    model.equal_from_type->bulk_import(equal_from_type_ordered_file);

    // calling this before inverted, so type is at pos 0
    set_distinct_type_stats(equal_from_type_ordered_file, catalog.type2equal_from_type_count);

    equal_from_type_ordered_file.order(std::array<uint_fast8_t, 3> { 1, 0, 2 });
    model.equal_from_type_inverted->bulk_import(equal_from_type_ordered_file);

    // TO=TYPE
    equal_to_type_ordered_file.order(std::array<uint_fast8_t, 3> { 0, 1, 2 });
    model.equal_to_type->bulk_import(equal_to_type_ordered_file);

    // calling this before inverted, so type is at pos 0
    set_distinct_type_stats(equal_to_type_ordered_file, catalog.type2equal_to_type_count);

    equal_to_type_ordered_file.order(std::array<uint_fast8_t, 3> { 1, 0, 2 });
    model.equal_to_type_inverted->bulk_import(equal_to_type_ordered_file);

    // FROM=TO=TYPE
    equal_from_to_type_ordered_file.order(std::array<uint_fast8_t, 2> { 0, 1 });
    model.equal_from_to_type->bulk_import(equal_from_to_type_ordered_file);

    set_distinct_type_stats(equal_from_to_type_ordered_file, catalog.type2equal_from_to_type_count);

}


void BulkImport::index_labels() {
    // NODE - LABEL
    labels_ordered_file.order(std::array<uint_fast8_t, 2> { 0, 1 });
    model.node_label->bulk_import(labels_ordered_file);

    // LABEL - NODE
    labels_ordered_file.order(std::array<uint_fast8_t, 2> { 1, 0 });
    model.label_node->bulk_import(labels_ordered_file);

    catalog.distinct_labels = catalog.label2total_count.size();
}


void BulkImport::index_properties() {
    // OBJECT - KEY - VALUE
    properties_ordered_file.order(std::array<uint_fast8_t, 3> { 0, 1, 2 });
    model.object_key_value->bulk_import(properties_ordered_file);

    // KEY - VALUE - OBJECT
    properties_ordered_file.order(std::array<uint_fast8_t, 3> { 2, 0, 1 });
    model.key_value_object->bulk_import(properties_ordered_file);

    // count total properties and distinct values
    map<uint64_t, uint64_t> map_key_count;
    map<uint64_t, uint64_t> map_distinct_values;
    uint64_t current_key     = 0;
    uint64_t current_value   = 0;
    uint64_t key_count       = 0;
    uint64_t distinct_values = 0;

    properties_ordered_file.begin_read();
    auto record = properties_ordered_file.next_record();
    while (record != nullptr) {
        // check same key
        if (record->ids[0] == current_key) {
            ++key_count;
            // check if value changed
            if (record->ids[1] != current_value) {
                ++distinct_values;
                current_value = record->ids[1];
            }
        } else {
            // save stats from last key
            if (current_key != 0) {
                map_key_count.insert({ current_key, key_count });
                map_distinct_values.insert({ current_key, distinct_values });
            }
            current_key   = record->ids[0];
            current_value = record->ids[1];

            key_count       = 1;
            distinct_values = 1;
        }
        record = properties_ordered_file.next_record();
    }
    // save stats from last key
    if (current_key != 0) {
        map_key_count.insert({ current_key, key_count });
        map_distinct_values.insert({ current_key, distinct_values });
    }

    catalog.key2distinct    = move(map_distinct_values);
    catalog.key2total_count = move(map_key_count);
    catalog.distinct_keys   = catalog.key2total_count.size();

}


template <std::size_t N>
void BulkImport::set_distinct_type_stats(OrderedFile<N>& ordered_file, std::map<uint64_t, uint64_t>& map) {
    map.clear();
    uint64_t current_type = 0;
    uint64_t count        = 0;

    ordered_file.begin_read();
    auto record = ordered_file.next_record();
    while (record != nullptr) {
        // check same key
        if (record->ids[0] == current_type) {
            ++count;
        } else {
            // save stats from last key
            if (current_type != 0) {
                map.insert({ current_type, count });
            }
            current_type = record->ids[0];
            count = 1;
        }
        record = ordered_file.next_record();
    }
    // save stats from last key
    if (current_type != 0) {
        map.insert({ current_type, count });
    }
}



uint64_t BulkImport::get_node_id(const boost::variant<std::string, bool, int64_t, float>& node_id) {
    return boost::apply_visitor(*this, node_id);
}



uint64_t BulkImport::process_node(const import::ast::Node& node) {
    uint64_t node_id = get_node_id(node.name);

    for (auto& label : node.labels) {
        auto label_id = model.get_or_create_object_id(GraphObject::make_string(label));
        ++catalog.label_count;
        ++catalog.label2total_count[label_id];

        labels_ordered_file.append_record(std::array<uint64_t, 2> { node_id, label_id });
    }

    for (auto& property : node.properties) {
        ValueVisitor visitor;
        auto v = property.value;
        const auto value = visitor(v);

        const auto key_id   = model.get_or_create_object_id(GraphObject::make_string(property.key));
        const auto value_id = model.get_or_create_object_id(value);

        ++catalog.properties_count;

        properties_ordered_file.append_record(std::array<uint64_t, 3> { node_id, key_id, value_id });
    }
    return node_id;
}


uint64_t BulkImport::process_edge(const import::ast::Edge& edge) {
    if (edge.labels.size() != 1) {
        throw logic_error("In this quad bulk import all edges must have 1 type");
    }

    uint64_t left_id  = get_node_id(edge.lhs_id);
    uint64_t right_id = get_node_id(edge.rhs_id);
    uint64_t type_id  = get_node_id(edge.labels[0]);;

    uint64_t edge_id = (edge.direction == import::ast::EdgeDirection::right)
        ? create_connection(left_id, right_id, type_id)
        : create_connection(right_id, left_id, type_id);

    for (auto& property : edge.properties) {
        ValueVisitor visitor;
        auto v = property.value;
        const auto value = visitor(v);

        const auto key_id   = model.get_or_create_object_id(GraphObject::make_string(property.key));
        const auto value_id = model.get_or_create_object_id(value);

        ++catalog.properties_count;
        // ++catalog.key2total_count[key_id];

        properties_ordered_file.append_record(std::array<uint64_t, 3> { edge_id, key_id, value_id });
    }
    ++catalog.type2total_count[type_id];
    return edge_id;
}


uint64_t BulkImport::process_implicit_edge(const import::ast::ImplicitEdge& edge,
                                           const uint64_t implicit_object_id)
{
    if (edge.labels.size() != 1) {
        throw logic_error("In this quad bulk import all edges must have 1 type");
    }
    uint64_t right_id = get_node_id(edge.rhs_id);
    uint64_t type_id  = get_node_id(edge.labels[0]);

    uint64_t edge_id = (edge.direction == import::ast::EdgeDirection::right)
        ? create_connection(implicit_object_id, right_id, type_id)
        : create_connection(right_id, implicit_object_id, type_id);

    for (auto& property : edge.properties) {
        ValueVisitor visitor;
        auto v = property.value;
        auto value = visitor(v);

        const auto key_id   = model.get_or_create_object_id(GraphObject::make_string(property.key));
        const auto value_id = model.get_or_create_object_id(value);

        ++catalog.properties_count;
        // ++catalog.key2total_count[key_id];

        properties_ordered_file.append_record(std::array<uint64_t, 3> { edge_id, key_id, value_id });
    }
    ++catalog.type2total_count[type_id];
    return edge_id;
}


uint64_t BulkImport::create_connection(const uint64_t from_id, const uint64_t to_id, const uint64_t type_id) {
    uint64_t edge_id = ++catalog.connections_count | model.CONNECTION_MASK;

    // special cases
    if (from_id == to_id) {
        ++catalog.equal_from_to_count;
        equal_from_to_ordered_file.append_record(std::array<uint64_t, 3> { from_id, type_id, edge_id });

        if (from_id == type_id) {
            ++catalog.equal_from_to_type_count;
            equal_from_to_type_ordered_file.append_record(std::array<uint64_t, 2> { from_id, edge_id });
        }
    }
    if (from_id == type_id) {
        ++catalog.equal_from_type_count;
        equal_from_type_ordered_file.append_record(std::array<uint64_t, 3> { from_id, to_id, edge_id });
    }
    if (to_id == type_id) {
        ++catalog.equal_to_type_count;
        equal_to_type_ordered_file.append_record(std::array<uint64_t, 3> { type_id, from_id, edge_id });
    }

    connections_ordered_file.append_record(std::array<uint64_t, 4> { from_id, to_id, type_id, edge_id });
    model.edge_table->append_record(std::array<uint64_t, 3> { from_id, to_id, type_id });

    return edge_id;
}


uint64_t BulkImport::operator()(const std::string& str) {
    if (str[0] == '_') { // Anonymous Node
        if (str[1] != 'a') {
            throw logic_error("Invalid anonymous node declaration");
        }
        // delete first 2 characters: '_a'
        std::string tmp = str.substr(2, str.size() - 2);
        uint64_t unmasked_id = std::stoull(tmp);
        if (catalog.anonymous_nodes_count < unmasked_id) {
            catalog.anonymous_nodes_count = unmasked_id;
        }
        return unmasked_id | model.ANONYMOUS_NODE_MASK;
    }
    else if (str[0] == '"') { // String Literal Node
        // delete first and last characters: ("")
        std::string tmp = str.substr(1, str.size() - 2);
        auto obj_id = model.get_or_create_object_id(GraphObject::make_string(tmp));
        if (named_node_ids.insert(obj_id).second) {
            nodes_ordered_file.append_record({obj_id});
        }
        return obj_id;
    }
    else { // Named Node
        auto obj_id = model.get_or_create_object_id(GraphObject::make_identifiable(str));
        if (named_node_ids.insert(obj_id).second) {
            ++catalog.identifiable_nodes_count;
            nodes_ordered_file.append_record({obj_id});
        }
        return obj_id;
    }
}


uint64_t BulkImport::operator()(bool b) {
    auto obj_id = model.get_object_id(GraphObject::make_bool(b)).id;
    if (named_node_ids.insert(obj_id).second) {
        nodes_ordered_file.append_record({obj_id});
    }
    return obj_id;
}


uint64_t BulkImport::operator()(int64_t i) {
    auto obj_id = model.get_object_id(GraphObject::make_int(i)).id;
    if (named_node_ids.insert(obj_id).second) {
        nodes_ordered_file.append_record({obj_id});
    }
    return obj_id;
}


uint64_t BulkImport::operator()(float f) {
    auto obj_id = model.get_object_id(GraphObject::make_float(f)).id;
    if (named_node_ids.insert(obj_id).second) {
        nodes_ordered_file.append_record({obj_id});
    }
    return obj_id;
}
