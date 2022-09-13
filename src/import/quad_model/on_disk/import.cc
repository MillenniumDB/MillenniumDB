#include "import.h"

#include <chrono>

#include "storage/index/hash/object_file_hash/object_file_hash_mem_import.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"
#include "stats_processor.h"

using namespace Import;

char* ExternalString::strings = nullptr;

void OnDiskImport::start_import(const std::string& input_filename) {
    auto start = std::chrono::system_clock::now();
    lexer.begin(input_filename);

    // we only use half of what the user gave us because hasmap will use a significant amount of memory
    size_t external_strings_initial_size = (1024ULL * 1024ULL * 1024ULL * buffer_size_in_GB)/2;

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

    declared_nodes.finish_appends();
    labels.finish_appends();
    properties.finish_appends();
    edges.finish_appends();
    equal_from_to.finish_appends();
    equal_from_to_type.finish_appends();
    equal_from_type.finish_appends();
    equal_to_type.finish_appends();

    {   // Destroy external_ids_map replacing it with an empty map
        robin_hood::unordered_set<ExternalString, ExternalStringHasher> tmp;
        external_ids_map.swap(tmp);
    }

    {   // Write ObjectFile and ObjectFileHash
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

    auto end_obj_file = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> obj_duration = end_obj_file - end_lexer;
    std::cout << "Write obj file and obj file hash duration: " << obj_duration.count() << " ms\n";

    // Append undeclared nodes (being on an edge)
    {
        robin_hood::unordered_set<uint64_t> nodes_set;

        declared_nodes.begin_tuple_iter();
        while (declared_nodes.has_next_tuple()) {
            auto& tuple = declared_nodes.next_tuple();
            nodes_set.insert(tuple[0]);
        }

        EdgeTableMemImport table_writer(db_folder + "/edges.table");

        edges.begin_tuple_iter();
        while (edges.has_next_tuple()) {
            auto& tuple = edges.next_tuple();
            table_writer.insert_tuple(tuple);
            if (nodes_set.insert(tuple[0]).second) {
                declared_nodes.push_back({tuple[0]});
            }
            if (nodes_set.insert(tuple[1]).second) {
                declared_nodes.push_back({tuple[1]});
            }
            if (nodes_set.insert(tuple[2]).second) {
                declared_nodes.push_back({tuple[2]});
            }
        }
        // declared_nodes.finish_appends() its called twice, no problem with that
        declared_nodes.finish_appends();

        catalog.identifiable_nodes_count = nodes_set.size();
    }

    size_t buffer_size = 1024ULL * 1024ULL * 1024ULL * buffer_size_in_GB;
    char* buffer = reinterpret_cast<char*>(std::aligned_alloc(Page::MDB_PAGE_SIZE, buffer_size));

    { // Nodes B+Tree
        size_t COL_NODE = 0;
        std::array<size_t, 1> original_permutation = { COL_NODE };

        NoStat<1> no_stat;

        declared_nodes.start_indexing(buffer, buffer_size, original_permutation);
        declared_nodes.create_bpt(db_folder + "/nodes",
                                  { COL_NODE },
                                   no_stat);
        declared_nodes.finish_indexing();
    }
    auto end_nodes_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> nodes_index_duration = end_nodes_index - end_obj_file;
    std::cout << "Write edge table and nodes index: " << nodes_index_duration.count() << " ms\n";

    { // Labels B+Tree
        size_t COL_NODE = 0, COL_LABEL = 1;
        std::array<size_t, 2> original_permutation = { COL_NODE, COL_LABEL };

        NoStat<2> no_stat;
        LabelStat label_stat;

        labels.start_indexing(buffer, buffer_size, original_permutation);
        catalog.label_count = labels.total_tuples;
        labels.create_bpt(db_folder + "/node_label",
                          { COL_NODE, COL_LABEL },
                          no_stat);

        labels.create_bpt(db_folder + "/label_node",
                          { COL_LABEL, COL_NODE },
                          label_stat);
        label_stat.end();
        labels.finish_indexing();

        catalog.distinct_labels   = label_stat.map_label_count.size();
        catalog.label2total_count = move(label_stat.map_label_count);
    }
    auto end_label_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> label_index_duration = end_label_index - end_nodes_index;
    std::cout << "Write labels index: " << label_index_duration.count() << " ms\n";

    { // Properties B+Tree
        size_t COL_OBJ = 0, COL_KEY = 1, COL_VALUE = 2;
        std::array<size_t, 3> original_permutation = { COL_OBJ, COL_KEY, COL_VALUE };

        NoStat<3> no_stat;
        PropStat prop_stat;

        properties.start_indexing(buffer, buffer_size, original_permutation);
        catalog.properties_count = properties.total_tuples;
        properties.create_bpt(db_folder + "/object_key_value",
                              { COL_OBJ, COL_KEY, COL_VALUE },
                              no_stat);

        properties.create_bpt(db_folder + "/key_value_object",
                              { COL_KEY, COL_VALUE, COL_OBJ },
                              prop_stat);
        prop_stat.end();

        properties.finish_indexing();

        catalog.key2distinct    = move(prop_stat.map_distinct_values);
        catalog.key2total_count = move(prop_stat.map_key_count);
        catalog.distinct_keys   = catalog.key2total_count.size();

    }
    auto end_prop_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> prop_index_duration = end_prop_index - end_label_index;
    std::cout << "Write properties index: " << prop_index_duration.count() << " ms\n";

    { // Quad B+Trees
        size_t COL_FROM = 0, COL_TO = 1, COL_TYPE = 2, COL_EDGE = 3;
        std::array<size_t, 4> original_permutation = { COL_FROM, COL_TO, COL_TYPE, COL_EDGE };

        edges.start_indexing(buffer, buffer_size, original_permutation);

        NoStat<4> no_stat;
        DistinctStat<4> distinct_from_stat;
        DistinctStat<4> distinct_to_stat;

        catalog.connections_count = edges.total_tuples;
        edges.create_bpt(db_folder + "/from_to_type_edge",
                         { COL_FROM, COL_TO, COL_TYPE, COL_EDGE },
                         distinct_from_stat);
        catalog.distinct_from = distinct_from_stat.distinct;

        edges.create_bpt(db_folder + "/to_type_from_edge",
                         { COL_TO, COL_TYPE, COL_FROM, COL_EDGE },
                         distinct_to_stat);
        catalog.distinct_to = distinct_to_stat.distinct;

        edges.create_bpt(db_folder + "/type_from_to_edge",
                         { COL_TYPE, COL_FROM, COL_TO, COL_EDGE },
                         no_stat);

        edges.create_bpt(db_folder + "/type_to_from_edge",
                         { COL_TYPE, COL_TO, COL_FROM, COL_EDGE },
                         no_stat);

        // set distinct_type, may be a redundant stat
        catalog.distinct_type = catalog.type2total_count.size();

        edges.finish_indexing();
    }

    auto end_quad_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> quad_index_duration = end_quad_index - end_prop_index;
    std::cout << "Write quad index: " << quad_index_duration.count() << " ms\n";

    {   // FROM=TO=TYPE EDGE
        size_t COL_FROM_TO_TYPE = 0, COL_EDGE = 1;
        std::array<size_t, 2> original_permutation = { COL_FROM_TO_TYPE, COL_EDGE };
        equal_from_to_type.start_indexing(buffer, buffer_size, original_permutation);

        DictCountStat<2> stat;
        catalog.equal_from_to_type_count = equal_from_to_type.total_tuples;
        equal_from_to_type.create_bpt(db_folder + "/equal_from_to_type",
                                      { COL_FROM_TO_TYPE, COL_EDGE },
                                      stat);

        stat.end();
        catalog.type2equal_from_to_type_count = move(stat.dict);
        equal_from_to_type.finish_indexing();
    }

    {   // FROM=TO TYPE EDGE
        size_t COL_FROM_TO = 0, COL_TYPE = 1, COL_EDGE = 2;
        std::array<size_t, 3> original_permutation = { COL_FROM_TO, COL_TYPE, COL_EDGE };
        equal_from_to.start_indexing(buffer, buffer_size, original_permutation);

        NoStat<3> no_stat;
        catalog.equal_from_to_count = equal_from_to.total_tuples;
        equal_from_to.create_bpt(db_folder + "/equal_from_to",
                                 { COL_FROM_TO, COL_TYPE, COL_EDGE },
                                 no_stat);

        DictCountStat<3> stat;
        equal_from_to.create_bpt(db_folder + "/equal_from_to_inverted",
                                 { COL_TYPE, COL_FROM_TO, COL_EDGE },
                                 stat);

        stat.end();

        catalog.type2equal_from_to_count = move(stat.dict);
        equal_from_to.finish_indexing();
    }

    {   // FROM=TYPE TO EDGE
        size_t COL_FROM_TYPE = 0, COL_TO = 1, COL_EDGE = 2;
        std::array<size_t, 3> original_permutation = { COL_FROM_TYPE, COL_TO, COL_EDGE };
        equal_from_type.start_indexing(buffer, buffer_size, original_permutation);

        DictCountStat<3> stat;
        catalog.equal_from_type_count = equal_from_type.total_tuples;
        equal_from_type.create_bpt(db_folder + "/equal_from_type",
                                   { COL_FROM_TYPE, COL_TO, COL_EDGE },
                                   stat);
        stat.end();
        catalog.type2equal_from_type_count = move(stat.dict);

        NoStat<3> no_stat;
        equal_from_type.create_bpt(db_folder + "/equal_from_type_inverted",
                                 { COL_TO, COL_FROM_TYPE, COL_EDGE },
                                 no_stat);

        equal_from_type.finish_indexing();
    }

    {   // TO=TYPE FROM EDGE
        size_t COL_TO_TYPE = 0, COL_FROM = 1, COL_EDGE = 2;
        std::array<size_t, 3> original_permutation = { COL_TO_TYPE, COL_FROM, COL_EDGE };
        equal_to_type.start_indexing(buffer, buffer_size, original_permutation);

        DictCountStat<3> stat;
        catalog.equal_to_type_count = equal_to_type.total_tuples;
        equal_to_type.create_bpt(db_folder + "/equal_to_type",
                                 { COL_TO_TYPE, COL_FROM, COL_EDGE },
                                 stat);
        stat.end();
        catalog.type2equal_to_type_count = move(stat.dict);

        NoStat<3> no_stat;
        equal_to_type.create_bpt(db_folder + "/equal_to_type_inverted",
                                 { COL_FROM, COL_TO_TYPE, COL_EDGE },
                                 no_stat);

        equal_to_type.finish_indexing();
    }

    auto end_special_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> special_index_duration = end_special_index - end_quad_index;
    std::cout << "Write special cases index: " << special_index_duration.count() << " ms\n";

    std::chrono::duration<float, std::milli> total_duration = end_special_index - start;
    std::cout << "Total duration: " << total_duration.count() << " ms\n";


    catalog.print();
    catalog.save_changes();
}

void OnDiskImport::create_automata() {
    state_transitions = new int[Token::TOTAL_TOKENS*State::TOTAL_STATES];

    // llenar vacío
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        for (int t = 1; t < Token::TOTAL_TOKENS; t++) {
            set_transition(s, t, State::WRONG_LINE, std::bind(&OnDiskImport::print_error, this) );
        }
    }
    // ignore whitespace token
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        set_transition(s, Token::WHITESPACE, s, std::bind(&OnDiskImport::do_nothing, this));
    }

     // wrong line stays wrong (without giving more errors) until endline
    for (int t = 0; t < Token::TOTAL_TOKENS; t++) {
        set_transition(State::WRONG_LINE, t, State::WRONG_LINE, std::bind(&OnDiskImport::do_nothing, this));
    }
    set_transition(State::WRONG_LINE, Token::ENDLINE, State::LINE_BEGIN, std::bind(&OnDiskImport::finish_wrong_line, this));

    set_transition(State::LINE_BEGIN, Token::IDENTIFIER, State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_identifier, this));
    set_transition(State::LINE_BEGIN, Token::ANON,       State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_anon, this));
    set_transition(State::LINE_BEGIN, Token::STRING,     State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_string, this));
    set_transition(State::LINE_BEGIN, Token::IRI,        State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_iri, this));
    set_transition(State::LINE_BEGIN, Token::INTEGER,    State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_int, this));
    set_transition(State::LINE_BEGIN, Token::FLOAT,      State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_float, this));
    set_transition(State::LINE_BEGIN, Token::TRUE,       State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_true, this));
    set_transition(State::LINE_BEGIN, Token::FALSE,      State::FIRST_ID, std::bind(&OnDiskImport::save_first_id_false, this));
    set_transition(State::LINE_BEGIN, Token::IMPLICIT,   State::IMPLICIT_EDGE, std::bind(&OnDiskImport::save_first_id_implicit, this));

    set_transition(State::FIRST_ID,     Token::COLON, State::EXPECT_NODE_LABEL, std::bind(&OnDiskImport::do_nothing, this));
    set_transition(State::NODE_DEFINED, Token::COLON, State::EXPECT_NODE_LABEL, std::bind(&OnDiskImport::do_nothing, this));

    set_transition(State::FIRST_ID,     Token::ENDLINE, State::LINE_BEGIN, std::bind(&OnDiskImport::finish_node_line, this));
    set_transition(State::NODE_DEFINED, Token::ENDLINE, State::LINE_BEGIN, std::bind(&OnDiskImport::finish_node_line, this));

    // TODO: accept IRI as label?
    set_transition(State::EXPECT_NODE_LABEL, Token::IDENTIFIER, State::NODE_DEFINED, std::bind(&OnDiskImport::add_node_label, this));

    set_transition(State::FIRST_ID,     Token::IDENTIFIER, State::EXPECT_NODE_PROP_COLON, std::bind(&OnDiskImport::save_prop_key, this));
    set_transition(State::NODE_DEFINED, Token::IDENTIFIER, State::EXPECT_NODE_PROP_COLON, std::bind(&OnDiskImport::save_prop_key, this));

    set_transition(State::EXPECT_NODE_PROP_COLON, Token::COLON, State::EXPECT_NODE_PROP_VALUE, std::bind(&OnDiskImport::do_nothing, this));

    // TODO: accept IRI as prop value?
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::STRING,  State::NODE_DEFINED, std::bind(&OnDiskImport::add_node_prop_string, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::INTEGER, State::NODE_DEFINED, std::bind(&OnDiskImport::add_node_prop_int, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::FLOAT,   State::NODE_DEFINED, std::bind(&OnDiskImport::add_node_prop_float, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::FALSE,   State::NODE_DEFINED, std::bind(&OnDiskImport::add_node_prop_false, this));
    set_transition(State::EXPECT_NODE_PROP_VALUE, Token::TRUE,    State::NODE_DEFINED, std::bind(&OnDiskImport::add_node_prop_true, this));

    set_transition(State::IMPLICIT_EDGE, Token::L_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&OnDiskImport::set_left_direction, this));
    set_transition(State::IMPLICIT_EDGE, Token::R_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&OnDiskImport::set_right_direction, this));
    set_transition(State::FIRST_ID,      Token::L_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&OnDiskImport::set_left_direction, this));
    set_transition(State::FIRST_ID,      Token::R_ARROW, State::EXPECT_EDGE_SECOND, std::bind(&OnDiskImport::set_right_direction, this));

    set_transition(State::EXPECT_EDGE_SECOND, Token::IDENTIFIER, State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_identifier, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::ANON,       State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_anon, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::STRING,     State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_string, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::IRI,        State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_iri, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::INTEGER,    State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_int, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::FLOAT,      State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_float, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::TRUE,       State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_true, this));
    set_transition(State::EXPECT_EDGE_SECOND, Token::FALSE,      State::EXPECT_EDGE_TYPE_COLON, std::bind(&OnDiskImport::save_second_id_false, this));

    set_transition(State::EXPECT_EDGE_TYPE_COLON, Token::COLON, State::EXPECT_EDGE_TYPE, std::bind(&OnDiskImport::do_nothing, this));

    // TODO: accept IRI as type?
    set_transition(State::EXPECT_EDGE_TYPE, Token::IDENTIFIER, State::EDGE_DEFINED, std::bind(&OnDiskImport::save_edge_type, this));

    set_transition(State::EDGE_DEFINED, Token::ENDLINE,    State::LINE_BEGIN, std::bind(&OnDiskImport::finish_edge_line, this));
    set_transition(State::EDGE_DEFINED, Token::IDENTIFIER, State::EXPECT_EDGE_PROP_COLON, std::bind(&OnDiskImport::save_prop_key, this));

    set_transition(State::EXPECT_EDGE_PROP_COLON, Token::COLON, State::EXPECT_EDGE_PROP_VALUE, std::bind(&OnDiskImport::do_nothing, this));

    // TODO: accept IRI as prop value?
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::STRING,  State::EDGE_DEFINED, std::bind(&OnDiskImport::add_edge_prop_string, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::INTEGER, State::EDGE_DEFINED, std::bind(&OnDiskImport::add_edge_prop_int, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::FLOAT,   State::EDGE_DEFINED, std::bind(&OnDiskImport::add_edge_prop_float, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::FALSE,   State::EDGE_DEFINED, std::bind(&OnDiskImport::add_edge_prop_false, this));
    set_transition(State::EXPECT_EDGE_PROP_VALUE, Token::TRUE,    State::EDGE_DEFINED, std::bind(&OnDiskImport::add_edge_prop_true, this));
}
