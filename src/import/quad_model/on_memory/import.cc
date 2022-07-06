#include "import.h"

#include <chrono>

#include "storage/index/bplus_tree/bpt_mem_import.h"
#include "storage/index/hash/object_file_hash/object_file_hash_mem_import.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"

using namespace Import;

char* ExternalString::strings = nullptr;

template <std::size_t N>
void InMemoryImport::create_bpt(const std::string& base_name, std::vector<std::array<uint64_t, N>>& values) {
    std::sort(values.begin(), values.end());

    BPTLeafWriter<N> leaf_writer(base_name + ".leaf");
    BPTDirWriter<N> dir_writer(base_name + ".dir");

    auto* i = values.data();
    auto* end = values.end().operator->();
    uint32_t current_block = 0;

    if (values.size() == 0) {
        leaf_writer.make_empty();
    }

    while (i < end) {
        char* begin = (char*) i;

        // skip first leaf from going into bulk_import
        if (i != values.data()) {
            dir_writer.bulk_insert(i, 0, current_block);
        }
        i += leaf_writer.max_records;
        if (i < end) {
            leaf_writer.process_block(begin, leaf_writer.max_records, ++current_block);
        } else {
            leaf_writer.process_block(begin, values.size() % leaf_writer.max_records, 0);
        }
    }
}


template <std::size_t N>
void InMemoryImport::reorder_cols(std::vector<std::array<uint64_t, N>>& values,
                                  std::array<size_t, N>& current_permutation,
                                  std::array<size_t, N>&& new_permutation)
{
    static_assert(N == 2 || N == 3 || N == 4, "Unsuported N");
    assert(current_permutation.size() == new_permutation.size());

    if (N == 2) {
        if (current_permutation[0] == new_permutation[0]) {
            assert(current_permutation[1] == new_permutation[1]);
        }
        else {
            for (auto& value : values) {
                std::swap(value[0], value[1]);
            }
        }
    }

    if (N == 3) {
        if (current_permutation[0] == new_permutation[0]
         && current_permutation[1] == new_permutation[1]
         && current_permutation[2] == new_permutation[2])
        {
            // do nothing
        }
        else if (new_permutation[0] == current_permutation[1]
              && new_permutation[1] == current_permutation[2]
              && new_permutation[2] == current_permutation[0])
        {
            for (auto& value : values) {
                auto aux  = value[0];
                value[0] = value[1];
                value[1] = value[2];
                value[2] = aux;
            }
        }
        else if (new_permutation[0] == current_permutation[1]
              && new_permutation[1] == current_permutation[0]
              && new_permutation[2] == current_permutation[2])
        {
            for (auto& value : values) {
                auto aux  = value[0];
                value[0] = value[1];
                value[1] = aux;
            }
        }
        else {
            throw std::invalid_argument("Unsuported permutation");
        }
    }

    if (N == 4) {
        if (current_permutation[0] == new_permutation[0]
         && current_permutation[1] == new_permutation[1]
         && current_permutation[2] == new_permutation[2]
         && current_permutation[3] == new_permutation[3])
        {
            // do nothing
        }
        else if (new_permutation[0] == current_permutation[1]
              && new_permutation[1] == current_permutation[2]
              && new_permutation[2] == current_permutation[0]
              && new_permutation[3] == current_permutation[3])
        {
            for (auto& value : values) {
                auto aux  = value[0];
                value[0] = value[1];
                value[1] = value[2];
                value[2] = aux;
            }
        }
        else if (new_permutation[0] == current_permutation[0]
              && new_permutation[1] == current_permutation[2]
              && new_permutation[2] == current_permutation[1]
              && new_permutation[3] == current_permutation[3])
        {
            for (auto& value : values) {
                auto aux  = value[1];
                value[1] = value[2];
                value[2] = aux;
            }
        }
        else {
            throw std::invalid_argument("Unsuported permutation");
        }
    }
    current_permutation = std::move(new_permutation);
}


void InMemoryImport::start_import(const std::string& input_filename) {
    auto start = std::chrono::system_clock::now();
    lexer.begin(input_filename);

    size_t external_strings_initial_size = 1024ULL * 1024ULL * 1024ULL * initial_strings_in_GB;
    external_strings = new char[external_strings_initial_size];
    ExternalString::strings = external_strings;
    external_strings_capacity = external_strings_initial_size;
    external_strings_end = 1;
    catalog.anonymous_nodes_count = 0;

    state_transitions = new int[Token::TOTAL_TOKENS*State::TOTAL_STATES];
    create_automata();

    int current_state = State::LINE_BEGIN;
    current_line = 1;
    while (int token = lexer.next_token()) {
        current_state = get_transition(current_state, token);
    }

    auto end_lexer = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> parser_duration = end_lexer - start;
    std::cout << "Parser duration: " << parser_duration.count() << " ms\n";

    {   // Destroy external_ids_map replacing it with an empty map
        robin_hood::unordered_set<ExternalString, ExternalStringHasher> tmp;
        external_ids_map.swap(tmp);
    }

    {   // Write Object File
        std::fstream object_file;
        object_file.open(db_folder + "/object_file.dat", std::ios::out|std::ios::binary);
        object_file.write(external_strings, external_strings_end);

        ObjectFileHashMemImport object_file_hash(db_folder + "/str_hash");

        size_t current_offset = 1;
        while (current_offset < external_strings_end) {
            auto current_str = external_strings + current_offset;
            object_file_hash.create_id(current_str, current_offset);
            current_offset += strlen(current_str) + 1;
        }

        delete[] external_strings;
    }

    catalog.connections_count = edges.size();
    catalog.equal_from_to_count = equal_from_to.size();
    catalog.equal_from_type_count = equal_from_type.size();
    catalog.equal_from_to_type_count = equal_from_to_type.size();

    {   // Write nodes B+Tree
        // write missing nodes from edges
        robin_hood::unordered_set<uint64_t> declared_nodes_set;

        for (auto n : declared_nodes) {
            declared_nodes_set.insert(n[0]);
        }

        for (const auto& edge : edges) {
            if (declared_nodes_set.insert(edge[0]).second) {
                declared_nodes.push_back({edge[0]});
            }
            if (declared_nodes_set.insert(edge[1]).second) {
                declared_nodes.push_back({edge[1]});
            }
            if (declared_nodes_set.insert(edge[2]).second) {
                declared_nodes.push_back({edge[2]});
            }
        }
        create_bpt(db_folder + "/nodes", declared_nodes);
        catalog.identifiable_nodes_count = declared_nodes_set.size();
    }

    {   // Write Labels B+Trees
        size_t COL_NODE = 0, COL_LABEL = 1;
        std::array<size_t, 2> current_label = { COL_NODE, COL_LABEL };

        reorder_cols(labels, current_label, { COL_NODE, COL_LABEL });
        create_bpt(db_folder + "/node_label", labels);

        reorder_cols(labels, current_label, { COL_LABEL, COL_NODE });
        create_bpt(db_folder + "/label_node", labels);
    }

    {   // Compute Label Statistics
        robin_hood::unordered_map<uint64_t, uint64_t> map_label_count;
        uint64_t current_label     = 0;
        // uint64_t current_value   = 0;
        uint64_t label_count       = 0;
        // uint64_t distinct_values = 0;

        for (const auto& record : labels) {
            // check same key
            if (record[0] == current_label) {
                ++label_count;
            } else {
                // save stats from last key
                if (current_label != 0) {
                    map_label_count.insert({ current_label, label_count });
                }
                current_label   = record[0];
                label_count = 1;
            }
        }
        // save stats from last key
        if (current_label != 0) {
            map_label_count.insert({ current_label, label_count });
        }

        catalog.distinct_labels   = map_label_count.size();
        catalog.label2total_count = move(map_label_count);
        catalog.label_count       = labels.size();
    }

    {   // Write Property B+Trees
        size_t COL_OBJ = 0, COL_KEY = 1, COL_VALUE = 2;
        std::array<size_t, 3> current_prop = { COL_OBJ, COL_KEY, COL_VALUE };

        reorder_cols(properties, current_prop, { COL_OBJ, COL_KEY, COL_VALUE });
        create_bpt(db_folder + "/object_key_value", properties);

        reorder_cols(properties, current_prop, { COL_KEY, COL_VALUE, COL_OBJ });
        create_bpt(db_folder + "/key_value_object", properties);
    }

    {   // Compute Property Statistics
        robin_hood::unordered_map<uint64_t, uint64_t> map_key_count;
        robin_hood::unordered_map<uint64_t, uint64_t> map_distinct_values;
        uint64_t current_key     = 0;
        uint64_t current_value   = 0;
        uint64_t key_count       = 0;
        uint64_t distinct_values = 0;

        for (const auto& record : properties) {
            // check same key
            if (record[0] == current_key) {
                ++key_count;
                // check if value changed
                if (record[1] != current_value) {
                    ++distinct_values;
                    current_value = record[1];
                }
            } else {
                // save stats from last key
                if (current_key != 0) {
                    map_key_count.insert({ current_key, key_count });
                    map_distinct_values.insert({ current_key, distinct_values });
                }
                current_key   = record[0];
                current_value = record[1];

                key_count       = 1;
                distinct_values = 1;
            }
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

    {   // Write Edge Table (must be done before ordering)
        EdgeTableMemImport table_writer(db_folder + "/edges.table");

        for (const auto& edge : edges) {
            table_writer.insert_tuple(edge);
        }
    }

    {   // Write Edges quad B+Trees
        size_t COL_FROM = 0, COL_TO = 1, COL_TYPE = 2, COL_EDGE = 3;
        std::array<size_t, 4> current_edge = { COL_FROM, COL_TO, COL_TYPE, COL_EDGE };

        reorder_cols(edges, current_edge, { COL_FROM, COL_TO, COL_TYPE, COL_EDGE });
        create_bpt(db_folder + "/from_to_type_edge", edges);

        { // set catalog.distinct_from
            uint64_t distinct_from = 0;
            uint64_t current_from  = 0;

            for (const auto& record : edges) {
                if (record[0] != current_from) {
                    ++distinct_from;
                    current_from = record[0];
                }
            }
            catalog.distinct_from = distinct_from;
        }

        reorder_cols(edges, current_edge, { COL_TO, COL_TYPE, COL_FROM, COL_EDGE });
        create_bpt(db_folder + "/to_type_from_edge", edges);

        { // set catalog.distinct_to
            uint64_t distinct_to = 0;
            uint64_t current_from  = 0;

            for (const auto& record : edges) {
                if (record[0] != current_from) {
                    ++distinct_to;
                    current_from = record[0];
                }
            }
            catalog.distinct_to = distinct_to;
        }

        reorder_cols(edges, current_edge, { COL_TYPE, COL_FROM, COL_TO, COL_EDGE });
        create_bpt(db_folder + "/type_from_to_edge", edges);

        reorder_cols(edges, current_edge, { COL_TYPE, COL_TO, COL_FROM, COL_EDGE });
        create_bpt(db_folder + "/type_to_from_edge", edges);

        // set distinct_type, may be a redundant stat
        catalog.distinct_type = catalog.type2total_count.size();
    }

    {   // FROM=TO=TYPE EDGE
        size_t COL_FROM_TO_TYPE = 0, COL_EDGE = 1;
        std::array<size_t, 2> current_edge = { COL_FROM_TO_TYPE, COL_EDGE };

        reorder_cols(equal_from_to_type, current_edge, { COL_FROM_TO_TYPE, COL_EDGE });
        create_bpt(db_folder + "/equal_from_to_type", equal_from_to_type);

        {
            // create catalog.type2equal_from_to_type_count
            uint64_t current_type = 0;
            uint64_t count        = 0;
            for (const auto& record : equal_from_to_type) {
                // check same key
                if (record[0] == current_type) {
                    ++count;
                } else {
                    // save stats from last key
                    if (current_type != 0) {
                        catalog.type2equal_from_to_type_count.insert({ current_type, count });
                    }
                    current_type = record[0];
                    count = 1;
                }
            }
            // save stats from last key
            if (current_type != 0) {
                catalog.type2equal_from_to_type_count.insert({ current_type, count });
            }
        }
    }

    {   // FROM=TO TYPE EDGE
        size_t COL_FROM_TO = 0, COL_TYPE = 1, COL_EDGE = 2;
        std::array<size_t, 3> current_edge = { COL_FROM_TO, COL_TYPE, COL_EDGE };

        reorder_cols(equal_from_to, current_edge, { COL_FROM_TO, COL_TYPE, COL_EDGE });
        create_bpt(db_folder + "/equal_from_to", equal_from_to);

        reorder_cols(equal_from_to, current_edge, { COL_TYPE, COL_FROM_TO, COL_EDGE });
        create_bpt(db_folder + "/equal_from_to_inverted", equal_from_to);

        {
            // create catalog.type2equal_from_to_count
            // calling this AFTER inverted, so type is at pos 0
            uint64_t current_type = 0;
            uint64_t count        = 0;
            for (const auto& record : equal_from_to) {
                // check same key
                if (record[0] == current_type) {
                    ++count;
                } else {
                    // save stats from last key
                    if (current_type != 0) {
                        catalog.type2equal_from_to_count.insert({ current_type, count });
                    }
                    current_type = record[0];
                    count = 1;
                }
            }
            // save stats from last key
            if (current_type != 0) {
                catalog.type2equal_from_to_count.insert({ current_type, count });
            }
        }
    }

    {   // FROM=TYPE TO EDGE
        size_t COL_FROM_TYPE = 0, COL_TO = 1, COL_EDGE = 2;
        std::array<size_t, 3> current_edge = { COL_FROM_TYPE, COL_TO, COL_EDGE };

        reorder_cols(equal_from_type, current_edge, { COL_FROM_TYPE, COL_TO, COL_EDGE });
        create_bpt(db_folder + "/equal_from_type", equal_from_type);

        {
            // create catalog.type2equal_from_type_count
            // calling this BEFORE inverted, so type is at pos 0
            uint64_t current_type = 0;
            uint64_t count        = 0;
            for (const auto& record : equal_from_type) {
                // check same key
                if (record[0] == current_type) {
                    ++count;
                } else {
                    // save stats from last key
                    if (current_type != 0) {
                        catalog.type2equal_from_type_count.insert({ current_type, count });
                    }
                    current_type = record[0];
                    count = 1;
                }
            }
            // save stats from last key
            if (current_type != 0) {
                catalog.type2equal_from_type_count.insert({ current_type, count });
            }
        }

        reorder_cols(equal_from_type, current_edge, { COL_TO, COL_FROM_TYPE, COL_EDGE });
        create_bpt(db_folder + "/equal_from_type_inverted", equal_from_type);
    }

    {   // TO=TYPE FROM EDGE
        size_t COL_TO_TYPE = 0, COL_FROM = 1, COL_EDGE = 2;
        std::array<size_t, 3> current_edge = { COL_TO_TYPE, COL_FROM, COL_EDGE };

        reorder_cols(equal_to_type, current_edge, { COL_TO_TYPE, COL_FROM, COL_EDGE });
        create_bpt(db_folder + "/equal_to_type", equal_to_type);

        {
            // create catalog.type2equal_to_type_count
            // calling this BEFORE inverted, so type is at pos 0
            uint64_t current_type = 0;
            uint64_t count        = 0;
            for (const auto& record : equal_from_to) {
                // check same key
                if (record[0] == current_type) {
                    ++count;
                } else {
                    // save stats from last key
                    if (current_type != 0) {
                        catalog.type2equal_to_type_count.insert({ current_type, count });
                    }
                    current_type = record[0];
                    count = 1;
                }
            }
            // save stats from last key
            if (current_type != 0) {
                catalog.type2equal_to_type_count.insert({ current_type, count });
            }
        }

        reorder_cols(equal_to_type, current_edge, { COL_FROM, COL_TO_TYPE, COL_EDGE });
        create_bpt(db_folder + "/equal_to_type_inverted", equal_to_type);
    }
    catalog.save_changes();

    auto end_order = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> order_duration = end_order - end_lexer;
    std::cout << "Order duration: " << order_duration.count() << " ms\n";
}


void InMemoryImport::create_automata() {
    state_transitions = new int[Token::TOTAL_TOKENS*State::TOTAL_STATES];

    // llenar vacío
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        for (int t = 1; t < Token::TOTAL_TOKENS; t++) {
            set_transition(s, t, State::WRONG_LINE, std::bind(&InMemoryImport::print_error, this) );
        }
    }
    // ignore whitespace token
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        set_transition(s, Token::WHITESPACE, s, std::bind(&InMemoryImport::do_nothing, this));
    }

     // wrong line stays wrong (without giving more errors) until endline
    for (int t = 0; t < Token::TOTAL_TOKENS; t++) {
        set_transition(State::WRONG_LINE, t, State::WRONG_LINE, std::bind(&InMemoryImport::do_nothing, this));
    }
    set_transition(State::WRONG_LINE, Token::ENDLINE, State::LINE_BEGIN, std::bind(&InMemoryImport::finish_wrong_line, this));

    set_transition(State::LINE_BEGIN, Token::IDENTIFIER, State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_identifier, this));
    set_transition(State::LINE_BEGIN, Token::ANON,       State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_anon, this));
    set_transition(State::LINE_BEGIN, Token::STRING,     State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_string, this));
    set_transition(State::LINE_BEGIN, Token::IRI,        State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_iri, this));
    set_transition(State::LINE_BEGIN, Token::INTEGER,    State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_int, this));
    set_transition(State::LINE_BEGIN, Token::FLOAT,      State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_float, this));
    set_transition(State::LINE_BEGIN, Token::TRUE,       State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_true, this));
    set_transition(State::LINE_BEGIN, Token::FALSE,      State::FIRST_ID, std::bind(&InMemoryImport::save_first_id_false, this));
    set_transition(State::LINE_BEGIN, Token::IMPLICIT,   State::IMPLICIT_EDGE, std::bind(&InMemoryImport::save_first_id_implicit, this));

    set_transition(State::FIRST_ID,     Token::COLON, State::EXPECT_NODE_LABEL, std::bind(&InMemoryImport::do_nothing, this));
    set_transition(State::NODE_DEFINED, Token::COLON, State::EXPECT_NODE_LABEL, std::bind(&InMemoryImport::do_nothing, this));

    set_transition(State::FIRST_ID,     Token::ENDLINE, State::LINE_BEGIN, std::bind(&InMemoryImport::finish_node_line, this));
    set_transition(State::NODE_DEFINED, Token::ENDLINE, State::LINE_BEGIN, std::bind(&InMemoryImport::finish_node_line, this));

    // TODO: accept IRI as label?
    set_transition(State::EXPECT_NODE_LABEL, Token::IDENTIFIER, State::NODE_DEFINED, std::bind(&InMemoryImport::add_node_label, this));

    set_transition(State::FIRST_ID,     Token::IDENTIFIER, State::EXPECT_NODE_PROP_COLON, std::bind(&InMemoryImport::save_prop_key, this));
    set_transition(State::NODE_DEFINED, Token::IDENTIFIER, State::EXPECT_NODE_PROP_COLON, std::bind(&InMemoryImport::save_prop_key, this));

    set_transition(State::EXPECT_NODE_PROP_COLON, Token::COLON, State::EXPECT_NODE_PROP_VALUE, std::bind(&InMemoryImport::do_nothing, this));

    // TODO: accept IRI as prop value?
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::STRING,  State::NODE_DEFINED, std::bind(&InMemoryImport::add_node_prop_string, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::INTEGER, State::NODE_DEFINED, std::bind(&InMemoryImport::add_node_prop_int, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::FLOAT,   State::NODE_DEFINED, std::bind(&InMemoryImport::add_node_prop_float, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::FALSE,   State::NODE_DEFINED, std::bind(&InMemoryImport::add_node_prop_false, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::TRUE,    State::NODE_DEFINED, std::bind(&InMemoryImport::add_node_prop_true, this));

    set_transition(State::IMPLICIT_EDGE, Token::L_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&InMemoryImport::set_left_direction, this));
    set_transition(State::IMPLICIT_EDGE, Token::R_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&InMemoryImport::set_right_direction, this));
    set_transition(State::FIRST_ID,      Token::L_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&InMemoryImport::set_left_direction, this));
    set_transition(State::FIRST_ID,      Token::R_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&InMemoryImport::set_right_direction, this));

    set_transition(State::EXPECT_EDGE_SECOND, Token::IDENTIFIER, State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_identifier, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::ANON,       State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_anon, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::STRING,     State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_string, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::IRI,        State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_iri, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::INTEGER,    State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_int, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::FLOAT,      State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_float, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::TRUE,       State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_true, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::FALSE,      State::EXPECT_EDGE_TYPE_COLON, std::bind(&InMemoryImport::save_second_id_false, this));

    set_transition(State::EXPECT_EDGE_TYPE_COLON, Token::COLON, State::EXPECT_EDGE_TYPE, std::bind(&InMemoryImport::do_nothing, this));

    // TODO: accept IRI as type?
    set_transition(State::EXPECT_EDGE_TYPE, Token::IDENTIFIER, State::EDGE_DEFINED, std::bind(&InMemoryImport::save_edge_type, this));

    set_transition(State::EDGE_DEFINED, Token::ENDLINE,    State::LINE_BEGIN, std::bind(&InMemoryImport::finish_edge_line, this));
    set_transition(State::EDGE_DEFINED, Token::IDENTIFIER, State::EXPECT_EDGE_PROP_COLON, std::bind(&InMemoryImport::save_prop_key, this));

    set_transition(State::EXPECT_EDGE_PROP_COLON, Token::COLON, State::EXPECT_EDGE_PROP_VALUE, std::bind(&InMemoryImport::do_nothing, this));

    // TODO: accept IRI as prop value?
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::STRING,  State::EDGE_DEFINED, std::bind(&InMemoryImport::add_edge_prop_string, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::INTEGER, State::EDGE_DEFINED, std::bind(&InMemoryImport::add_edge_prop_int, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::FLOAT,   State::EDGE_DEFINED, std::bind(&InMemoryImport::add_edge_prop_float, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::FALSE,   State::EDGE_DEFINED, std::bind(&InMemoryImport::add_edge_prop_false, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::TRUE,    State::EDGE_DEFINED, std::bind(&InMemoryImport::add_edge_prop_true, this));
}
