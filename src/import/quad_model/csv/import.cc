#include "import.h"
#include "import/import_helper.h"
#include "misc/fatal_error.h"
#include "misc/unicode_escape.h"
#include "storage/index/hash/strings_hash/strings_hash_bulk_ondisk_import.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"
#include <cctype>

using namespace Import::QuadModel::CSV;

OnDiskImport::OnDiskImport(const std::string& db_folder, uint64_t buffer_size) :
    buffer_size(buffer_size),
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
    label_splitter = ';'; // TODO: Make this modifiable in the constructor
}

OnDiskImport::~OnDiskImport()
{
    delete[] state_transitions;
}

void OnDiskImport::start_import(
    std::vector<std::unique_ptr<MDBIstreamFiles>>& in_nodes,
    std::vector<std::unique_ptr<MDBIstreamFiles>>& in_edges
)
{
    auto start = std::chrono::system_clock::now();

    external_strings = reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(buffer_size));
    if (external_strings == nullptr)
        FATAL_ERROR("Could not allocate buffer, try using a smaller buffer size");

    Import::ExternalString::strings = external_strings;
    external_strings_capacity = buffer_size;
    external_strings_end = 0;
    assert(external_strings_capacity > StringManager::MAX_STRING_SIZE);

    // First, import nodes to the database. After that, import relationships
    parse_node_files(in_nodes);
    parse_edge_files(in_edges);

    // Write external strings file
    std::fstream strings_file;
    strings_file.open(db_folder + "/strings.dat", std::ios::out | std::ios::binary);
    strings_file.write(external_strings, external_strings_end);
    uint64_t last_str_pos = strings_file.tellp();

    external_strings_set.clear();

    {
        char* string_buffer = reinterpret_cast<char*>(
            MDB_ALIGNED_ALLOC(StringManager::MAX_STRING_SIZE)
        );

        StringsHashBulkOnDiskImport string_hash(
            db_folder + "/str_hash",
            external_strings,
            external_strings_capacity
        );
        strings_file.close();
        strings_file.open(db_folder + "/strings.dat", std::ios::in | std::ios::binary);
        strings_file.seekg(0, strings_file.beg);
        uint64_t current_pos = 0;

        // Read all strings one by one and add them to the StringHash
        while (current_pos < last_str_pos) {
            auto remaining_in_block = StringManager::BLOCK_SIZE - (current_pos % StringManager::BLOCK_SIZE);

            if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
                current_pos += remaining_in_block;
                strings_file.read(string_buffer, remaining_in_block);
            }

            auto str_len = read_str(strings_file, string_buffer);
            string_hash.create_id(string_buffer, current_pos, str_len);
            current_pos = strings_file.tellg();
        }

        strings_file.close();
        MDB_ALIGNED_FREE(string_buffer);
    }

    char* const buffer = external_strings;
    buffer_size = external_strings_capacity;

    declared_nodes.finish_appends();
    labels.finish_appends();
    properties.finish_appends();
    edges.finish_appends();
    equal_from_to.finish_appends();
    equal_from_type.finish_appends();
    equal_to_type.finish_appends();
    equal_from_to_type.finish_appends();

    {
        boost::unordered_flat_set<uint64_t> nodes_set;

        declared_nodes.begin_tuple_iter();
        while (declared_nodes.has_next_tuple()) {
            auto& tuple = declared_nodes.next_tuple();
            nodes_set.insert(tuple[0]);
        }

        EdgeTableMemImport<4> table_writer(db_folder + "/edges.table");

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

        declared_nodes.finish_appends();
        catalog.identifiable_nodes_count = nodes_set.size();
    }

    declared_nodes.start_indexing(buffer, buffer_size, { 0 });
    labels.start_indexing(buffer, buffer_size, { 0, 1 });
    properties.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    edges.start_indexing(buffer, buffer_size, { 0, 1, 2, 3 });
    equal_from_to.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    equal_from_type.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    equal_to_type.start_indexing(buffer, buffer_size, { 0, 1 });
    equal_from_to_type.start_indexing(buffer, buffer_size, { 0, 1 });

    // Nodes
    {
        size_t COL_NODE = 0;
        NoStat<1> no_stat;

        declared_nodes.create_bpt(db_folder + "/nodes", { COL_NODE }, no_stat);
    }

    // Labels
    {
        size_t COL_NODE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        LabelStat label_stat;

        labels.create_bpt(db_folder + "/node_label", { COL_NODE, COL_LABEL }, no_stat);

        labels.create_bpt(db_folder + "/label_node", { COL_LABEL, COL_NODE }, label_stat);
        catalog.label_count = label_stat.all;
        label_stat.end();

        catalog.label2total_count = std::move(label_stat.map_label_count);
    }

    // Properties
    {
        size_t COL_OBJ = 0, COL_KEY = 1, COL_VALUE = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        properties.create_bpt(db_folder + "/object_key_value", { COL_OBJ, COL_KEY, COL_VALUE }, no_stat);

        properties.create_bpt(db_folder + "/key_value_object", { COL_KEY, COL_VALUE, COL_OBJ }, prop_stat);
        catalog.properties_count = prop_stat.all;
        prop_stat.end();

        catalog.key2total_count = std::move(prop_stat.map_key_count);
    }

    // Edges
    {
        size_t COL_FROM = 0, COL_TO = 1, COL_TYPE = 2, COL_EDGE = 3;

        NoStat<4> no_stat;
        AllStat<4> all_stat;

        edges
            .create_bpt(db_folder + "/from_to_type_edge", { COL_FROM, COL_TO, COL_TYPE, COL_EDGE }, all_stat);
        catalog.edge_count = all_stat.all;

        edges.create_bpt(db_folder + "/to_type_from_edge", { COL_TO, COL_TYPE, COL_FROM, COL_EDGE }, no_stat);
        edges.create_bpt(db_folder + "/type_from_to_edge", { COL_TYPE, COL_FROM, COL_TO, COL_EDGE }, no_stat);
        edges.create_bpt(db_folder + "/type_to_from_edge", { COL_TYPE, COL_TO, COL_FROM, COL_EDGE }, no_stat);
    }

    // From=To=Type edge
    {
        size_t COL_FROM_TO_TYPE = 0, COL_EDGE = 1;

        DictCountStat<2> stat;
        equal_from_to_type
            .create_bpt(db_folder + "/equal_from_to_type", { COL_FROM_TO_TYPE, COL_EDGE }, stat);
        catalog.equal_from_to_type_count = stat.all;

        stat.end();
        catalog.type2equal_from_to_count = std::move(stat.dict);
    }

    // From=To type edge
    {
        size_t COL_FROM_TO = 0, COL_TYPE = 1, COL_EDGE = 2;

        NoStat<3> no_stat;
        equal_from_to.create_bpt(db_folder + "/equal_from_to", { COL_FROM_TO, COL_TYPE, COL_EDGE }, no_stat);

        DictCountStat<3> stat;
        equal_from_to
            .create_bpt(db_folder + "/equal_from_to_inverted", { COL_FROM_TO, COL_TYPE, COL_EDGE }, stat);
        stat.end();

        catalog.equal_from_to_count = stat.all;
        catalog.type2equal_from_to_count = std::move(stat.dict);
    }

    // From=Type to edge
    {
        size_t COL_FROM_TYPE = 0, COL_TO = 1, COL_EDGE = 2;

        DictCountStat<3> stat;
        equal_from_type.create_bpt(db_folder + "/equal_from_type", { COL_FROM_TYPE, COL_TO, COL_EDGE }, stat);
        stat.end();
        catalog.equal_from_to_type_count = stat.all;
        catalog.type2equal_from_type_count = std::move(stat.dict);

        NoStat<3> no_stat;
        equal_from_type.create_bpt(
            db_folder + "/equal_from_type_inverted",
            { COL_TO, COL_FROM_TYPE, COL_EDGE },
            no_stat
        );
    }

    //To=Type from edge
    {
        size_t COL_FROM_TYPE = 0, COL_TO = 1, COL_EDGE = 2;

        DictCountStat<3> stat;
        equal_from_type.create_bpt(db_folder + "/equal_to_type", { COL_FROM_TYPE, COL_TO, COL_EDGE }, stat);
        stat.end();
        catalog.equal_from_to_type_count = stat.all;
        catalog.type2equal_from_type_count = std::move(stat.dict);

        NoStat<3> no_stat;
        equal_from_type
            .create_bpt(db_folder + "/equal_to_type_inverted", { COL_TO, COL_FROM_TYPE, COL_EDGE }, no_stat);
    }

    declared_nodes.finish_indexing();
    labels.finish_indexing();
    properties.finish_indexing();
    edges.finish_indexing();
    equal_from_to.finish_indexing();
    equal_from_type.finish_indexing();
    equal_to_type.finish_indexing();
    equal_from_to_type.finish_indexing();
    MDB_ALIGNED_FREE(external_strings);

    catalog.print(std::cout);
    print_duration("Total import time", start);
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

uint64_t OnDiskImport::get_or_create_external_string_id(const std::string& str)
{
    while (external_strings_end
        + str.size()
        + StringManager::MIN_PAGE_REMAINING_BYTES
        + StringManager::MAX_LEN_BYTES
        + 1 >= external_strings_capacity)
    {
        // Duplicate buffer
        char* new_external_strings = reinterpret_cast<char*>(
            MDB_ALIGNED_ALLOC(external_strings_capacity * 2)
        );
        std::memcpy(new_external_strings, external_strings, external_strings_capacity);
        external_strings_capacity *= 2;

        MDB_ALIGNED_FREE(external_strings);
        external_strings = new_external_strings;
        ExternalString::strings = external_strings;
    }

    // Encode length
    auto bytes_for_len = StringManager::write_encoded_strlen(
        &external_strings[external_strings_end],
        str.size());

    // copy string
    std::memcpy(
        &external_strings[external_strings_end] + bytes_for_len,
        str.data(),
        str.size());

    ExternalString s(external_strings_end);
    auto found = external_strings_set.find(s);
    if (found == external_strings_set.end()) {
        external_strings_set.insert(s);
        external_strings_end += str.size() + bytes_for_len;

        size_t remaining_in_block = StringManager::BLOCK_SIZE
                                    - (external_strings_end % StringManager::BLOCK_SIZE);
        if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
            external_strings_end += remaining_in_block;
        }

        return s.offset;
    } else {
        return found->offset;
    }
}

void OnDiskImport::parse_node_files(std::vector<std::unique_ptr<MDBIstreamFiles>>& in_nodes)
{
    std::cout << "Importing nodes\n";
    for (auto& in_file : in_nodes) {
        current_state = State::START_HEADER_NODES;
        lexer.begin(*in_file);
        std::cout << "Reading file " << in_file->filenames[in_file->filename_index] << std::endl;

        while (auto token = lexer.get_token()) {
            current_token = token;
            current_state = get_transition(current_state, token);
        }

        if (current_token != Token::ENDLINE && current_token != Token::END_OF_FILE) {
            save_node_to_disk();
        }

        reset_automata();
    }
}

void OnDiskImport::parse_edge_files(std::vector<std::unique_ptr<MDBIstreamFiles>>& in_edges)
{
    std::cout << "Importing edges\n";
    for (auto& in_file : in_edges) {
        current_state = State::START_HEADER_EDGES;
        lexer.begin(*in_file);
        std::cout << "Reading file " << in_file->filenames[in_file->filename_index] << std::endl;

        while (auto token = lexer.get_token()) {
            current_token = token;
            current_state = get_transition(current_state, token);
        }

        if (current_token != Token::ENDLINE && current_token != Token::END_OF_FILE) {
            save_edge_to_disk();
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
    if (split_new_col[0].size() < 8)
        new_column_key_id = Inliner::inline_string(split_new_col[0].c_str())
                          | ObjectId::MASK_STRING_SIMPLE_INLINED;
    else
        new_column_key_id = get_or_create_external_string_id(split_new_col[0])
                          | ObjectId::MASK_STRING_SIMPLE_EXTERN;

    columns.push_back(CSVColumn(new_column_type, split_new_col[0], new_column_key_id));
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

void OnDiskImport::save_node_to_disk()
{
    uint64_t node_id;
    if (anonymous_nodes) {
        node_id = current_id | ObjectId::MASK_ANON_INLINED;
        declared_nodes.push_back({ node_id });
    } else if (!anonymous_nodes && columns[column_with_id].value_size == 0) {
        print_error_msg("No ID was given for the node. It will be saved as an anonymous node");
        node_id = current_id | ObjectId::MASK_ANON_INLINED;
        declared_nodes.push_back({ node_id });
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
            bool is_number = true;
            for (int char_idx = 0; char_idx < (int) columns[column_with_id].value_size; char_idx++) {
                if (!std::isdigit(columns[column_with_id].value_str[char_idx]))
                    is_number = false;
            }

            if (is_number) {
                node_id = try_parse_int(columns[column_with_id].value_str);
            } else {
                normalize_string_literal(columns[column_with_id]);
                if (columns[column_with_id].value_size < 8)
                    node_id = Inliner::inline_string(columns[column_with_id].value_str)
                            | ObjectId::MASK_NAMED_NODE_INLINED;
                else
                    node_id = get_or_create_external_string_id(columns[column_with_id].value_str)
                            | ObjectId::MASK_NAMED_NODE_EXTERN;
            }

            declared_nodes.push_back({ node_id });
            csvid_global.insert({ columns[column_with_id].value_str, node_id });
        } else {
            // Using IDs inside the scope of a group
            node_id = current_id | ObjectId::MASK_ANON_INLINED;
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
            declared_nodes.push_back({ node_id });
            csvid_groups[current_group_idx].insert({ columns[column_with_id].value_str, node_id });
        }
    }

    for (auto& col : columns) {
        // TODO: Handle empty properties (do not add)
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
                    label_id = get_or_create_external_string_id(label) | ObjectId::MASK_STRING_SIMPLE_EXTERN;
                }

                labels.push_back({ node_id, label_id });
            }
            break;
        }
        case CSVType::STR: {
            uint64_t value_id;
            normalize_string_literal(col);

            if (col.value_size < 8)
                value_id = Inliner::inline_string(col.value_str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
            else
                value_id = get_or_create_external_string_id(col.value_str)
                         | ObjectId::MASK_STRING_SIMPLE_EXTERN;

            properties.push_back({ node_id, col.key_id, value_id });
            break;
        }
        case CSVType::INT: {
            uint64_t value_id = try_parse_int(col.value_str);
            properties.push_back({ node_id, col.key_id, value_id });
            break;
        }
        case CSVType::DECIMAL: {
            uint64_t value_id = try_parse_float(col.value_str);
            properties.push_back({ node_id, col.key_id, value_id });
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
            properties.push_back({ node_id, col.key_id, value_id });
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
            properties.push_back({ node_id, col.key_id, value_id });
            break;
        }

        default:
            print_error_msg("Unhandled type");
            break;
        }
    }

    current_id++;
    go_to_next_line();
}

void OnDiskImport::save_edge_to_disk()
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
        type_id = get_or_create_external_string_id(columns[column_with_type].value_str)
                | ObjectId::MASK_NAMED_NODE_EXTERN;

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

    uint64_t edge_id = edge_count++ | ObjectId::MASK_EDGE;

    edges.push_back({ id_node_from, id_node_to, type_id, edge_id });
    ++catalog.type2total_count[type_id];

    if (id_node_from == id_node_to) {
        equal_from_to.push_back({ id_node_from, type_id, edge_id });

        if (id_node_from == type_id) {
            equal_from_to_type.push_back({ id_node_from, edge_id });
        }
    }
    if (id_node_from == type_id) {
        equal_from_type.push_back({ id_node_from, id_node_to, edge_id });
    }
    if (id_node_to == type_id) {
        equal_to_type.push_back({ id_node_from, id_node_to, edge_id });
    }

    for (auto& col : columns) {
        if (col.value_size == 0)
            continue;
        switch (col.type) {
        // START_ID, END_ID and TYPE should do nothing
        case CSVType::START_ID: {
            break;
        }
        case CSVType::END_ID: {
            break;
        }
        case CSVType::TYPE: {
            break;
        }
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
                value_id = get_or_create_external_string_id(col.value_str)
                         | ObjectId::MASK_STRING_SIMPLE_EXTERN;

            properties.push_back({ edge_id, col.key_id, value_id });
            break;
        }
        case CSVType::INT: {
            uint64_t value_id = try_parse_int(col.value_str);
            properties.push_back({ edge_id, col.key_id, value_id });
            break;
        }
        case CSVType::DECIMAL: {
            uint64_t value_id = try_parse_float(col.value_str);
            properties.push_back({ edge_id, col.key_id, value_id });
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
            properties.push_back({ edge_id, col.key_id, value_id });
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
            properties.push_back({ edge_id, col.key_id, value_id });
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
