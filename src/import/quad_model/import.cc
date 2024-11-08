#include "import.h"

#include <chrono>

#include "import/import_helper.h"
#include "import/stats_processor.h"
#include "misc/fatal_error.h"
#include "storage/index/hash/strings_hash/strings_hash_bulk_ondisk_import.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"

using namespace Import::QuadModel;


void OnDiskImport::start_import(const std::string& input_filename) {
    auto start = std::chrono::system_clock::now();
    auto import_start = start;

    lexer.begin(input_filename);

    { // Initial memory allocation
        external_strings = reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(VPage::SIZE, buffer_size));

        if (external_strings == nullptr) {
            FATAL_ERROR("Could not allocate buffer, try using a smaller buffer size");
        }

        Import::ExternalString::strings = external_strings;
        external_strings_capacity = buffer_size;
        external_strings_end = StringManager::METADATA_SIZE;
        assert(external_strings_capacity > StringManager::STRING_BLOCK_SIZE);
    }
    catalog.anonymous_nodes_count = 0;

    int current_state = State::LINE_BEGIN;
    current_line = 1;
    while (int token = lexer.next_token()) {
        current_state = get_transition(current_state, token);
    }
    // After EOF simulate and endline
    get_transition(current_state, Token::ENDLINE);

    std::cout << "-------------------------------------\n";
    if (parsing_errors != 0) {
        std::cout << "\n!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!\n";
        std::cout << "  " << parsing_errors << " errors found.\n";
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
    }
    print_duration("Parsing", start);

    // Finish Strings File
    std::fstream strings_file;
    strings_file.open(db_folder + "/strings.dat", std::ios::out | std::ios::binary);
    strings_file.write(external_strings, external_strings_end);

    // std::cout << "external_strings_end: " << external_strings_end << std::endl;

    // Round up to strings file to be a multiple of StringManager::STRING_BLOCK_SIZE
    uint64_t last_str_pos = strings_file.tellp();
    uint64_t last_block_offset = last_str_pos % StringManager::STRING_BLOCK_SIZE;
    uint64_t remaining = StringManager::STRING_BLOCK_SIZE - last_block_offset;

    // can copy anything, content doesn't matter, but setting zeros is better for debug
    memset(external_strings, '\0', remaining);
    strings_file.write(external_strings, remaining);

    // Write strings_file metadata
    strings_file.seekp(0, strings_file.beg);
    strings_file.write(reinterpret_cast<char*>(&last_block_offset), sizeof(uint64_t));

    print_duration("Processing strings", start);

    { // Destroy external_strings_set replacing it with an empty map
        robin_hood::unordered_set<ExternalString> tmp;
        external_strings_set.swap(tmp);
    }

    { // Create StringsHash
        // Big enough buffer to store any string
        char* string_buffer =
            reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(VPage::SIZE, StringManager::STRING_BLOCK_SIZE));

        // we reuse the allocated memory for external strings as a buffer
        StringsHashBulkOnDiskImport strings_hash(
            db_folder + "/str_hash",
            external_strings,
            external_strings_capacity
        );
        strings_file.close();
        strings_file.open(db_folder + "/strings.dat", std::ios::in | std::ios::binary);
        strings_file.seekg(StringManager::METADATA_SIZE, strings_file.beg);
        uint64_t current_pos = StringManager::METADATA_SIZE;

        // read all strings one by one and add them to the StringsHash
        while (current_pos < last_str_pos) {
            size_t remaining_in_block =
                StringManager::STRING_BLOCK_SIZE - (current_pos % StringManager::STRING_BLOCK_SIZE);

            if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
                current_pos += remaining_in_block;
                strings_file.read(string_buffer, remaining_in_block);
            }

            auto str_len = read_str(strings_file, string_buffer);
            strings_hash.create_id(string_buffer, current_pos, str_len);
            current_pos = strings_file.tellg();
        }
        strings_file.close();
        MDB_ALIGNED_FREE(string_buffer);
    }

    print_duration("Write strings and strings hashes", start);

    // we reuse the buffer for external strings in the B+trees creation
    char* const buffer = external_strings;
    buffer_size = external_strings_capacity;

    // Save lasts blocks to disk
    declared_nodes.finish_appends();
    labels.finish_appends();
    properties.finish_appends();
    edges.finish_appends();
    equal_from_to.finish_appends();
    equal_from_to_type.finish_appends();
    equal_from_type.finish_appends();
    equal_to_type.finish_appends();

    { // Append undeclared nodes (being on an edge)
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
                declared_nodes.push_back({ tuple[0] });
            }
            if (nodes_set.insert(tuple[1]).second) {
                declared_nodes.push_back({ tuple[1] });
            }
            if (nodes_set.insert(tuple[2]).second) {
                declared_nodes.push_back({ tuple[2] });
            }
        }
        // declared_nodes.finish_appends() its called twice, no problem with that
        declared_nodes.finish_appends();

        catalog.identifiable_nodes_count = nodes_set.size();
    }
    print_duration("Write table", start);

    declared_nodes.start_indexing(buffer, buffer_size, { 0 });
    labels.start_indexing(buffer, buffer_size, { 0, 1 });
    properties.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    edges.start_indexing(buffer, buffer_size, { 0, 1, 2, 3 });
    equal_from_to_type.start_indexing(buffer, buffer_size, { 0, 1 });
    equal_from_to.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    equal_from_type.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    equal_to_type.start_indexing(buffer, buffer_size, { 0, 1, 2 });


    { // Nodes B+Tree
        size_t COL_NODE = 0;
        NoStat<1> no_stat;

        declared_nodes.create_bpt(db_folder + "/nodes", { COL_NODE }, no_stat);
    }

    { // Labels B+Tree
        size_t COL_NODE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        LabelStat label_stat;

        labels.create_bpt(db_folder + "/node_label", { COL_NODE, COL_LABEL }, no_stat);

        labels.create_bpt(db_folder + "/label_node", { COL_LABEL, COL_NODE }, label_stat);
        catalog.label_count = label_stat.all;
        label_stat.end();

        catalog.label2total_count = std::move(label_stat.map_label_count);
    }

    { // Properties B+Tree
        size_t COL_OBJ = 0, COL_KEY = 1, COL_VALUE = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        properties.create_bpt(db_folder + "/object_key_value", { COL_OBJ, COL_KEY, COL_VALUE }, no_stat);

        properties.create_bpt(db_folder + "/key_value_object", { COL_KEY, COL_VALUE, COL_OBJ }, prop_stat);
        catalog.properties_count = prop_stat.all;
        prop_stat.end();

        catalog.key2total_count = std::move(prop_stat.map_key_count);
    }

    { // Quad B+Trees
        size_t COL_FROM = 0, COL_TO = 1, COL_TYPE = 2, COL_EDGE = 3;

        NoStat<4> no_stat;
        AllStat<4> all_stat;

        edges.create_bpt(
            db_folder + "/from_to_type_edge",
            { COL_FROM, COL_TO, COL_TYPE, COL_EDGE },
            all_stat
        );
        catalog.edge_count = all_stat.all;

        edges.create_bpt(
            db_folder + "/to_type_from_edge",
            { COL_TO, COL_TYPE, COL_FROM, COL_EDGE },
            no_stat
        );

        edges.create_bpt(db_folder + "/type_from_to_edge", { COL_TYPE, COL_FROM, COL_TO, COL_EDGE }, no_stat);

        edges.create_bpt(db_folder + "/type_to_from_edge", { COL_TYPE, COL_TO, COL_FROM, COL_EDGE }, no_stat);
    }

    { // FROM=TO=TYPE EDGE
        size_t COL_FROM_TO_TYPE = 0, COL_EDGE = 1;

        DictCountStat<2> stat;
        equal_from_to_type
            .create_bpt(db_folder + "/equal_from_to_type", { COL_FROM_TO_TYPE, COL_EDGE }, stat);
        catalog.equal_from_to_type_count = stat.all;

        stat.end();
        catalog.type2equal_from_to_type_count = std::move(stat.dict);
    }

    { // FROM=TO TYPE EDGE
        size_t COL_FROM_TO = 0, COL_TYPE = 1, COL_EDGE = 2;

        NoStat<3> no_stat;
        equal_from_to.create_bpt(db_folder + "/equal_from_to", { COL_FROM_TO, COL_TYPE, COL_EDGE }, no_stat);

        DictCountStat<3> stat;
        equal_from_to
            .create_bpt(db_folder + "/equal_from_to_inverted", { COL_TYPE, COL_FROM_TO, COL_EDGE }, stat);

        stat.end();

        catalog.equal_from_to_count = stat.all;
        catalog.type2equal_from_to_count = std::move(stat.dict);
    }

    { // FROM=TYPE TO EDGE
        size_t COL_FROM_TYPE = 0, COL_TO = 1, COL_EDGE = 2;

        DictCountStat<3> stat;
        equal_from_type.create_bpt(db_folder + "/equal_from_type", { COL_FROM_TYPE, COL_TO, COL_EDGE }, stat);
        stat.end();
        catalog.equal_from_type_count = stat.all;
        catalog.type2equal_from_type_count = std::move(stat.dict);

        NoStat<3> no_stat;
        equal_from_type.create_bpt(
            db_folder + "/equal_from_type_inverted",
            { COL_TO, COL_FROM_TYPE, COL_EDGE },
            no_stat
        );
    }

    { // TO=TYPE FROM EDGE
        size_t COL_TO_TYPE = 0, COL_FROM = 1, COL_EDGE = 2;

        DictCountStat<3> stat;
        equal_to_type.create_bpt(db_folder + "/equal_to_type", { COL_TO_TYPE, COL_FROM, COL_EDGE }, stat);
        stat.end();
        catalog.equal_to_type_count = stat.all;
        catalog.type2equal_to_type_count = std::move(stat.dict);

        NoStat<3> no_stat;
        equal_to_type
            .create_bpt(db_folder + "/equal_to_type_inverted", { COL_FROM, COL_TO_TYPE, COL_EDGE }, no_stat);
    }
    // calling finish_indexing() closes and removes the file.
    declared_nodes.finish_indexing();
    properties.finish_indexing();
    labels.finish_indexing();
    edges.finish_indexing();
    equal_from_to_type.finish_indexing();
    equal_from_to.finish_indexing();
    equal_from_type.finish_indexing();
    equal_to_type.finish_indexing();
    MDB_ALIGNED_FREE(external_strings);

    print_duration("Write B+tree indexes", start);

    catalog.print(std::cout);

    print_duration("Write catalog", start);
    print_duration("Total Import", import_start);
}


void OnDiskImport::create_automata() {
    // set all transitions as error at first, transitions that are defined later will stay as error.
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        for (int t = 1; t < Token::TOTAL_TOKENS; t++) {
            set_transition(s, t, State::WRONG_LINE, std::bind(&OnDiskImport::print_error, this));
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
    set_transition(
        State::WRONG_LINE,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_wrong_line, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_wrong_line, this)
    );

    set_transition(
        State::LINE_BEGIN,
        Token::IDENTIFIER,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_identifier, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::ANON,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_anon, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::STRING,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_string, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::INTEGER,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_int, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::FLOAT,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_float, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::TRUE,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_true, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::FALSE,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_false, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::IMPLICIT,
        State::IMPLICIT_EDGE,
        std::bind(&OnDiskImport::save_first_id_implicit, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::COLON,
        State::EXPECT_NODE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );
    set_transition(
        State::NODE_DEFINED,
        Token::COLON,
        State::EXPECT_NODE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_node_line, this)
    );
    set_transition(
        State::NODE_DEFINED,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_node_line, this)
    );

    set_transition(
        State::EXPECT_NODE_LABEL,
        Token::IDENTIFIER,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_label, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::IDENTIFIER,
        State::EXPECT_NODE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );
    set_transition(
        State::NODE_DEFINED,
        Token::IDENTIFIER,
        State::EXPECT_NODE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_COLON,
        Token::COLON,
        State::EXPECT_NODE_PROP_VALUE,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::TYPED_STRING,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_datatype, this)
    );
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::STRING,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_string, this)
    );
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::INTEGER,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_int, this)
    );
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::FLOAT,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_float, this)
    );
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::FALSE,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_false, this)
    );
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::TRUE,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_true, this)
    );

    set_transition(
        State::IMPLICIT_EDGE,
        Token::L_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::set_left_direction, this)
    );
    set_transition(
        State::IMPLICIT_EDGE,
        Token::R_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::set_right_direction, this)
    );
    set_transition(
        State::FIRST_ID,
        Token::L_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::set_left_direction, this)
    );
    set_transition(
        State::FIRST_ID,
        Token::R_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::set_right_direction, this)
    );

    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::IDENTIFIER,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_identifier, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::ANON,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_anon, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::STRING,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_string, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::INTEGER,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_int, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::FLOAT,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_float, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::TRUE,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_true, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::FALSE,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_false, this)
    );

    set_transition(
        State::EXPECT_EDGE_TYPE_COLON,
        Token::COLON,
        State::EXPECT_EDGE_TYPE,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_EDGE_TYPE,
        Token::IDENTIFIER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_edge_type, this)
    );

    set_transition(
        State::EDGE_DEFINED,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_edge_line, this)
    );
    set_transition(
        State::EDGE_DEFINED,
        Token::IDENTIFIER,
        State::EXPECT_EDGE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_COLON,
        Token::COLON,
        State::EXPECT_EDGE_PROP_VALUE,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::TYPED_STRING,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_datatype, this)
    );
    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::STRING,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_string, this)
    );
    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::INTEGER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_int, this)
    );
    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::FLOAT,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_float, this)
    );
    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::FALSE,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_false, this)
    );
    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::TRUE,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_true, this)
    );
}
