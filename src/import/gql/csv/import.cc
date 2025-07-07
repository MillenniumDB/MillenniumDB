#include "import.h"

#include <iostream>

#include "graph_models/inliner.h"
#include "import/import_helper.h"
#include "import/external_helper.h"
#include "misc/fatal_error.h"
#include "misc/unicode_escape.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"

using namespace Import::GQL::CSV;

OnDiskImport::OnDiskImport(
    const std::string& db_folder,
    uint64_t strings_buffer_size,
    uint64_t tensors_buffer_size,
    char list_separator
) :
    strings_buffer_size(strings_buffer_size),
    tensors_buffer_size(tensors_buffer_size),
    db_folder(db_folder),
    catalog(GQLCatalog("catalog.dat")),
    node_labels(db_folder + "/node_labels"),
    edge_labels(db_folder + "/edge_labels"),
    node_properties(db_folder + "/node_properties"),
    edge_properties(db_folder + "/edge_properties"),
    directed_edges(db_folder + "/directed_edges"),
    undirected_edges(db_folder + "/undirected_edges"),
    directed_equal_edges(db_folder + "/directed_equal_edges"),
    undirected_equal_edges(db_folder + "/undirected_equal_edges")
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

    pending_node_properties = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_NODE_PROPERTIES_FILENAME_PREFIX
    );
    pending_edge_properties = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_EDGE_PROPERTIES_FILENAME_PREFIX
    );

    // Initialize external helper
    external_helper = std::make_unique<ExternalHelper>(db_folder, strings_buffer_size, tensors_buffer_size);

    parse_node_files(in_nodes);
    parse_edge_files(in_edges);

    external_helper->flush_to_disk();

    { // process pending files
        pending_node_properties->finish_appends();
        pending_edge_properties->finish_appends();

        int i = 0;
        while (true) {
            const auto total_pending = pending_node_properties->get_total_tuples()
                                     + pending_edge_properties->get_total_tuples();

            if (total_pending == 0) {
                break;
            }
            std::cout << "total pending: " << total_pending << std::endl;

            auto old_pending_node_properties = std::move(pending_node_properties);
            auto old_pending_edge_properties = std::move(pending_edge_properties);

            pending_node_properties = std::make_unique<DiskVector<3>>(
                db_folder + "/" + PENDING_NODE_PROPERTIES_FILENAME_PREFIX + std::to_string(i)
            );
            pending_edge_properties = std::make_unique<DiskVector<3>>(
                db_folder + "/" + PENDING_EDGE_PROPERTIES_FILENAME_PREFIX + std::to_string(i)
            );
            ++i;

            // advance pending variables for current iteration
            external_helper->advance_pending();
            external_helper->clear_sets();

            old_pending_node_properties->begin_tuple_iter();
            while (old_pending_node_properties->has_next_tuple()) {
                const auto& pending_tuple = old_pending_node_properties->next_tuple();

                uint64_t node_id = pending_tuple[0];
                uint64_t key_id = pending_tuple[1];
                uint64_t value_id = external_helper->resolve_id(pending_tuple[2]);

                try_save_node_property(node_id, key_id, value_id);
            }

            old_pending_edge_properties->begin_tuple_iter();
            while (old_pending_edge_properties->has_next_tuple()) {
                const auto& pending_tuple = old_pending_edge_properties->next_tuple();

                uint64_t edge_id = pending_tuple[0];
                uint64_t key_id = pending_tuple[1];
                uint64_t value_id = external_helper->resolve_id(pending_tuple[2]);

                try_save_edge_property(edge_id, key_id, value_id);
            }

            external_helper->flush_to_disk();
            external_helper->clean_up_old();

            pending_node_properties->finish_appends();
            pending_edge_properties->finish_appends();
            old_pending_node_properties->skip_indexing();
            old_pending_edge_properties->skip_indexing();
        }

        // process pending finished, clean up the last pending file
        pending_node_properties->skip_indexing();
        pending_edge_properties->skip_indexing();
    }

    external_helper->clean_up();

    external_helper->build_disk_hash();

    char* const buffer = external_helper->buffer;
    const auto buffer_size = external_helper->buffer_size;

    node_labels.finish_appends();
    edge_labels.finish_appends();
    node_properties.finish_appends();
    edge_properties.finish_appends();
    directed_edges.finish_appends();
    undirected_edges.finish_appends();
    directed_equal_edges.finish_appends();
    undirected_equal_edges.finish_appends();

    // Directed Edges Table
    {
        EdgeTableMemImport<3> table_writer(db_folder + "/d_edges.table");

        directed_edges.begin_tuple_iter();
        while (directed_edges.has_next_tuple()) {
            auto& tuple = directed_edges.next_tuple();
            table_writer.insert_tuple(tuple);
        }
    }

    // Undirected Edges Table
    {
        EdgeTableMemImport<3> table_writer(db_folder + "/u_edges.table");

        undirected_edges.begin_tuple_iter();
        while (undirected_edges.has_next_tuple()) {
            auto& tuple = undirected_edges.next_tuple();
            table_writer.insert_tuple(tuple);
        }
    }

    node_labels.start_indexing(buffer, buffer_size, { 0, 1 });
    edge_labels.start_indexing(buffer, buffer_size, { 0, 1 });
    node_properties.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    edge_properties.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    directed_edges.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    undirected_edges.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    directed_equal_edges.start_indexing(buffer, buffer_size, { 0, 1 });
    undirected_equal_edges.start_indexing(buffer, buffer_size, { 0, 1 });

    // Nodes Labels B+Tree
    {
        size_t COL_NODE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        DictCountStat<2> label_stat;

        node_labels.create_bpt(db_folder + "/node_label", { COL_NODE, COL_LABEL }, no_stat);
        node_labels.create_bpt(db_folder + "/label_node", { COL_LABEL, COL_NODE }, label_stat);

        catalog.node_labels_count = label_stat.all;
        label_stat.end();

        catalog.node_label2total_count = std::move(label_stat.dict);
    }

    // Edges Labels B+Tree
    {
        size_t COL_EDGE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        DictCountStat<2> label_stat;

        edge_labels.create_bpt(db_folder + "/edge_label", { COL_EDGE, COL_LABEL }, no_stat);
        edge_labels.create_bpt(db_folder + "/label_edge", { COL_LABEL, COL_EDGE }, label_stat);

        catalog.edge_labels_count = label_stat.all;
        label_stat.end();

        catalog.edge_label2total_count = std::move(label_stat.dict);
    }

    // Node Properties B+Tree
    {
        size_t COL_NODE = 0, COL_KEY = 1, COL_VAL = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        node_properties.create_bpt(db_folder + "/node_key_value", { COL_NODE, COL_KEY, COL_VAL }, no_stat);
        node_properties.create_bpt(db_folder + "/key_value_node", { COL_KEY, COL_VAL, COL_NODE }, prop_stat);

        catalog.node_properties_count = prop_stat.all;
        prop_stat.end();

        catalog.node_key2total_count = std::move(prop_stat.map_key_count);
    }

    // Edge Properties B+Tree
    {
        size_t COL_EDGE = 0, COL_KEY = 1, COL_VAL = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        edge_properties.create_bpt(db_folder + "/edge_key_value", { COL_EDGE, COL_KEY, COL_VAL }, no_stat);
        edge_properties.create_bpt(db_folder + "/key_value_edge", { COL_KEY, COL_VAL, COL_EDGE }, prop_stat);

        catalog.edge_properties_count = prop_stat.all;
        prop_stat.end();

        catalog.edge_key2total_count = std::move(prop_stat.map_key_count);
    }

    // Undirected edges
    {
        size_t COL_N1 = 0, COL_N2 = 1, COL_EDGE = 2;
        NoStat<3> stat;

        undirected_edges.create_bpt(db_folder + "/u_edge", { COL_N1, COL_N2, COL_EDGE }, stat);
    }

    // Directed edges
    {
        size_t COL_FROM = 0, COL_TO = 1, COL_EDGE = 2;
        NoStat<3> no_stat;

        directed_edges.create_bpt(db_folder + "/from_to_edge", { COL_FROM, COL_TO, COL_EDGE }, no_stat);
        directed_edges.create_bpt(db_folder + "/to_from_edge", { COL_TO, COL_FROM, COL_EDGE }, no_stat);
    }

    // Undirected equal edges
    {
        AllStat<2> all_stat;
        undirected_equal_edges.create_bpt(db_folder + "/equal_u_edge", { 0, 1 }, all_stat);
        catalog.equal_undirected_edges_count = all_stat.all;
    }

    // Directed equal edges
    {
        AllStat<2> all_stat;
        directed_equal_edges.create_bpt(db_folder + "/equal_d_edge", { 0, 1 }, all_stat);
        catalog.equal_directed_edges_count = all_stat.all;
    }

    node_labels.finish_indexing();
    edge_labels.finish_indexing();
    node_properties.finish_indexing();
    edge_properties.finish_indexing();
    directed_edges.finish_indexing();
    undirected_edges.finish_indexing();
    directed_equal_edges.finish_indexing();
    undirected_equal_edges.finish_indexing();

    catalog.print(std::cout);
    print_duration("Total import time", start);
}

void OnDiskImport::try_save_node_property(uint64_t node_id, uint64_t key_id, uint64_t value_id)
{
    if ((value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_properties->push_back({ node_id, key_id, value_id });
    } else {
        node_properties.push_back({ node_id, key_id, value_id });
    }
}

void OnDiskImport::try_save_edge_property(uint64_t edge_id, uint64_t key_id, uint64_t value_id)
{
    if ((value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_edge_properties->push_back({ edge_id, key_id, value_id });
    } else {
        edge_properties.push_back({ edge_id, key_id, value_id });
    }
}

void OnDiskImport::print_error()
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << std::endl;
}

void OnDiskImport::print_error_msg(std::string msg)
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << ": " << msg << std::endl;
}

void OnDiskImport::print_fatal_error_msg(std::string msg)
{
    std::string error = "FATAL ERROR on line ";
    error += std::to_string(current_line);
    error += ": " + msg;
    FATAL_ERROR(error);
}

std::vector<std::string> OnDiskImport::split(std::string input, std::string delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + 1);
    }

    tokens.push_back(input);

    return tokens;
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
        } else if (split_new_col[1] == "UNDIRECTED_ID") {
            new_column_type = CSVType::ID;
            if (!has_set_first_undirected) {
                if (split_new_col.size() == 3) {
                    if (csvid_groups_index.contains(split_new_col[2]))
                        current_group_idx_from = csvid_groups_index[split_new_col[2]];
                    else
                        print_fatal_error_msg("Group \"" + split_new_col[2] + "\" does not exist");
                } else {
                    current_group_idx_from = -1;
                }
                has_set_first_undirected = true;
            } else {
                if (split_new_col.size() == 3) {
                    if (csvid_groups_index.contains(split_new_col[2]))
                        current_group_idx_to = csvid_groups_index[split_new_col[2]];
                    else
                        print_fatal_error_msg("Group \"" + split_new_col[2] + "\" does not exist");
                } else {
                    current_group_idx_to = -1;
                }
            }
        }
        // GQL does not have TYPE. Instead, it uses LABELs
        else if (split_new_col[1] == "TYPE")
            new_column_type = CSVType::LABEL;

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

    columns.push_back(CSVColumn(new_column_type, split_new_col[0]));
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
    // either a START_ID and END_ID, or two ID.
    bool has_start_id = false, has_end_id = false;
    bool has_first_undirected_id = false, has_second_undirected_id = false;
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

        else if (columns[col_idx].type == CSVType::ID && !has_first_undirected_id) {
            has_first_undirected_id = true;
            column_with_id_from = col_idx;
        } else if (columns[col_idx].type == CSVType::ID && has_first_undirected_id
                   && !has_second_undirected_id)
        {
            has_second_undirected_id = true;
            column_with_id_to = col_idx;
        } else if (columns[col_idx].type == CSVType::ID && has_first_undirected_id
                   && !has_second_undirected_id)
            print_fatal_error_msg("Too many undirected IDs are present");
    }

    bool has_directed_ids = has_start_id && has_end_id;
    bool has_undirected_ids = has_first_undirected_id && has_second_undirected_id;
    if (has_directed_ids && has_undirected_ids) {
        print_fatal_error_msg("Both directed and undirected IDs have been specified. Please choose only one");
    }
    if (!has_directed_ids && !has_undirected_ids) {
        print_fatal_error_msg("No IDs have been specified for the relations, or some ID columns are missing");
    }

    has_direction = has_directed_ids;
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

uint64_t OnDiskImport::get_node_key_id(const std::string& column_name)
{
    auto it = node_keys_map.find(column_name);
    if (it != node_keys_map.end()) {
        return it->second;
    } else {
        auto res = current_node_key++ | ObjectId::MASK_NODE_KEY;
        catalog.node_keys_str.push_back(column_name);
        node_keys_map.insert({ column_name, res });
        return res;
    }
}

void OnDiskImport::process_node_line()
{
    uint64_t node_id;
    if (anonymous_nodes) {
        node_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
    } else {
        if (columns[column_with_id].value_size == 0) {
            print_error_msg(
                "No ID was given for the node despite declaring IDs in the header. The node will be saved"
            );
            node_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
        } else {
            if (global_ids) {
                // If a node with the same ID exists, do not save.
                if (csvid_global.contains(columns[column_with_id].value_str)) {
                    std::string error_msg = "Duplicated ID \"";
                    error_msg += columns[column_with_id].value_str;
                    error_msg += "\". Node will not be saved.";
                    print_error_msg(error_msg);

                    go_to_next_line();

                    return;
                }
                // Using global IDs
                node_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                csvid_global.insert({ columns[column_with_id].value_str, node_id });
            } else {
                // Using IDs inside the scope of a group
                if (csvid_groups[current_group_idx].contains(columns[column_with_id].value_str)) {
                    std::string error_msg = "Duplicated ID \"";
                    error_msg += columns[column_with_id].value_str;
                    error_msg += "\" inside group \"";
                    error_msg += current_group;
                    error_msg += "\". Node will not be saved.";

                    go_to_next_line();

                    print_error_msg(error_msg);
                    return;
                }
                node_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                csvid_groups[current_group_idx].insert({ columns[column_with_id].value_str, node_id });
            }
        }
    }

    for (auto& col : columns) {
        if (col.value_size == 0)
            continue;
        switch (col.type) {
        case CSVType::ID: {
            // The ID column was already worked on. Nothing should be done
            break;
        }
        case CSVType::LABEL: {
            std::vector<std::string> labels = split(col.value_str, label_splitter);
            for (auto label : labels) {
                if (label.size() == 0)
                    continue;

                uint64_t label_id;

                auto it = node_labels_map.find(label);
                if (it != node_labels_map.end()) {
                    label_id = it->second;
                } else {
                    label_id = current_node_label++ | ObjectId::MASK_NODE_LABEL;
                    catalog.node_labels_str.push_back(label);
                    node_labels_map.insert({ label, label_id });
                }

                node_labels.push_back({ node_id, label_id });
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

            uint64_t key_id = get_node_key_id(col.name);
            if ((value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                pending_node_properties->push_back({ node_id, key_id, value_id });
            } else {
                node_properties.push_back({ node_id, key_id, value_id });
            }
            break;
        }
        case CSVType::INT: {
            uint64_t value_id = try_parse_int(col.value_str);
            uint64_t key_id = get_node_key_id(col.name);
            node_properties.push_back({ node_id, key_id, value_id });
            break;
        }
        case CSVType::DECIMAL: {
            uint64_t value_id = try_parse_float(col.value_str);
            uint64_t key_id = get_node_key_id(col.name);
            node_properties.push_back({ node_id, key_id, value_id });
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
            uint64_t key_id = get_node_key_id(col.name);
            node_properties.push_back({ node_id, key_id, value_id });
            break;
        }
        case CSVType::DATETIME: {
            uint64_t value_id = DateTime::from_dateTime(col.value_str);
            if (value_id == ObjectId::NULL_ID) {
                std::string error = "Invalid date: ";
                error += col.value_str;
                print_error_msg(error);
                break;
            }
            uint64_t key_id = get_node_key_id(col.name);
            node_properties.push_back({ node_id, key_id, value_id });
            break;
        }

        default:
            print_error_msg("Unhandled type");
            break;
        }
    }
    go_to_next_line();
}

uint64_t OnDiskImport::get_edge_key_id(const std::string& column_name)
{
    auto it = edge_keys_map.find(column_name);
    if (it != edge_keys_map.end()) {
        return it->second;
    } else {
        auto res = current_edge_key++ | ObjectId::MASK_EDGE_KEY;
        catalog.edge_keys_str.push_back(column_name);
        edge_keys_map.insert({ column_name, res });
        return res;
    }
}

void OnDiskImport::save_edge_line()
{
    if (columns[column_with_id_from].value_size == 0 || columns[column_with_id_to].value_size == 0) {
        print_error_msg("The edge has missing IDs and they are required. The edge will not be saved");
        go_to_next_line();
        return;
    }

    uint64_t id_node_from, id_node_to;
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
        id_node_from = csvid_global[columns[column_with_id_from].value_str];
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
        id_node_from = csvid_groups[current_group_idx_from][columns[column_with_id_from].value_str];
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
        id_node_to = csvid_global[columns[column_with_id_to].value_str];
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
        id_node_to = csvid_groups[current_group_idx_to][columns[column_with_id_to].value_str];
    }

    uint64_t edge_id;
    if (has_direction) {
        edge_id = catalog.directed_edges_count++ | ObjectId::MASK_DIRECTED_EDGE;
        directed_edges.push_back({ id_node_from, id_node_to, edge_id });
        if (id_node_from == id_node_to) {
            directed_equal_edges.push_back({ id_node_from, edge_id });
        }
    } else {
        edge_id = catalog.undirected_edges_count++ | ObjectId::MASK_UNDIRECTED_EDGE;
        undirected_edges.push_back({ id_node_from, id_node_to, edge_id });
        if (id_node_from == id_node_to) {
            undirected_equal_edges.push_back({ id_node_from, edge_id });
        } else {
            undirected_edges.push_back({ id_node_to, id_node_from, edge_id });
        }
    }

    for (auto& col : columns) {
        if (col.value_size == 0)
            continue;
        switch (col.type) {
        // START_ID, END_ID and ID should do nothing
        case CSVType::START_ID: {
            break;
        }
        case CSVType::END_ID: {
            break;
        }
        case CSVType::ID: {
            break;
        }
        case CSVType::LABEL: {
            std::vector<std::string> labels = split(col.value_str, label_splitter);
            for (auto label : labels) {
                if (label.size() == 0)
                    continue;

                uint64_t label_id;

                auto it = edge_labels_map.find(label);
                if (it != edge_labels_map.end()) {
                    label_id = it->second;
                } else {
                    label_id = current_edge_label++ | ObjectId::MASK_EDGE_LABEL;
                    catalog.edge_labels_str.push_back(label);
                    edge_labels_map.insert({ label, label_id });
                }

                edge_labels.push_back({ edge_id, label_id });
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

            uint64_t key_id = get_edge_key_id(col.name);
            if ((value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                pending_edge_properties->push_back({ edge_id, key_id, value_id });
            } else {
                edge_properties.push_back({ edge_id, key_id, value_id });
            }
            break;
        }
        case CSVType::INT: {
            uint64_t value_id = try_parse_int(col.value_str);
            uint64_t key_id = get_edge_key_id(col.name);
            edge_properties.push_back({ edge_id, key_id, value_id });
            break;
        }
        case CSVType::DECIMAL: {
            uint64_t value_id = try_parse_float(col.value_str);
            uint64_t key_id = get_edge_key_id(col.name);
            edge_properties.push_back({ edge_id, key_id, value_id });
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
            uint64_t key_id = get_edge_key_id(col.name);
            edge_properties.push_back({ edge_id, key_id, value_id });
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
            uint64_t key_id = get_edge_key_id(col.name);
            edge_properties.push_back({ edge_id, key_id, value_id });
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

template<std::size_t N, typename ResolveAndSaveFunc>
inline void OnDiskImport::process_pending(
    std::unique_ptr<DiskVector<N>>& pending_vector,
    const std::string& name,
    const std::string& pending_filename_prefix,
    ResolveAndSaveFunc resolve_and_save_func
)
{
    pending_vector->finish_appends();
    int i = 0;
    while (true) {
        const auto total_pending = pending_vector->get_total_tuples();
        if (total_pending == 0) {
            break;
        }
        std::cout << "pending " + name + ": " << total_pending << std::endl;

        auto old_pending_vector = std::move(pending_vector);
        pending_vector = std::make_unique<DiskVector<N>>(
            db_folder + "/" + pending_filename_prefix + std::to_string(i)
        );
        ++i;

        // advance pending variables for current iteration
        external_helper->advance_pending();
        external_helper->clear_sets();

        old_pending_vector->begin_tuple_iter();
        while (old_pending_vector->has_next_tuple()) {
            const auto& pending_tuple = old_pending_vector->next_tuple();

            resolve_and_save_func(pending_tuple);
        }

        // write out new data
        external_helper->flush_to_disk();
        // close and delete the old pending files
        external_helper->clean_up_old();

        // close and delete old pending file
        pending_vector->finish_appends();
        old_pending_vector->skip_indexing(); // will close and remove file
    }

    // process pending finished, clean up the last pending file
    pending_vector->skip_indexing();
}
