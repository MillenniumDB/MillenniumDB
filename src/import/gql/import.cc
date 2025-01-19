#include "import.h"

#include "graph_models/common/conversions.h"
#include "graph_models/inliner.h"
#include "import/import_helper.h"
#include "misc/fatal_error.h"
#include "misc/unicode_escape.h"
#include "storage/index/hash/strings_hash/strings_hash_bulk_ondisk_import.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"

using namespace Import::GQL;

OnDiskImport::OnDiskImport(const std::string& db_folder, uint64_t buffer_size) :
    catalog("catalog.dat"),
    buffer_size(buffer_size),
    db_folder(db_folder),
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
}

OnDiskImport::~OnDiskImport()
{
    delete[] (state_transitions);
}

uint64_t OnDiskImport::get_str_id()
{
    if (lexer.str_len < 8) {
        return Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
    } else {
        return get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
    }
}

uint64_t OnDiskImport::get_or_create_external_string_id()
{
    // reserve more space if needed
    while (external_strings_end + lexer.str_len + StringManager::MIN_PAGE_REMAINING_BYTES
               + ExternalString::MAX_LEN_BYTES + 1
           >= external_strings_capacity)
    {
        // duplicate buffer
        char* new_external_strings = reinterpret_cast<char*>(
            MDB_ALIGNED_ALLOC(VPage::SIZE, external_strings_capacity * 2)
        );
        std::memcpy(new_external_strings, external_strings, external_strings_capacity);

        external_strings_capacity *= 2;

        MDB_ALIGNED_FREE(external_strings);
        external_strings = new_external_strings;
        ExternalString::strings = external_strings;
    }

    // encode length
    auto bytes_for_len = StringManager::write_encoded_strlen(
        &external_strings[external_strings_end],
        lexer.str_len
    );

    // copy string
    std::memcpy(&external_strings[external_strings_end] + bytes_for_len, lexer.str, lexer.str_len);

    ExternalString s(external_strings_end);
    auto found = external_strings_set.find(s);
    if (found == external_strings_set.end()) {
        external_strings_set.insert(s);
        external_strings_end += lexer.str_len + bytes_for_len;

        size_t remaining_in_block = StringManager::STRING_BLOCK_SIZE
                                  - (external_strings_end % StringManager::STRING_BLOCK_SIZE);
        if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
            external_strings_end += remaining_in_block;
        }

        return s.offset;
    } else {
        return found->offset;
    }
}

uint64_t OnDiskImport::get_node_id(uint64_t id)
{
    auto it = node_ids_map.find(id);
    if (it != node_ids_map.end()) {
        return it->second;
    } else {
        auto res = catalog.nodes_count++ | ObjectId::MASK_NODE;
        node_ids_map.insert({ id, res });

        return res;
    }
}

uint64_t OnDiskImport::get_node_label_id(const std::string& label)
{
    auto it = node_labels_map.find(label);
    if (it != node_labels_map.end()) {
        return it->second;
    } else {
        auto res = current_node_label++ | ObjectId::MASK_NODE_LABEL;

        catalog.node_labels_str.push_back(label);
        node_labels_map.insert({ label, res });
        return res;
    }
}

uint64_t OnDiskImport::get_edge_label_id(const std::string& label)
{
    auto it = edge_labels_map.find(label);
    if (it != edge_labels_map.end()) {
        return it->second;
    } else {
        auto res = current_edge_label++ | ObjectId::MASK_EDGE_LABEL;

        catalog.edge_labels_str.push_back(label);
        edge_labels_map.insert({ label, res });
        return res;
    }
}

uint64_t OnDiskImport::get_node_key_id()
{
    auto it = node_keys_map.find(saved_key);
    if (it != node_keys_map.end()) {
        return it->second;
    } else {
        auto res = current_node_key++ | ObjectId::MASK_NODE_KEY;
        catalog.node_keys_str.push_back(saved_key);
        node_keys_map.insert({ saved_key, res });
        return res;
    }
}

uint64_t OnDiskImport::get_edge_key_id()
{
    auto it = edge_keys_map.find(saved_key);
    if (it != edge_keys_map.end()) {
        return it->second;
    } else {
        auto res = current_edge_key++ | ObjectId::MASK_EDGE_KEY;
        catalog.edge_keys_str.push_back(saved_key);
        edge_keys_map.insert({ saved_key, res });
        return res;
    }
}

int64_t try_parse_int(char* c_str)
{
    return Common::Conversions::pack_int(atoll(c_str)).id;
}

int64_t try_parse_float(char* c_str)
{
    return Common::Conversions::pack_float(atof(c_str)).id;
}

void OnDiskImport::save_first_id_identifier()
{
    id1 = get_str_id();
}

void OnDiskImport::save_first_id_string()
{
    normalize_string_literal();

    id1 = get_str_id();
}

void OnDiskImport::save_first_id_int()
{
    id1 = try_parse_int(lexer.str);
}

void OnDiskImport::save_prop_key()
{
    saved_key = std::string(lexer.str, lexer.str_len);
}

void OnDiskImport::save_direction(EdgeDir dir)
{
    this->edge_dir = dir;
}

void OnDiskImport::add_node_label()
{
    auto node_id = get_node_id(id1);
    auto label_id = get_node_label_id(std::string(lexer.str, lexer.str_len));
    node_labels.push_back({ node_id, label_id });
}

void OnDiskImport::add_node_prop_string()
{
    normalize_string_literal();
    auto node_id = get_node_id(id1);
    auto key_id = get_node_key_id();
    auto value_id = get_str_id();
    node_properties.push_back({ node_id, key_id, value_id });
}

void OnDiskImport::add_node_prop_int()
{
    auto node_id = get_node_id(id1);
    auto key_id = get_node_key_id();
    uint64_t value_id = try_parse_int(lexer.str);
    node_properties.push_back({ node_id, key_id, value_id });
}

void OnDiskImport::add_node_prop_float()
{
    auto node_id = get_node_id(id1);
    auto key_id = get_node_key_id();
    uint64_t value_id = try_parse_float(lexer.str);
    node_properties.push_back({ node_id, key_id, value_id });
}

void OnDiskImport::add_node_prop_true()
{
    auto node_id = get_node_id(id1);
    auto key_id = get_node_key_id();
    uint64_t value_id = Common::Conversions::pack_bool(true).id;
    node_properties.push_back({ node_id, key_id, value_id });
}

void OnDiskImport::add_node_prop_false()
{
    auto node_id = get_node_id(id1);
    auto key_id = get_node_key_id();
    uint64_t value_id = Common::Conversions::pack_bool(false).id;
    node_properties.push_back({ node_id, key_id, value_id });
}

void OnDiskImport::add_node_prop_datatype()
{
    add_prop_datatype(get_node_id(id1), node_properties);
}

void OnDiskImport::add_prop_datatype(uint64_t obj_id, DiskVector<3>& obj_properties)
{
    // we have something like: `datatype("string")`
    // parse datatype name
    char* datatype_beg = lexer.str;
    char* datatype_end = lexer.str;
    while (isalpha(*datatype_end)) {
        datatype_end++;
    }
    *datatype_end = '\0';

    char* str_value_end = lexer.str + (lexer.str_len - 1);
    lexer.str = datatype_end + 1;
    while (*lexer.str != '"') {
        lexer.str++;
    }

    // it may have whitespaces `datatype("string"  )` so we iterate
    while (*str_value_end != '"') {
        str_value_end--;
    }
    lexer.str_len = (str_value_end - lexer.str) + 1;

    // we edited lexer.str_len and lexer.str to point correctly at the datatype (considering quotes)
    normalize_string_literal();

    uint64_t value_id;
    if (strcmp(datatype_beg, "dateTime") == 0) {
        value_id = DateTime::from_dateTime(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << " invalid dateTime: " << lexer.str << "\n";
            return;
        }
    } else if (strcmp(datatype_beg, "date") == 0) {
        value_id = DateTime::from_date(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "invalid date: " << lexer.str << "\n";
            return;
        }
    } else if (strcmp(datatype_beg, "time") == 0) {
        value_id = DateTime::from_time(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "invalid time: " << lexer.str << "\n";
            return;
        }
    } else if (strcmp(datatype_beg, "dateTimeStamp") == 0) {
        value_id = DateTime::from_dateTimeStamp(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "invalid dateTimeStamp: " << lexer.str << "\n";
            return;
        }
    } else {
        parsing_errors++;
        std::cout << "ERROR on line " << current_line << ", ";
        std::cout << "unknown datatype: " << datatype_beg << "\n";
        return;
    }

    // auto node_id = ;
    auto key_id = get_node_key_id();
    obj_properties.push_back({ obj_id, key_id, value_id });
}

void OnDiskImport::save_second_id_identifier()
{
    id2 = get_str_id();
    save_edge();
}

void OnDiskImport::save_second_id_string()
{
    normalize_string_literal();
    id2 = get_str_id();
    save_edge();
}

void OnDiskImport::save_second_id_int()
{
    id2 = try_parse_int(lexer.str);
    save_edge();
}

void OnDiskImport::save_edge()
{
    auto n1 = get_node_id(id1);
    auto n2 = get_node_id(id2);
    if (edge_dir == EdgeDir::UNDIRECTED) {
        edge_id = ObjectId::MASK_UNDIRECTED_EDGE | catalog.undirected_edges_count++;

        undirected_edges.push_back({ n1, n2, edge_id });
        if (n1 != n2) {
            undirected_edges.push_back({ n2, n1, edge_id });
        } else {
            undirected_equal_edges.push_back({ n1, edge_id });
        }
    } else {
        edge_id = ObjectId::MASK_DIRECTED_EDGE | catalog.directed_edges_count++;

        if (edge_dir == EdgeDir::RIGHT) {
            directed_edges.push_back({ n1, n2, edge_id });
        } else {
            directed_edges.push_back({ n2, n1, edge_id });
        }

        if (n1 == n2) {
            directed_equal_edges.push_back({ n1, edge_id });
        }
    }
}

void OnDiskImport::add_edge_label()
{
    auto label_id = get_edge_label_id(std::string(lexer.str, lexer.str_len));
    edge_labels.push_back({ edge_id, label_id });
}

void OnDiskImport::add_edge_prop_datatype()
{
    add_prop_datatype(edge_id, edge_properties);
}

void OnDiskImport::add_edge_prop_string()
{
    normalize_string_literal();
    auto key_id = get_edge_key_id();
    auto value_id = get_str_id();
    edge_properties.push_back({ edge_id, key_id, value_id });
}

void OnDiskImport::add_edge_prop_int()
{
    auto key_id = get_edge_key_id();
    uint64_t value_id = try_parse_int(lexer.str);
    edge_properties.push_back({ edge_id, key_id, value_id });
}

void OnDiskImport::add_edge_prop_float()
{
    auto key_id = get_edge_key_id();
    uint64_t value_id = try_parse_float(lexer.str);
    edge_properties.push_back({ edge_id, key_id, value_id });
}

void OnDiskImport::add_edge_prop_true()
{
    auto key_id = get_edge_key_id();
    uint64_t value_id = Common::Conversions::pack_bool(true).id;
    edge_properties.push_back({ edge_id, key_id, value_id });
}

void OnDiskImport::add_edge_prop_false()
{
    auto key_id = get_edge_key_id();
    uint64_t value_id = Common::Conversions::pack_bool(false).id;
    edge_properties.push_back({ edge_id, key_id, value_id });
}

void OnDiskImport::finish_line()
{
    current_line++;
}

void OnDiskImport::print_error()
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << "\n";
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

    // error clears after an endline
    set_transition(
        State::WRONG_LINE,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    // ignore empty lines
    set_transition(
        State::LINE_BEGIN,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    set_transition(
        State::LINE_BEGIN,
        Token::IDENTIFIER,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_identifier, this)
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
        State::FIRST_ID,
        Token::COLON,
        State::EXPECT_NODE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );
    set_transition(
        State::FIRST_ID,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );
    set_transition(
        State::FIRST_ID,
        Token::IDENTIFIER,
        State::EXPECT_NODE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );
    set_transition(
        State::FIRST_ID,
        Token::L_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::save_direction, this, EdgeDir::LEFT)
    );
    set_transition(
        State::FIRST_ID,
        Token::R_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::save_direction, this, EdgeDir::RIGHT)
    );
    set_transition(
        State::FIRST_ID,
        Token::UNDIRECTED,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::save_direction, this, EdgeDir::UNDIRECTED)
    );

    set_transition(
        State::NODE_DEFINED,
        Token::COLON,
        State::EXPECT_NODE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );
    set_transition(
        State::NODE_DEFINED,
        Token::IDENTIFIER,
        State::EXPECT_NODE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );
    set_transition(
        State::NODE_DEFINED,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    set_transition(
        State::EXPECT_NODE_LABEL,
        Token::IDENTIFIER,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_label, this)
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
        State::EXPECT_EDGE_SECOND,
        Token::IDENTIFIER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_second_id_identifier, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::STRING,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_second_id_string, this)
    );
    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::INTEGER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_second_id_int, this)
    );

    set_transition(
        State::EDGE_DEFINED,
        Token::COLON,
        State::EXPECT_EDGE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );
    set_transition(
        State::EDGE_DEFINED,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
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
        State::EXPECT_EDGE_LABEL,
        Token::IDENTIFIER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_label, this)
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

void OnDiskImport::start_import(MDBIstream& in)
{
    auto start = std::chrono::system_clock::now();
    auto import_start = start;

    lexer.begin(in);

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

    int current_state = State::LINE_BEGIN;
    current_line = 1;
    while (auto token = lexer.get_token()) {
        // std::cout << "token: ";
        // switch (token) {
        //     case Token::END_OF_FILE:
        //         std::cout << "END_OF_FILE\n";
        //         break;
        //     case Token::COLON:
        //         std::cout << "COLON\n";
        //         break;
        //     case Token::L_ARROW:
        //         std::cout << "L_ARROW\n";
        //         break;
        //     case Token::R_ARROW:
        //         std::cout << "R_ARROW\n";
        //         break;
        //     case Token::UNDIRECTED:
        //         std::cout << "UNDIRECTED\n";
        //         break;
        //     case Token::TRUE:
        //         std::cout << "TRUE\n";
        //         break;
        //     case Token::FALSE:
        //         std::cout << "FALSE\n";
        //         break;
        //     case Token::STRING:
        //         std::cout << "STRING\n";
        //         break;
        //     case Token::TYPED_STRING:
        //         std::cout << "TYPED_STRING\n";
        //         break;
        //     case Token::IDENTIFIER:
        //         std::cout << "IDENTIFIER\n";
        //         break;
        //     case Token::INTEGER:
        //         std::cout << "INTEGER\n";
        //         break;
        //     case Token::FLOAT:
        //         std::cout << "FLOAT\n";
        //         break;
        //     case Token::WHITESPACE:
        //         std::cout << "WHITESPACE\n";
        //         break;
        //     case Token::ENDLINE:
        //         std::cout << "ENDLINE\n";
        //         break;
        //     case Token::UNRECOGNIZED:
        //         std::cout << "UNRECOGNIZED\n";
        //         break;
        // }
        current_state = get_transition(current_state, token);
        // std::cout << "current_state: ";
        // switch (current_state) {
        //     case State::WRONG_LINE:
        //         std::cout << "WRONG_LINE\n";
        //         break;
        //     case State::LINE_BEGIN:
        //         std::cout << "LINE_BEGIN\n";
        //         break;
        //     case State::FIRST_ID:
        //         std::cout << "FIRST_ID\n";
        //         break;
        //     case State::NODE_DEFINED:
        //         std::cout << "NODE_DEFINED\n";
        //         break;
        //     case State::EXPECT_NODE_LABEL:
        //         std::cout << "EXPECT_NODE_LABEL\n";
        //         break;
        //     case State::EXPECT_NODE_PROP_COLON:
        //         std::cout << "EXPECT_NODE_PROP_COLON\n";
        //         break;
        //     case State::EXPECT_NODE_PROP_VALUE:
        //         std::cout << "EXPECT_NODE_PROP_VALUE\n";
        //         break;
        //     case State::EXPECT_EDGE_SECOND:
        //         std::cout << "EXPECT_EDGE_SECOND\n";
        //         break;
        //     case State::EDGE_DEFINED:
        //         std::cout << "EDGE_DEFINED\n";
        //         break;
        //     case State::EXPECT_EDGE_LABEL:
        //         std::cout << "EXPECT_EDGE_LABEL\n";
        //         break;
        //     case State::EXPECT_EDGE_PROP_COLON:
        //         std::cout << "EXPECT_EDGE_PROP_COLON\n";
        //         break;
        //     case State::EXPECT_EDGE_PROP_VALUE:
        //         std::cout << "EXPECT_EDGE_PROP_VALUE\n";
        //         break;
        // }
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

    { // write node mapping as properties
        saved_key = "_id";
        auto key_id = get_node_key_id();
        for (auto&& [k, v] : node_ids_map) {
            node_properties.push_back({ v, key_id, k });
        }
    }

    { // Create StringsHash
        // Big enough buffer to store any string
        char* string_buffer = reinterpret_cast<char*>(
            MDB_ALIGNED_ALLOC(VPage::SIZE, StringManager::STRING_BLOCK_SIZE)
        );

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
            size_t remaining_in_block = StringManager::STRING_BLOCK_SIZE
                                      - (current_pos % StringManager::STRING_BLOCK_SIZE);

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
    node_labels.finish_appends();
    edge_labels.finish_appends();
    node_properties.finish_appends();
    edge_properties.finish_appends();
    directed_edges.finish_appends();
    undirected_edges.finish_appends();
    directed_equal_edges.finish_appends();
    undirected_equal_edges.finish_appends();

    { // DIRECTED EDGES TABLE
        EdgeTableMemImport<3> table_writer(db_folder + "/d_edges.table");

        directed_edges.begin_tuple_iter();
        while (directed_edges.has_next_tuple()) {
            auto& tuple = directed_edges.next_tuple();
            table_writer.insert_tuple(tuple);
        }
    }

    { // UNDIRECTED EDGES TABLE
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

    // TODO: create empty bpt of deleted nodes?

    { // Node Labels B+Tree
        size_t COL_NODE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        LabelStat label_stat;

        node_labels.create_bpt(db_folder + "/node_label", { COL_NODE, COL_LABEL }, no_stat);

        node_labels.create_bpt(db_folder + "/label_node", { COL_LABEL, COL_NODE }, label_stat);
        catalog.node_labels_count = label_stat.all;
        label_stat.end();

        catalog.node_label2total_count = std::move(label_stat.map_label_count);
    }

    { // Edge Labels B+Tree
        size_t COL_EDGE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        LabelStat label_stat;

        edge_labels.create_bpt(db_folder + "/edge_label", { COL_EDGE, COL_LABEL }, no_stat);

        edge_labels.create_bpt(db_folder + "/label_edge", { COL_LABEL, COL_EDGE }, label_stat);
        catalog.edge_labels_count = label_stat.all;
        label_stat.end();

        catalog.edge_label2total_count = std::move(label_stat.map_label_count);
    }

    { // Node Properties B+Tree
        size_t COL_NODE = 0, COL_KEY = 1, COL_VAL = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        node_properties.create_bpt(db_folder + "/node_key_value", { COL_NODE, COL_KEY, COL_VAL }, no_stat);
        node_properties.create_bpt(db_folder + "/key_value_node", { COL_KEY, COL_VAL, COL_NODE }, prop_stat);

        catalog.node_properties_count = prop_stat.all;
        prop_stat.end();

        catalog.node_key2total_count = std::move(prop_stat.map_key_count);
    }

    { // Edge Properties B+Tree
        size_t COL_EDGE = 0, COL_KEY = 1, COL_VAL = 2;

        NoStat<3> no_stat;
        PropStat prop_stat;

        edge_properties.create_bpt(db_folder + "/edge_key_value", { COL_EDGE, COL_KEY, COL_VAL }, no_stat);
        edge_properties.create_bpt(db_folder + "/key_value_edge", { COL_KEY, COL_VAL, COL_EDGE }, prop_stat);

        catalog.edge_properties_count = prop_stat.all;
        prop_stat.end();

        catalog.edge_key2total_count = std::move(prop_stat.map_key_count);
    }

    { // UNDIRECTED EDGES
        size_t COL_N1 = 0, COL_N2 = 1, COL_EDGE = 2;
        NoStat<3> stat;

        undirected_edges.create_bpt(db_folder + "/u_edge", { COL_N1, COL_N2, COL_EDGE }, stat);
    }

    { // DIRECTED EDGES
        size_t COL_FROM = 0, COL_TO = 1, COL_EDGE = 2;
        NoStat<3> no_stat;

        directed_edges.create_bpt(db_folder + "/from_to_edge", { COL_FROM, COL_TO, COL_EDGE }, no_stat);
        directed_edges.create_bpt(db_folder + "/to_from_edge", { COL_TO, COL_FROM, COL_EDGE }, no_stat);
    }

    { // UNDIRECTED EQUAL EDGES
        AllStat<2> all_stat;
        undirected_equal_edges.create_bpt(db_folder + "/equal_u_edge", { 0, 1 }, all_stat);
        catalog.equal_undirected_edges_count = all_stat.all;
    }

    { // DIRECTED EQUAL EDGES
        AllStat<2> all_stat;
        directed_equal_edges.create_bpt(db_folder + "/equal_d_edge", { 0, 1 }, all_stat);
        catalog.equal_directed_edges_count = all_stat.all;
    }

    // calling finish_indexing() closes and removes the file.
    node_labels.finish_indexing();
    edge_labels.finish_indexing();
    node_properties.finish_indexing();
    edge_properties.finish_indexing();
    directed_edges.finish_indexing();
    undirected_edges.finish_indexing();
    directed_equal_edges.finish_indexing();
    undirected_equal_edges.finish_indexing();
    MDB_ALIGNED_FREE(external_strings);

    print_duration("Write B+tree indexes", start);

    catalog.print(std::cout);

    print_duration("Total Import", import_start);
}

void OnDiskImport::set_transition(int state, int token, int value, std::function<void()> func)
{
    state_funcs[State::TOTAL_STATES * state + token] = func;
    state_transitions[State::TOTAL_STATES * state + token] = value;
}

int OnDiskImport::get_transition(int state, int token)
{
    try {
        state_funcs[State::TOTAL_STATES * state + token]();
        return state_transitions[State::TOTAL_STATES * state + token];
    } catch (std::exception& e) {
        parsing_errors++;
        std::cout << "ERROR on line " << current_line << "\n";
        std::cout << e.what() << "\n";
        return State::WRONG_LINE;
    }
}

void OnDiskImport::normalize_string_literal()
{
    char* write_ptr = lexer.str;
    char* read_ptr = write_ptr + 1; // skip first character: '"'

    lexer.str_len -= 2;
    char* end = lexer.str + lexer.str_len + 1;

    UnicodeEscape::normalize_string(read_ptr, write_ptr, end, lexer.str_len);
}
