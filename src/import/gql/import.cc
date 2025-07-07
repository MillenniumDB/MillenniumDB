#include "import.h"

#include "graph_models/common/conversions.h"
#include "graph_models/inliner.h"
#include "import/import_helper.h"
#include "misc/unicode_escape.h"
#include "storage/index/random_access_table/edge_table_mem_import.h"

using namespace Import::GQL;

OnDiskImport::OnDiskImport(
    const std::string& db_folder,
    uint64_t strings_buffer_size,
    uint64_t tensors_buffer_size
) :
    catalog("catalog.dat"),
    strings_buffer_size(strings_buffer_size),
    tensors_buffer_size(tensors_buffer_size),
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
        return external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
             | ObjectId::MASK_STRING_SIMPLE;
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
    auto id = get_str_id();

    if ((id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        id1 = id;
        return;
    }

    auto it = node_ids_map.find(id);
    if (it != node_ids_map.end()) {
        id1 = it->second;
    } else {
        id1 = catalog.nodes_count++ | ObjectId::MASK_NODE;
        node_ids_map.insert({ id, id1 });
    }
}

void OnDiskImport::save_first_id_string()
{
    normalize_string_literal();

    auto id = get_str_id();

    if ((id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        id1 = id;
        return;
    }

    auto it = node_ids_map.find(id);
    if (it != node_ids_map.end()) {
        id1 = it->second;
    } else {
        id1 = catalog.nodes_count++ | ObjectId::MASK_NODE;
        node_ids_map.insert({ id, id1 });
    }
}

void OnDiskImport::save_first_id_int()
{
    auto id_int = try_parse_int(lexer.str);

    auto it = node_ids_map.find(id_int);
    if (it != node_ids_map.end()) {
        id1 = it->second;
    } else {
        id1 = catalog.nodes_count++ | ObjectId::MASK_NODE;
        node_ids_map.insert({ id_int, id1 });
    }
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
    auto label_id = get_node_label_id(std::string(lexer.str, lexer.str_len));
    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_labels->push_back({ id1, label_id });
    } else {
        node_labels.push_back({ id1, label_id });
    }
}

void OnDiskImport::add_node_prop_string()
{
    normalize_string_literal();
    auto key_id = get_node_key_id();
    auto value_id = get_str_id();

    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
    {
        pending_node_properties->push_back({ id1, key_id, value_id });
    } else {
        node_properties.push_back({ id1, key_id, value_id });
    }
}

void OnDiskImport::add_node_prop_int()
{
    auto key_id = get_node_key_id();
    uint64_t value_id = try_parse_int(lexer.str);

    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_properties->push_back({ id1, key_id, value_id });
    } else {
        node_properties.push_back({ id1, key_id, value_id });
    }
}

void OnDiskImport::add_node_prop_float()
{
    auto key_id = get_node_key_id();
    uint64_t value_id = try_parse_float(lexer.str);

    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_properties->push_back({ id1, key_id, value_id });
    } else {
        node_properties.push_back({ id1, key_id, value_id });
    }
}

void OnDiskImport::add_node_prop_true()
{
    auto key_id = get_node_key_id();
    uint64_t value_id = Common::Conversions::pack_bool(true).id;

    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_properties->push_back({ id1, key_id, value_id });
    } else {
        node_properties.push_back({ id1, key_id, value_id });
    }
}

void OnDiskImport::add_node_prop_false()
{
    auto key_id = get_node_key_id();
    uint64_t value_id = Common::Conversions::pack_bool(false).id;

    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_properties->push_back({ id1, key_id, value_id });
    } else {
        node_properties.push_back({ id1, key_id, value_id });
    }
}

void OnDiskImport::add_node_prop_datatype()
{
    uint64_t value_id = get_datatype_value_id();

    if (value_id == ObjectId::NULL_ID) {
        return;
    }

    auto key_id = get_node_key_id();
    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_properties->push_back({ id1, key_id, value_id });
    } else {
        node_properties.push_back({ id1, key_id, value_id });
    }
}

// returns ObjectId::NULL_ID if invalid
uint64_t OnDiskImport::get_datatype_value_id()
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

    uint64_t value_id = ObjectId::NULL_ID;

    if (strcmp(datatype_beg, "dateTime") == 0) {
        value_id = DateTime::from_dateTime(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << " invalid dateTime: " << lexer.str << "\n";
        }
    } else if (strcmp(datatype_beg, "date") == 0) {
        value_id = DateTime::from_date(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "invalid date: " << lexer.str << "\n";
        }
    } else if (strcmp(datatype_beg, "time") == 0) {
        value_id = DateTime::from_time(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "invalid time: " << lexer.str << "\n";
        }
    } else if (strcmp(datatype_beg, "dateTimeStamp") == 0) {
        value_id = DateTime::from_dateTimeStamp(lexer.str);
        if (value_id == ObjectId::NULL_ID) {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "invalid dateTimeStamp: " << lexer.str << "\n";
        }
    } else {
        parsing_errors++;
        std::cout << "ERROR on line " << current_line << ", ";
        std::cout << "unknown datatype: " << datatype_beg << "\n";
    }
    return value_id;
}

void OnDiskImport::save_second_id_identifier()
{
    auto id = get_str_id();

    if ((id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        id2 = id;
    } else {
        auto it = node_ids_map.find(id);
        if (it != node_ids_map.end()) {
            id2 = it->second;
        } else {
            id2 = catalog.nodes_count++ | ObjectId::MASK_NODE;
            node_ids_map.insert({ id, id2 });
        }
    }

    save_edge();
}

void OnDiskImport::save_second_id_string()
{
    normalize_string_literal();

    auto id = get_str_id();

    if ((id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        id2 = id;
    } else {
        auto it = node_ids_map.find(id);
        if (it != node_ids_map.end()) {
            id2 = it->second;
        } else {
            id2 = catalog.nodes_count++ | ObjectId::MASK_NODE;
            node_ids_map.insert({ id, id2 });
        }
    }

    save_edge();
}

void OnDiskImport::save_second_id_int()
{
    auto id_int = try_parse_int(lexer.str);

    auto it = node_ids_map.find(id_int);
    if (it != node_ids_map.end()) {
        id2 = it->second;
    } else {
        id2 = catalog.nodes_count++ | ObjectId::MASK_NODE;
        node_ids_map.insert({ id_int, id2 });
    }

    save_edge();
}

void OnDiskImport::save_edge()
{
    if (edge_dir == EdgeDir::UNDIRECTED) {
        edge_id = ObjectId::MASK_UNDIRECTED_EDGE | catalog.undirected_edges_count++;
        try_save_undirected_edge();

    } else {
        edge_id = ObjectId::MASK_DIRECTED_EDGE | catalog.directed_edges_count++;
        try_save_directed_edge();
    }
}

void OnDiskImport::try_save_undirected_edge()
{
    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP || (id2 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_undirected_edges->push_back({ id1, id2, edge_id });
    } else {
        undirected_edges.push_back({ id1, id2, edge_id });
        if (id1 != id2) {
            undirected_edges.push_back({ id2, id1, edge_id });
        } else {
            undirected_equal_edges.push_back({ id1, edge_id });
        }
    }
}

void OnDiskImport::try_save_directed_edge()
{
    if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP || (id2 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_directed_edges->push_back({ id1, id2, edge_id });
    } else {
        if (edge_dir == EdgeDir::RIGHT) {
            directed_edges.push_back({ id1, id2, edge_id });
        } else {
            directed_edges.push_back({ id2, id1, edge_id });
        }

        if (id1 == id2) {
            directed_equal_edges.push_back({ id1, edge_id });
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
    get_datatype_value_id();
    auto key_id = get_node_key_id();
    uint64_t value_id = get_datatype_value_id();
    edge_properties.push_back({ id1, key_id, value_id });
}

void OnDiskImport::add_edge_prop_string()
{
    normalize_string_literal();
    auto key_id = get_edge_key_id();
    auto value_id = get_str_id();

    if ((value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_edge_properties->push_back({ edge_id, key_id, value_id });
    } else {
        edge_properties.push_back({ edge_id, key_id, value_id });
    }
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

void OnDiskImport::try_save_node_label(uint64_t node_id, uint64_t label_id)
{
    if ((node_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
        pending_node_labels->push_back({ node_id, label_id });
    } else {
        node_labels.push_back({ node_id, label_id });
    }
}

void OnDiskImport::try_save_node_property(uint64_t node_id, uint64_t key_id, uint64_t value_id)
{
    if ((node_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
    {
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

void OnDiskImport::start_import(MDBIstream& in)
{
    auto start = std::chrono::system_clock::now();
    auto import_start = start;

    pending_directed_edges = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_DIRECTED_EDGES_FILENAME_PREFIX
    );
    pending_undirected_edges = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_UNDIRECTED_EDGES_FILENAME_PREFIX
    );

    pending_node_labels = std::make_unique<DiskVector<2>>(
        db_folder + "/" + PENDING_NODE_LABELS_FILENAME_PREFIX
    );

    pending_node_properties = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_NODE_PROPERTIES_FILENAME_PREFIX
    );
    pending_edge_properties = std::make_unique<DiskVector<3>>(
        db_folder + "/" + PENDING_EDGE_PROPERTIES_FILENAME_PREFIX
    );

    // Initialize external helper
    external_helper = std::make_unique<ExternalHelper>(db_folder, strings_buffer_size, tensors_buffer_size);

    lexer.begin(in);

    int current_state = State::LINE_BEGIN;
    current_line = 1;
    while (auto token = lexer.get_token()) {
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

    external_helper->flush_to_disk();

    print_duration("Processing strings", start);

    { // process pending files
        pending_directed_edges->finish_appends();
        pending_undirected_edges->finish_appends();
        pending_node_labels->finish_appends();
        pending_node_properties->finish_appends();
        pending_edge_properties->finish_appends();

        int i = 0;
        while (true) {
            const auto total_pending = pending_directed_edges->get_total_tuples()
                                     + pending_undirected_edges->get_total_tuples()
                                     + pending_node_labels->get_total_tuples()
                                     + pending_node_properties->get_total_tuples()
                                     + pending_edge_properties->get_total_tuples();
            if (total_pending == 0) {
                break;
            }
            std::cout << "total pending: " << total_pending << std::endl;

            auto old_directed_pending_edges = std::move(pending_directed_edges);
            auto old_undirected_pending_edges = std::move(pending_undirected_edges);
            auto old_pending_node_labels = std::move(pending_node_labels);
            auto old_pending_node_properties = std::move(pending_node_properties);
            auto old_pending_edge_properties = std::move(pending_edge_properties);

            pending_directed_edges = std::make_unique<DiskVector<3>>(
                db_folder + "/" + PENDING_DIRECTED_EDGES_FILENAME_PREFIX + std::to_string(i)
            );
            pending_undirected_edges = std::make_unique<DiskVector<3>>(
                db_folder + "/" + PENDING_UNDIRECTED_EDGES_FILENAME_PREFIX + std::to_string(i)
            );
            pending_node_labels = std::make_unique<DiskVector<2>>(
                db_folder + "/" + PENDING_NODE_LABELS_FILENAME_PREFIX + std::to_string(i)
            );
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

            old_directed_pending_edges->begin_tuple_iter();
            while (old_directed_pending_edges->has_next_tuple()) {
                const auto& pending_tuple = old_directed_pending_edges->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                id2 = external_helper->resolve_id(pending_tuple[1]);
                edge_id = pending_tuple[2];

                if ((id1 & ObjectId::MOD_MASK) != ObjectId::MOD_TMP
                    && (id1 & ObjectId::TYPE_MASK) != ObjectId::MASK_NODE)
                {
                    auto it = node_ids_map.find(id1);
                    if (it != node_ids_map.end()) {
                        id1 = it->second;
                    } else {
                        auto internal_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                        node_ids_map.insert({ id1, internal_id });
                        id1 = internal_id;
                    }
                }

                if ((id2 & ObjectId::MOD_MASK) != ObjectId::MOD_TMP
                    && (id2 & ObjectId::TYPE_MASK) != ObjectId::MASK_NODE)
                {
                    auto it = node_ids_map.find(id2);
                    if (it != node_ids_map.end()) {
                        id2 = it->second;
                    } else {
                        auto internal_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                        node_ids_map.insert({ id2, internal_id });
                        id2 = internal_id;
                    }
                }

                try_save_directed_edge();
            }

            old_undirected_pending_edges->begin_tuple_iter();
            while (old_undirected_pending_edges->has_next_tuple()) {
                const auto& pending_tuple = old_undirected_pending_edges->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                id2 = external_helper->resolve_id(pending_tuple[1]);
                edge_id = pending_tuple[2];

                if ((id1 & ObjectId::MOD_MASK) != ObjectId::MOD_TMP
                    && (id1 & ObjectId::TYPE_MASK) != ObjectId::MASK_NODE)
                {
                    auto it = node_ids_map.find(id1);
                    if (it != node_ids_map.end()) {
                        id1 = it->second;
                    } else {
                        auto internal_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                        node_ids_map.insert({ id1, internal_id });
                        id1 = internal_id;
                    }
                }

                if ((id2 & ObjectId::MOD_MASK) != ObjectId::MOD_TMP
                    && (id2 & ObjectId::TYPE_MASK) != ObjectId::MASK_NODE)
                {
                    auto it = node_ids_map.find(id2);
                    if (it != node_ids_map.end()) {
                        id2 = it->second;
                    } else {
                        auto internal_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                        node_ids_map.insert({ id2, internal_id });
                        id2 = internal_id;
                    }
                }

                try_save_undirected_edge();
            }

            old_pending_node_labels->begin_tuple_iter();
            while (old_pending_node_labels->has_next_tuple()) {
                const auto& pending_tuple = old_pending_node_labels->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                uint64_t label_id = pending_tuple[1];

                if ((id1 & ObjectId::MOD_MASK) != ObjectId::MOD_TMP) {
                    auto it = node_ids_map.find(id1);
                    if (it != node_ids_map.end()) {
                        id1 = it->second;
                    } else {
                        auto internal_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                        node_ids_map.insert({ id1, internal_id });
                        id1 = internal_id;
                    }
                }

                try_save_node_label(id1, label_id);
            }

            old_pending_node_properties->begin_tuple_iter();
            while (old_pending_node_properties->has_next_tuple()) {
                const auto& pending_tuple = old_pending_node_properties->next_tuple();

                id1 = external_helper->resolve_id(pending_tuple[0]);
                uint64_t key_id = pending_tuple[1];
                uint64_t value_id = external_helper->resolve_id(pending_tuple[2]);

                if ((id1 & ObjectId::MOD_MASK) != ObjectId::MOD_TMP
                    && (id1 & ObjectId::TYPE_MASK) != ObjectId::MASK_NODE)
                {
                    auto it = node_ids_map.find(id1);
                    if (it != node_ids_map.end()) {
                        id1 = it->second;
                    } else {
                        auto internal_id = catalog.nodes_count++ | ObjectId::MASK_NODE;
                        node_ids_map.insert({ id1, internal_id });
                        id1 = internal_id;
                    }
                }

                try_save_node_property(id1, key_id, value_id);
            }

            old_pending_edge_properties->begin_tuple_iter();
            while (old_pending_edge_properties->has_next_tuple()) {
                const auto& pending_tuple = old_pending_edge_properties->next_tuple();

                id1 = pending_tuple[0];
                uint64_t key_id = pending_tuple[1];
                uint64_t value_id = external_helper->resolve_id(pending_tuple[2]);

                try_save_edge_property(id1, key_id, value_id);
            }

            { // write node mapping as properties
                saved_key = "_id";
                auto key_id = get_node_key_id();
                for (auto&& [k, v] : node_ids_map) {
                    node_properties.push_back({ v, key_id, k });
                }
                node_ids_map.clear();
            }

            // write out new data
            external_helper->flush_to_disk();
            // close and delete the old pending files
            external_helper->clean_up_old();

            // close and delete old pending file
            pending_directed_edges->finish_appends();
            pending_undirected_edges->finish_appends();
            pending_node_labels->finish_appends();
            pending_node_properties->finish_appends();
            pending_edge_properties->finish_appends();
            // close and remove file
            old_directed_pending_edges->skip_indexing();
            old_undirected_pending_edges->skip_indexing();
            old_pending_node_labels->skip_indexing();
            old_pending_node_properties->skip_indexing();
            old_pending_edge_properties->skip_indexing();
        }

        // process pending finished, clean up the last pending file
        pending_directed_edges->skip_indexing();
        pending_undirected_edges->skip_indexing();
        pending_node_labels->skip_indexing();
        pending_node_properties->skip_indexing();
        pending_edge_properties->skip_indexing();
    }

    { // write node mapping as properties
        saved_key = "_id";
        auto key_id = get_node_key_id();
        for (auto&& [k, v] : node_ids_map) {
            node_properties.push_back({ v, key_id, k });
        }
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
        DictCountStat<2> label_stat;

        node_labels.create_bpt(db_folder + "/node_label", { COL_NODE, COL_LABEL }, no_stat);

        node_labels.create_bpt(db_folder + "/label_node", { COL_LABEL, COL_NODE }, label_stat);
        catalog.node_labels_count = label_stat.all;
        label_stat.end();

        catalog.node_label2total_count = std::move(label_stat.dict);
    }

    { // Edge Labels B+Tree
        size_t COL_EDGE = 0, COL_LABEL = 1;

        NoStat<2> no_stat;
        DictCountStat<2> label_stat;

        edge_labels.create_bpt(db_folder + "/edge_label", { COL_EDGE, COL_LABEL }, no_stat);

        edge_labels.create_bpt(db_folder + "/label_edge", { COL_LABEL, COL_EDGE }, label_stat);
        catalog.edge_labels_count = label_stat.all;
        label_stat.end();

        catalog.edge_label2total_count = std::move(label_stat.dict);
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
