#pragma once

#include "boost/unordered/unordered_flat_map.hpp"
#include <boost/unordered/unordered_flat_set.hpp>
#include <cctype>

#include "graph_models/common/conversions.h"
#include "graph_models/inliner.h"
#include "graph_models/object_id.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "import/disk_vector.h"
#include "import/external_string.h"
#include "import/quad_model/csv/lexer/state.h"
#include "import/quad_model/csv/lexer/token.h"
#include "import/quad_model/csv/lexer/tokenizer.h"

namespace Import { namespace QuadModel { namespace CSV {
enum CSVType {
    UNDEFINED,
    ID,
    START_ID,
    END_ID,
    TYPE,
    LABEL,
    STR,
    INT,
    DECIMAL,
    DATE,
    DATETIME
};

struct CSVColumn {
    CSVType type;
    std::string name;
    uint64_t key_id;
    char* value_str;
    size_t value_size;

    CSVColumn(CSVType type, std::string name, uint64_t key_id) :
        type(type),
        name(name),
        key_id(key_id)
    {
        value_str = new char[64 * 1024 * 1024];
        value_size = 0;
    }

    // Not allowed to generate copies
    CSVColumn(const CSVColumn& other) = delete;

    CSVColumn(CSVColumn&& other)
    {
        this->value_str = other.value_str;
        other.value_str = nullptr;
        this->type = other.type;
        this->name = other.name;
        this->key_id = other.key_id;
        this->value_size = other.value_size;
    }

    ~CSVColumn()
    {
        if (value_str)
            delete[] value_str;
    }
};

class OnDiskImport {
public:
    OnDiskImport(const std::string& db_folder, uint64_t buffer_size);

    ~OnDiskImport();

    void start_import(
        std::vector<std::unique_ptr<MDBIstreamFiles>>& in_nodes,
        std::vector<std::unique_ptr<MDBIstreamFiles>>& in_edges
    );

    void print_error();

    void print_error_msg(const std::string& msg);

    void print_fatal_error_msg(const std::string& msg);

    std::vector<std::string> split(const std::string& input, const std::string& delimiter);

private:
    void parse_node_files(std::vector<std::unique_ptr<MDBIstreamFiles>>& in_nodes);
    void parse_edge_files(std::vector<std::unique_ptr<MDBIstreamFiles>>& in_edges);

    uint64_t buffer_size;
    std::string db_folder;

    CSVTokenizer lexer;
    int current_line = 1;
    int current_id = 0;
    int current_state = State::START_HEADER_NODES;
    int current_token;
    uint64_t edge_count = 0;
    uint64_t parsing_errors = 0;

    std::string label_splitter;

    bool anonymous_nodes = true;
    bool global_ids = true;

    std::string current_group;
    uint64_t current_group_idx;
    std::string current_group_from;
    int64_t current_group_idx_from;
    std::string current_group_to;
    int64_t current_group_idx_to;
    int column_with_id;
    int column_with_id_from;
    int column_with_id_to;
    int column_with_type;
    std::vector<CSVColumn> columns;
    int current_column = 0;

    QuadCatalog catalog;
    char* external_strings;
    boost::unordered_flat_set<ExternalString, std::hash<ExternalString>> external_strings_set;
    uint64_t external_strings_capacity;
    uint64_t external_strings_end;

    boost::unordered_flat_map<std::string, uint64_t> csvid_global;
    uint64_t group_count = 0;
    std::vector<boost::unordered_flat_map<std::string, uint64_t>> csvid_groups;
    boost::unordered_flat_map<std::string, uint64_t> csvid_groups_index;

    DiskVector<1> declared_nodes;
    DiskVector<2> labels;
    DiskVector<3> properties;
    DiskVector<4> edges;
    DiskVector<3> equal_from_to;
    DiskVector<3> equal_from_type;
    DiskVector<3> equal_to_type;
    DiskVector<2> equal_from_to_type;

    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];

    int* state_transitions;

    void create_automata();

    void reset_automata();

    void set_transition(int state, int token, int value, std::function<void()> func);

    int get_transition(int state, int token);

    void save_header_column();

    void verify_anon();

    void verify_edge_file_header();

    void save_body_column_to_buffer();

    void save_empty_body_column_to_buffer();

    void save_node_to_disk();

    void save_edge_to_disk();

    void go_to_next_line();

    uint64_t get_or_create_external_string_id(const std::string& input);

    void normalize_string_literal(CSVColumn& col);

    void do_nothing() { }

    void move_to_next_column()
    {
        current_column++;
    }

    int64_t try_parse_int(char* num_str)
    {
        return Common::Conversions::pack_int(atoll(num_str)).id;
    }

    int64_t try_parse_float(char* num_str)
    {
        return Common::Conversions::pack_float(atof(num_str)).id;
    }
};

}}} // namespace Import::QuadModel::CSV