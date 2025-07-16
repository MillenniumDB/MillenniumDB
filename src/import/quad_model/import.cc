#include "import.h"

#include <chrono>

#include "import/import_helper.h"
#include "import/stats_processor.h"

using namespace Import::QuadModel;

void OnDiskImport::start_import(MDBIstream& in)
{
    auto start = std::chrono::system_clock::now();
    auto import_start = start;

    pending_declared_nodes = std::make_unique<DiskVector<1>>(
        db_folder + "/" + PENDING_DECLARED_NODES_FILENAME_PREFIX
    );
    pending_labels = std::make_unique<DiskVector<2>>(db_folder + "/" + PENDING_LABELS_FILENAME_PREFIX);
    pending_properties = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_PROPERTIES_FILENAME_PREFIX
    );
    pending_edges = std::make_unique<DiskVector<4>>(db_folder + "/" + PENDING_EDGES_FILENAME_PREFIX);

    // Initialize external helper
    external_helper = std::make_unique<ExternalHelper>(db_folder, strings_buffer_size, tensors_buffer_size);

    lexer.begin(in);

    int current_state = State::LINE_BEGIN;
    current_line = 1;
    while (auto token = lexer.get_token()) {
        // std::cout << "token: " << token << "\n";
        // std::cout << "str_len: " << lexer.str_len << "\n";
        // std::cout.write(lexer.str, lexer.str_len);
        current_state = get_transition(current_state, token);
        // std::cout << "\ncurrect_state: " << current_state << "\n";
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

    // initial flush
    external_helper->flush_to_disk();

    { // process pending files
        pending_declared_nodes->finish_appends();
        pending_labels->finish_appends();
        pending_properties->finish_appends();
        pending_edges->finish_appends();

        int i = 0;
        while (true) {
            const auto total_pending = pending_declared_nodes->get_total_tuples()
                                     + pending_labels->get_total_tuples()
                                     + pending_properties->get_total_tuples()
                                     + pending_edges->get_total_tuples();
            if (total_pending == 0) {
                break;
            }
            std::cout << "total pending: " << total_pending << std::endl;

            auto old_pending_declared_nodes = std::move(pending_declared_nodes);
            auto old_pending_labels = std::move(pending_labels);
            auto old_pending_properties = std::move(pending_properties);
            auto old_pending_edges = std::move(pending_edges);

            pending_declared_nodes = std::make_unique<DiskVector<1>>(
                db_folder + "/" + PENDING_DECLARED_NODES_FILENAME_PREFIX + std::to_string(i)
            );
            pending_labels = std::make_unique<DiskVector<2>>(
                db_folder + "/" + PENDING_LABELS_FILENAME_PREFIX + std::to_string(i)
            );
            pending_properties = std::make_unique<DiskVector<3>>(
                db_folder + "/" + PENDING_PROPERTIES_FILENAME_PREFIX + std::to_string(i)
            );
            pending_edges = std::make_unique<DiskVector<4>>(
                db_folder + "/" + PENDING_EDGES_FILENAME_PREFIX + std::to_string(i)
            );
            ++i;

            // advance pending variables for current iteration
            external_helper->advance_pending();
            external_helper->clear_sets();

            old_pending_declared_nodes->begin_tuple_iter();
            while (old_pending_declared_nodes->has_next_tuple()) {
                const auto& pending_tuple = old_pending_declared_nodes->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);

                try_save_declared_node();
            }

            old_pending_labels->begin_tuple_iter();
            while (old_pending_labels->has_next_tuple()) {
                const auto& pending_tuple = old_pending_labels->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                label_id = external_helper->resolve_id(pending_tuple[1]);

                try_save_label();
            }

            old_pending_properties->begin_tuple_iter();
            while (old_pending_properties->has_next_tuple()) {
                const auto& pending_tuple = old_pending_properties->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                key_id = external_helper->resolve_id(pending_tuple[1]);
                value_id = external_helper->resolve_id(pending_tuple[2]);

                try_save_property(id1);
            }

            old_pending_edges->begin_tuple_iter();
            while (old_pending_edges->has_next_tuple()) {
                const auto& pending_tuple = old_pending_edges->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                id2 = external_helper->resolve_id(pending_tuple[1]);
                type_id = external_helper->resolve_id(pending_tuple[2]);
                edge_id = pending_tuple[3]; // always inlined

                try_save_quad<true>(); // was stored as right-directed
            }

            // write out new data
            external_helper->flush_to_disk();
            // close and delete the old pending files
            external_helper->clean_up_old();

            // close and delete old pending file
            pending_declared_nodes->finish_appends();
            pending_labels->finish_appends();
            pending_properties->finish_appends();
            pending_edges->finish_appends();

            old_pending_declared_nodes->skip_indexing(); // will close and remove file
            old_pending_labels->skip_indexing(); // will close and remove file
            old_pending_properties->skip_indexing(); // will close and remove file
            old_pending_edges->skip_indexing(); // will close and remove file
        }

        // process pending finished, clean up the last pending file
        pending_declared_nodes->skip_indexing();
        pending_labels->skip_indexing();
        pending_properties->skip_indexing();
        pending_edges->skip_indexing();
    }

    // delete all unnecessary files and free-up memory
    external_helper->clean_up();

    print_duration("Process strings and tensors", start);

    external_helper->build_disk_hash();

    print_duration("Write strings and tensors hashes", start);

    // we reuse the buffer for external strings in the B+trees creation
    char* const buffer = external_helper->buffer;
    const auto buffer_size = external_helper->buffer_size;

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
        boost::unordered_flat_set<uint64_t> nodes_set;

        declared_nodes.begin_tuple_iter();
        while (declared_nodes.has_next_tuple()) {
            auto& tuple = declared_nodes.next_tuple();
            nodes_set.insert(tuple[0]);
        }

        edges.begin_tuple_iter();
        while (edges.has_next_tuple()) {
            auto& tuple = edges.next_tuple();
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
        catalog.nodes_count = nodes_set.size();
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
        size_t C_NODE = 0;
        NoStat<1> no_stat;

        declared_nodes.create_bpt(db_folder + "/nodes", { C_NODE }, no_stat);
    }

    { // Labels B+Tree
        size_t C_NODE = 0, C_LABEL = 1;

        NoStat<2> no_stat;
        DictCountStat<2> label_stat;

        labels.create_bpt(db_folder + "/node_label", { C_NODE, C_LABEL }, no_stat);

        labels.create_bpt(db_folder + "/label_node", { C_LABEL, C_NODE }, label_stat);
        catalog.label_count = label_stat.all;
        label_stat.end();

        catalog.label2total_count = std::move(label_stat.dict);
    }

    { // Properties B+Tree
        size_t C_OBJ = 0, C_KEY = 1, C_VALUE = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        properties.create_bpt(db_folder + "/object_key_value", { C_OBJ, C_KEY, C_VALUE }, no_stat);

        properties.create_bpt(db_folder + "/key_value_object", { C_KEY, C_VALUE, C_OBJ }, prop_stat);
        catalog.properties_count = prop_stat.all;
        prop_stat.end();

        catalog.key2total_count = std::move(prop_stat.map_key_count);
    }

    { // Quad B+Trees
        size_t C_FROM = 0, C_TO = 1, C_TYPE = 2, C_EDGE = 3;

        NoStat<4> no_stat;
        AllStat<4> all_stat;
        DictCountStat<4> dict_count_stat;

        edges.create_bpt(db_folder + "/from_to_type_edge", { C_FROM, C_TO, C_TYPE, C_EDGE }, all_stat);

        edges.create_bpt(db_folder + "/to_type_from_edge", { C_TO, C_TYPE, C_FROM, C_EDGE }, no_stat);

        edges.create_bpt(db_folder + "/type_from_to_edge", { C_TYPE, C_FROM, C_TO, C_EDGE }, dict_count_stat);

        edges.create_bpt(db_folder + "/type_to_from_edge", { C_TYPE, C_TO, C_FROM, C_EDGE }, no_stat);

        edges.create_bpt(db_folder + "/edge_from_to_type", { C_EDGE, C_FROM, C_TO, C_TYPE }, no_stat);

        catalog.edge_count = all_stat.all;
        catalog.type2total_count = std::move(dict_count_stat.dict);
    }

    { // FROM=TO=TYPE EDGE
        size_t C_FROM_TO_TYPE = 0, C_EDGE = 1;

        DictCountStat<2> stat;
        equal_from_to_type.create_bpt(db_folder + "/equal_from_to_type", { C_FROM_TO_TYPE, C_EDGE }, stat);
        catalog.equal_from_to_type_count = stat.all;

        stat.end();
        catalog.type2equal_from_to_type_count = std::move(stat.dict);
    }

    { // FROM=TO TYPE EDGE
        size_t C_FROM_TO = 0, C_TYPE = 1, C_EDGE = 2;

        NoStat<3> no_stat;
        equal_from_to.create_bpt(db_folder + "/equal_from_to", { C_FROM_TO, C_TYPE, C_EDGE }, no_stat);

        DictCountStat<3> stat;
        equal_from_to.create_bpt(db_folder + "/equal_from_to_inverted", { C_TYPE, C_FROM_TO, C_EDGE }, stat);
        stat.end();

        catalog.equal_from_to_count = stat.all;
        catalog.type2equal_from_to_count = std::move(stat.dict);
    }

    { // FROM=TYPE TO EDGE
        size_t C_FROM_TYPE = 0, C_TO = 1, C_EDGE = 2;

        DictCountStat<3> stat;
        equal_from_type.create_bpt(db_folder + "/equal_from_type", { C_FROM_TYPE, C_TO, C_EDGE }, stat);
        stat.end();
        catalog.equal_from_type_count = stat.all;
        catalog.type2equal_from_type_count = std::move(stat.dict);

        NoStat<3> no_stat;
        equal_from_type
            .create_bpt(db_folder + "/equal_from_type_inv", { C_TO, C_FROM_TYPE, C_EDGE }, no_stat);
    }

    { // TO=TYPE FROM EDGE
        size_t C_TO_TYPE = 0, C_FROM = 1, C_EDGE = 2;

        DictCountStat<3> stat;
        equal_to_type.create_bpt(db_folder + "/equal_to_type", { C_TO_TYPE, C_FROM, C_EDGE }, stat);
        stat.end();
        catalog.equal_to_type_count = stat.all;
        catalog.type2equal_to_type_count = std::move(stat.dict);

        NoStat<3> no_stat;
        equal_to_type
            .create_bpt(db_folder + "/equal_to_type_inverted", { C_FROM, C_TO_TYPE, C_EDGE }, no_stat);
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

    print_duration("Write B+tree indexes", start);

    catalog.print(std::cout);

    print_duration("Total Import", import_start);
}

void OnDiskImport::create_automata()
{
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
        Token::K_TRUE,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_true, this)
    );
    set_transition(
        State::LINE_BEGIN,
        Token::K_FALSE,
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
        Token::K_FALSE,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_false, this)
    );
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::K_TRUE,
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
        Token::K_TRUE,
        State::EXPECT_EDGE_TYPE_COLON,
        std::bind(&OnDiskImport::save_second_id_true, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::K_FALSE,
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
        Token::K_FALSE,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_false, this)
    );
    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::K_TRUE,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_true, this)
    );
}
