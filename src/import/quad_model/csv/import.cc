#include "import.h"

#include <cctype>

#include "graph_models/inliner.h"
#include "import/import_helper.h"
#include "misc/fatal_error.h"
#include "misc/unicode_escape.h"

using namespace Import::QuadModel::CSV;

OnDiskImport::OnDiskImport(
    const std::string& db_folder,
    uint64_t strings_buffer_size,
    uint64_t tensors_buffer_size,
    char list_separator
) :
    strings_buffer_size(strings_buffer_size),
    tensors_buffer_size(tensors_buffer_size),
    db_folder(db_folder),
    catalog(QuadCatalog("catalog.dat")),
    declared_nodes(db_folder + "/tmp_declared_nodes"),
    labels(db_folder + "/tmp_labels"),
    properties(db_folder + "/tmp_properties"),
    edges(db_folder + "/tmp_edges"),
    equal_from_to(db_folder + "/tmp_equal_from_to"),
    equal_from_type(db_folder + "/tmp_equal_from_type"),
    equal_to_type(db_folder + "/tmp_equal_to_type"),
    equal_from_to_type(db_folder + "tmp_equal_from_to_type")
{
    state_transitions = new int[Token::TOTAL_TOKENS * State::TOTAL_STATES];
    create_automata();
    label_splitter = list_separator;
}

OnDiskImport::~OnDiskImport()
{
    delete[] state_transitions;
}

void OnDiskImport::start_import(
    std::vector<std::unique_ptr<MDBIstreamFile>>& in_nodes,
    std::vector<std::unique_ptr<MDBIstreamFile>>& in_edges
)
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

    // We save the headers first, so that the keys are never temporal
    save_headers(in_nodes);
    save_headers(in_edges);

    // First, import nodes to the database. After that, import relationships
    parse_node_files(in_nodes);
    parse_edge_files(in_edges);

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

                auto id1 = external_helper->resolve_id(pending_tuple[0]);

                try_save_declared_node(id1);
            }

            old_pending_labels->begin_tuple_iter();
            while (old_pending_labels->has_next_tuple()) {
                const auto& pending_tuple = old_pending_labels->next_tuple();

                auto id1 = external_helper->resolve_id(pending_tuple[0]);
                auto label_id = external_helper->resolve_id(pending_tuple[1]);

                try_save_label(id1, label_id);
            }

            old_pending_properties->begin_tuple_iter();
            while (old_pending_properties->has_next_tuple()) {
                const auto& pending_tuple = old_pending_properties->next_tuple();

                auto id1 = external_helper->resolve_id(pending_tuple[0]);
                auto key_id = pending_tuple[1];
                auto value_id = external_helper->resolve_id(pending_tuple[2]);

                try_save_property(id1, key_id, value_id);
            }

            old_pending_edges->begin_tuple_iter();
            while (old_pending_edges->has_next_tuple()) {
                const auto& pending_tuple = old_pending_edges->next_tuple();

                auto id1 = external_helper->resolve_id(pending_tuple[0]);
                auto id2 = external_helper->resolve_id(pending_tuple[1]);
                auto type_id = external_helper->resolve_id(pending_tuple[2]);
                auto edge_id = pending_tuple[3]; // always inlined

                try_save_quad(id1, id2, type_id, edge_id);
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

    declared_nodes.finish_appends();
    labels.finish_appends();
    properties.finish_appends();
    edges.finish_appends();
    equal_from_to.finish_appends();
    equal_from_type.finish_appends();
    equal_to_type.finish_appends();
    equal_from_to_type.finish_appends();

    { // Append undeclared nodes (being on an edge)
        std::unordered_set<uint64_t> nodes_set;

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
            .create_bpt(db_folder + "/equal_from_type_inverted", { C_TO, C_FROM_TYPE, C_EDGE }, no_stat);
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

void OnDiskImport::print_error()
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << std::endl;
}

void OnDiskImport::print_error_msg(const std::string& msg)
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << ": " << msg << std::endl;
}

void OnDiskImport::print_fatal_error_msg(const std::string& msg)
{
    std::string error = "FATAL ERROR on line ";
    error += std::to_string(current_line);
    error += ": " + msg;
    FATAL_ERROR(error);
}

std::vector<std::string> OnDiskImport::split(const std::string& input, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    // Add the last substring
    tokens.push_back(input.substr(start));

    return tokens;
}

void OnDiskImport::save_headers(std::vector<std::unique_ptr<MDBIstreamFile>>& files)
{
    for (auto& in_file : files) {
        lexer.begin(*in_file);

        while (auto token = lexer.get_token()) {
            if (token == Token::ENDLINE || token == Token::END_OF_FILE) {
                break;
            }
            if (token != Token::STRING && token != Token::UNQUOTED_STRING) {
                continue;
            }

            std::vector<std::string> split_col = split(lexer.str, ":");

            for (auto str : split_col) {
                if (str.size() >= 8) {
                    external_helper->get_or_create_external_string_id(str.c_str(), str.size());
                }
            }
        }
        in_file->rewind();
    }
}

void OnDiskImport::parse_node_files(std::vector<std::unique_ptr<MDBIstreamFile>>& in_nodes)
{
    std::cout << "Importing nodes\n";
    for (auto& in_file : in_nodes) {
        current_state = State::START_HEADER_NODES;
        lexer.begin(*in_file);
        std::cout << "Reading file " << in_file->filename << std::endl;

        while (auto token = lexer.get_token()) {
            current_token = token;
            current_state = get_transition(current_state, token);
        }

        if (current_token != Token::ENDLINE && current_token != Token::END_OF_FILE) {
            process_node_line();
        }

        reset_automata();
    }
}

void OnDiskImport::parse_edge_files(std::vector<std::unique_ptr<MDBIstreamFile>>& in_edges)
{
    std::cout << "Importing edges\n";
    for (auto& in_file : in_edges) {
        current_state = State::START_HEADER_EDGES;
        lexer.begin(*in_file);
        std::cout << "Reading file " << in_file->filename << std::endl;

        while (auto token = lexer.get_token()) {
            current_token = token;
            current_state = get_transition(current_state, token);
        }

        if (current_token != Token::ENDLINE && current_token != Token::END_OF_FILE) {
            save_edge_line();
        }

        reset_automata();
    }
}

void OnDiskImport::reset_automata()
{
    columns.clear();
    current_column = 0;
    current_line = 1;
    anonymous_nodes = true;
    global_ids = true;
    current_group.clear();
    current_group_from.clear();
    current_group_to.clear();
    current_group_idx_from = 0;
    current_group_idx_to = 0;
    column_with_type = 0;
    column_with_id = 0;
}

void OnDiskImport::set_transition(int state, int token, int value, std::function<void()> func)
{
    state_funcs[State::TOTAL_STATES * token + state] = func;
    state_transitions[State::TOTAL_STATES * token + state] = value;
}

int OnDiskImport::get_transition(int state, int token)
{
    state_funcs[State::TOTAL_STATES * token + state]();
    return state_transitions[State::TOTAL_STATES * token + state];
}

void OnDiskImport::save_header_column()
{
    if (current_state == State::START_BODY_NODES)
        current_line++;

    CSVType new_column_type;
    std::vector<std::string> split_new_col = split(lexer.str, ":");
    if (split_new_col.size() == 1)
        new_column_type = CSVType::UNDEFINED;
    else {
        if (split_new_col[1] == "ID") {
            new_column_type = CSVType::ID;
            if (split_new_col.size() == 3) {
                global_ids = false;
                current_group = split_new_col[2];
                if (!csvid_groups_index.contains(split_new_col[2])) {
                    csvid_groups.push_back(boost::unordered_flat_map<std::string, uint64_t>());
                    csvid_groups_index.insert({ split_new_col[2], group_count });
                    current_group_idx = group_count;
                    group_count++;
                } else {
                    current_group_idx = csvid_groups_index[split_new_col[2]];
                }
            }
        } else if (split_new_col[1] == "START_ID") {
            new_column_type = CSVType::START_ID;
            if (split_new_col.size() == 3) {
                if (csvid_groups_index.contains(split_new_col[2]))
                    current_group_idx_from = csvid_groups_index[split_new_col[2]];
                else
                    print_fatal_error_msg("Group \"" + split_new_col[2] + "\" does not exist");
            } else {
                current_group_idx_from = -1;
            }
        } else if (split_new_col[1] == "END_ID") {
            new_column_type = CSVType::END_ID;
            if (split_new_col.size() == 3) {
                if (csvid_groups_index.contains(split_new_col[2]))
                    current_group_idx_to = csvid_groups_index[split_new_col[2]];
                else
                    print_fatal_error_msg("Group \"" + split_new_col[2] + "\" does not exist");
            } else {
                current_group_idx_to = -1;
            }
        }

        else if (split_new_col[1] == "TYPE")
            new_column_type = CSVType::TYPE;

        else if (split_new_col[1] == "STR")
            new_column_type = CSVType::STR;
        else if (split_new_col[1] == "INT")
            new_column_type = CSVType::INT;
        else if (split_new_col[1] == "FLOAT")
            new_column_type = CSVType::DECIMAL;
        else if (split_new_col[1] == "LABEL")
            new_column_type = CSVType::LABEL;
        else if (split_new_col[1] == "DATE")
            new_column_type = CSVType::DATE;
        else if (split_new_col[1] == "DATETIME")
            new_column_type = CSVType::DATETIME;

        else
            new_column_type = CSVType::UNDEFINED;
    }
    uint64_t new_column_key_id;
    std::string& col = split_new_col[0];
    if (col.size() < 8)
        new_column_key_id = Inliner::inline_string(col.c_str()) | ObjectId::MASK_STRING_SIMPLE_INLINED;
    else {
        new_column_key_id = external_helper->get_or_create_external_string_id(col.data(), col.size())
                          | ObjectId::MASK_STRING_SIMPLE;
    }

    columns.emplace_back(new_column_type, col, new_column_key_id);
}

void OnDiskImport::verify_anon()
{
    for (int col_idx = 0; col_idx < (int) columns.size(); col_idx++) {
        if (columns[col_idx].type == CSVType::ID) {
            anonymous_nodes = false;
            column_with_id = col_idx;
            break;
        }
    }
    current_line++;
}

void OnDiskImport::verify_edge_file_header()
{
    // For an edges file to be correct, in the columns we should find
    // one START_ID, one END_ID and one TYPE column. Having more or less than that
    // is a bad file and the import should stop (or at least the file should be skipped)

    bool has_start_id = false, has_end_id = false, has_type = false;
    for (int col_idx = 0; col_idx < (int) columns.size(); col_idx++) {
        if (columns[col_idx].type == CSVType::START_ID && !has_start_id) {
            has_start_id = true;
            column_with_id_from = col_idx;
        } else if (columns[col_idx].type == CSVType::START_ID && has_start_id)
            print_fatal_error_msg("More than one START_ID column is present");
        else if (columns[col_idx].type == CSVType::END_ID && !has_end_id) {
            has_end_id = true;
            column_with_id_to = col_idx;
        } else if (columns[col_idx].type == CSVType::END_ID && has_end_id)
            print_fatal_error_msg("More than one END_ID column is present");
        else if (columns[col_idx].type == CSVType::TYPE && !has_type) {
            has_type = true;
            column_with_type = col_idx;
        } else if (columns[col_idx].type == CSVType::TYPE && has_type)
            print_fatal_error_msg("More than one TYPE column is present");
    }

    if (!has_start_id || !has_end_id || !has_type) {
        std::string error = "The following column(s) are missing from the header:";
        if (!has_start_id)
            error += " START_ID";
        if (!has_end_id)
            error += " END_ID";
        if (!has_type)
            error += " TYPE";
        error += '.';
        print_fatal_error_msg(error);
    }
    current_line++;
}

void OnDiskImport::save_body_column_to_buffer()
{
    std::strcpy(columns[current_column].value_str, lexer.str);
    columns[current_column].value_size = lexer.str_len;

    if (columns[current_column].type == CSVType::UNDEFINED) {
        switch (current_token) {
        case Token::STRING:
            columns[current_column].type = CSVType::STR;
            break;
        case Token::UNQUOTED_STRING:
            columns[current_column].type = CSVType::STR;
            break;
        case Token::INTEGER:
            columns[current_column].type = CSVType::INT;
            break;
        case Token::FLOAT:
            columns[current_column].type = CSVType::DECIMAL;
            break;

        default:
            print_error_msg("Cannot detect type. Please specify in CSV header");
            break;
        }
    }
}

void OnDiskImport::save_empty_body_column_to_buffer()
{
    columns[current_column].value_size = 0;
    current_column++;
}

void OnDiskImport::process_node_line()
{
    CSVColumn& id_col = columns[column_with_id];
    uint64_t node_id;
    if (anonymous_nodes) {
        node_id = current_anon_id++ | ObjectId::MASK_ANON_INLINED;
    } else if (id_col.value_size == 0) {
        print_error_msg("No ID was given for the node. It will be saved as an anonymous node");
        node_id = current_anon_id++ | ObjectId::MASK_ANON_INLINED;
    } else if (global_ids) {
        // If a node with the same ID exists, do not save.
        if (csvid_global.contains(id_col.value_str)) {
            std::string error_msg = "Duplicated ID \"";
            error_msg += id_col.value_str;
            error_msg += "\". Node will not be saved.";
            print_error_msg(error_msg);

            go_to_next_line();

            return;
        }
        // Using global IDs
        bool is_number = true;
        for (int char_idx = 0; char_idx < (int) id_col.value_size; char_idx++) {
            if (!std::isdigit(id_col.value_str[char_idx])) {
                is_number = false;
                break;
            }
        }

        if (is_number) {
            node_id = try_parse_int(id_col.value_str);
        } else {
            normalize_string_literal(id_col);
            if (id_col.value_size < 8)
                node_id = Inliner::inline_string(id_col.value_str) | ObjectId::MASK_NAMED_NODE_INLINED;
            else
                node_id = external_helper->get_or_create_external_string_id(
                              id_col.value_str,
                              id_col.value_size
                          )
                        | ObjectId::MASK_NAMED_NODE;
        }
        csvid_global.insert({ id_col.value_str, node_id });
    } else {
        // Using IDs inside the scope of a group
        node_id = current_anon_id++ | ObjectId::MASK_ANON_INLINED;
        if (csvid_groups[current_group_idx].contains(id_col.value_str)) {
            std::string error_msg = "Duplicated ID \"";
            error_msg += id_col.value_str;
            error_msg += "\" inside group \"";
            error_msg += current_group;
            error_msg += "\". Node will not be saved.";

            go_to_next_line();

            print_error_msg(error_msg);
            return;
        }
        csvid_groups[current_group_idx].insert({ id_col.value_str, node_id });
    }
    try_save_declared_node(node_id);

    for (auto& col : columns) {
        if (col.value_size == 0)
            continue;
        switch (col.type) {
        case CSVType::ID: {
            // The ID column was already worked on. Nothing should be done.
            break;
        }
        case CSVType::LABEL: {
            std::vector<std::string> labels_vector = split(col.value_str, label_splitter);
            for (auto label : labels_vector) {
                uint64_t label_id;
                if (col.value_size < 8) {
                    label_id = Inliner::inline_string(label.c_str()) | ObjectId::MASK_STRING_SIMPLE_INLINED;
                } else {
                    label_id = external_helper->get_or_create_external_string_id(label.data(), label.size())
                             | ObjectId::MASK_STRING_SIMPLE;
                }

                try_save_label(node_id, label_id);
            }
            break;
        }
        case CSVType::STR: {
            uint64_t value_id;
            normalize_string_literal(col);

            if (col.value_size < 8)
                value_id = Inliner::inline_string(col.value_str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
            else
                value_id = external_helper->get_or_create_external_string_id(col.value_str, col.value_size)
                         | ObjectId::MASK_STRING_SIMPLE;

            try_save_property(node_id, col.key_id, value_id);
            break;
        }
        case CSVType::INT: {
            uint64_t value_id = try_parse_int(col.value_str);
            try_save_property(node_id, col.key_id, value_id);
            break;
        }
        case CSVType::DECIMAL: {
            uint64_t value_id = try_parse_float(col.value_str);
            try_save_property(node_id, col.key_id, value_id);
            break;
        }
        case CSVType::DATE: {
            uint64_t value_id = DateTime::from_date(col.value_str);
            if (value_id == ObjectId::NULL_ID) {
                std::string error = "Invalid date: ";
                error += col.value_str;
                print_error_msg(error);
                break;
            }
            try_save_property(node_id, col.key_id, value_id);
            break;
        }
        case CSVType::DATETIME: {
            uint64_t value_id = DateTime::from_dateTime(col.value_str);
            if (value_id == ObjectId::NULL_ID) {
                std::string error = "Invalid dateTime: ";
                error += col.value_str;
                print_error_msg(error);
                break;
            }
            try_save_property(node_id, col.key_id, value_id);
            break;
        }

        default:
            print_error_msg("Unhandled type");
            break;
        }
    }

    go_to_next_line();
}

void OnDiskImport::save_edge_line()
{
    if (columns[column_with_type].value_size == 0) {
        print_error_msg("The edge does not have a type and it is required. The edge will not be saved");
        go_to_next_line();
        return;
    }
    uint64_t type_id;
    if (columns[column_with_type].value_size < 8)
        type_id = Inliner::inline_string(columns[column_with_type].value_str)
                | ObjectId::MASK_NAMED_NODE_INLINED;
    else
        type_id = external_helper->get_or_create_external_string_id(
                      columns[column_with_type].value_str,
                      columns[column_with_type].value_size
                  )
                | ObjectId::MASK_NAMED_NODE;

    if (columns[column_with_id_from].value_size == 0 || columns[column_with_id_to].value_size == 0) {
        print_error_msg("The edge has missing IDs and they are required. The edge will not be saved");
        go_to_next_line();
        return;
    }

    uint64_t from_id, to_id;
    if (current_group_idx_from == -1) {
        // Using global scope of ids.
        if (!csvid_global.contains(columns[column_with_id_from].value_str)) {
            std::string error = "The ID ";
            error += columns[column_with_id_from].value_str;
            error += " does not exist in the global scope. Edge will not be saved.";
            print_error_msg(error);

            go_to_next_line();

            return;
        }
        from_id = csvid_global[columns[column_with_id_from].value_str];
    } else {
        // Using a group of ids.
        if (!csvid_groups[current_group_idx_from].contains(columns[column_with_id_from].value_str)) {
            std::string error = "The ID ";
            error += columns[column_with_id_from].value_str;
            error += " does not exist in the group ";
            error += current_group_from;
            error += ". Edge will not be saved.";
            print_error_msg(error);

            go_to_next_line();

            return;
        }
        from_id = csvid_groups[current_group_idx_from][columns[column_with_id_from].value_str];
    }

    if (current_group_idx_to == -1) {
        // Using global scope of ids.
        if (!csvid_global.contains(columns[column_with_id_to].value_str)) {
            std::string error = "The ID ";
            error += columns[column_with_id_to].value_str;
            error += " does not exist in the global scope. Edge will not be saved.";
            print_error_msg(error);

            go_to_next_line();

            return;
        }
        to_id = csvid_global[columns[column_with_id_to].value_str];
    } else {
        // Using a group of ids.
        if (!csvid_groups[current_group_idx_to].contains(columns[column_with_id_to].value_str)) {
            std::string error = "The ID ";
            error += columns[column_with_id_to].value_str;
            error += " does not exist in the group ";
            error += current_group_to;
            error += ". Edge will not be saved.";
            print_error_msg(error);

            go_to_next_line();

            return;
        }
        to_id = csvid_groups[current_group_idx_to][columns[column_with_id_to].value_str];
    }

    uint64_t edge_id = edge_count++ | ObjectId::MASK_EDGE;

    try_save_quad(from_id, to_id, type_id, edge_id);

    for (auto& col : columns) {
        if (col.value_size == 0)
            continue;
        switch (col.type) {
        // START_ID, END_ID and TYPE should do nothing
        case CSVType::START_ID:
        case CSVType::END_ID:
        case CSVType::TYPE:
            break;
        case CSVType::LABEL: {
            print_error_msg("Labels are not supported in QuadModel. Skipping");
            break;
        }
        case CSVType::STR: {
            uint64_t value_id;
            normalize_string_literal(col);

            if (col.value_size < 8)
                value_id = Inliner::inline_string(col.value_str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
            else
                value_id = external_helper->get_or_create_external_string_id(col.value_str, col.value_size)
                         | ObjectId::MASK_STRING_SIMPLE;

            try_save_property(edge_id, col.key_id, value_id);
            break;
        }
        case CSVType::INT: {
            uint64_t value_id = try_parse_int(col.value_str);
            try_save_property(edge_id, col.key_id, value_id);
            break;
        }
        case CSVType::DECIMAL: {
            uint64_t value_id = try_parse_float(col.value_str);
            try_save_property(edge_id, col.key_id, value_id);
            break;
        }
        case CSVType::DATE: {
            uint64_t value_id = DateTime::from_date(col.value_str);
            if (value_id == ObjectId::NULL_ID) {
                std::string error = "Invalid date: ";
                error += col.value_str;
                print_error_msg(error);
                break;
            }
            try_save_property(edge_id, col.key_id, value_id);
            break;
        }
        case CSVType::DATETIME: {
            uint64_t value_id = DateTime::from_dateTime(col.value_str);
            if (value_id == ObjectId::NULL_ID) {
                std::string error = "Invalid dateTime: ";
                error += col.value_str;
                print_error_msg(error);
                break;
            }
            try_save_property(edge_id, col.key_id, value_id);
            break;
        }

        default:
            print_error_msg("Unhandled type");
            break;
        }
    }

    go_to_next_line();
}

void OnDiskImport::go_to_next_line()
{
    current_column = 0;
    current_line++;
}

void OnDiskImport::normalize_string_literal(CSVColumn& col)
{
    char* write_ptr = col.value_str;
    char* read_ptr;
    char* end;
    if (write_ptr[0] == '"') {
        // Remove quotation marks
        read_ptr = write_ptr + 1;
        col.value_size -= 2;
        end = col.value_str + col.value_size + 1;
    } else {
        // Leave the string as it is
        read_ptr = write_ptr;
        end = col.value_str + col.value_size;
    }

    UnicodeEscape::normalize_string(read_ptr, write_ptr, end, col.value_size);
}

void OnDiskImport::try_save_declared_node(uint64_t node_id)
{
    if ((node_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_declared_nodes->push_back({ node_id });
    } else {
        declared_nodes.push_back({ node_id });
    }
}

void OnDiskImport::try_save_label(uint64_t node_id, uint64_t label_id)
{
    if ((node_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (label_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
    {
        pending_labels->push_back({ node_id, label_id });
    } else {
        labels.push_back({ node_id, label_id });
    }
}

void OnDiskImport::try_save_property(uint64_t id1, uint64_t key_id, uint64_t value_id)
{
    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
    {
        pending_properties->push_back({ id1, key_id, value_id });
    } else {
        properties.push_back({ id1, key_id, value_id });
    }
}

void OnDiskImport::try_save_quad(uint64_t from_id, uint64_t to_id, uint64_t type_id, uint64_t edge_id)
{
    if ((from_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (to_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (type_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
    {
        pending_edges->push_back({ from_id, to_id, type_id, edge_id });
        return;
    }

    edges.push_back({ from_id, to_id, type_id, edge_id });

    if (from_id == to_id) {
        equal_from_to.push_back({ from_id, type_id, edge_id });

        if (from_id == type_id) {
            equal_from_to_type.push_back({ from_id, edge_id });
        }
    }
    if (from_id == type_id) {
        equal_from_type.push_back({ from_id, to_id, edge_id });
    }
    if (to_id == type_id) {
        equal_to_type.push_back({ from_id, to_id, edge_id });
    }
}
