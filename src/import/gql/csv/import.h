#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/common/conversions.h"
#include "graph_models/gql/gql_catalog.h"
#include "import/disk_vector.h"
#include "import/gql/csv/lexer/state.h"
#include "import/gql/csv/lexer/token.h"
#include "import/gql/csv/lexer/tokenizer.h"
#include "import/external_helper.h"
#include "misc/istream.h"

namespace Import { namespace GQL { namespace CSV {
enum CSVType {
    UNDEFINED,
    ID,
    START_ID,
    END_ID,
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
    char* value_str;
    size_t value_size;

    CSVColumn(CSVType type, std::string name) :
        type(type),
        name(name)
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
        // this->key_id = other.key_id;
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
    static constexpr char PENDING_NODE_PROPERTIES_FILENAME_PREFIX[] = "tmp_pending_node_properties";
    static constexpr char PENDING_EDGE_PROPERTIES_FILENAME_PREFIX[] = "tmp_pending_edge_properties";

    OnDiskImport(
        const std::string& db_folder,
        uint64_t string_buffer_size,
        uint64_t tensor_buffer_size,
        char list_separator
    );

    ~OnDiskImport();

    void start_import(
        std::vector<std::unique_ptr<MDBIstreamFile>>& in_nodes,
        std::vector<std::unique_ptr<MDBIstreamFile>>& in_edges
    );

    void print_error();

    void print_error_msg(std::string msg);

    void print_fatal_error_msg(std::string msg);

    std::vector<std::string> split(std::string input, std::string delimiter);

private:
    void parse_node_files(std::vector<std::unique_ptr<MDBIstreamFile>>& in_nodes);
    void parse_edge_files(std::vector<std::unique_ptr<MDBIstreamFile>>& in_edges);

    uint64_t strings_buffer_size;
    uint64_t tensors_buffer_size;
    std::string db_folder;

    CSVTokenizer lexer;
    int current_line = 1;
    uint64_t parsing_errors = 0;
    int current_token;
    int current_state;

    std::string label_splitter;

    bool anonymous_nodes = true;
    bool global_ids = true;
    bool has_direction;
    bool has_set_first_undirected = false;

    int current_id = 0;
    int current_node_label = 0;
    int current_node_key = 0;
    int current_edge_label = 0;
    int current_edge_key = 0;
    std::string current_group;
    int64_t current_group_idx;
    std::string current_group_from;
    int64_t current_group_idx_from;
    std::string current_group_to;
    int64_t current_group_idx_to;
    int column_with_id;
    int column_with_id_from;
    int column_with_id_to;
    std::vector<CSVColumn> columns;
    int current_column = 0;

    GQLCatalog catalog;

    boost::unordered_flat_map<std::string, uint64_t> csvid_global;
    uint64_t group_count = 0;
    std::vector<boost::unordered_flat_map<std::string, uint64_t>> csvid_groups;
    boost::unordered_flat_map<std::string, uint64_t> csvid_groups_index;

    boost::unordered_flat_map<std::string, uint64_t> node_labels_map;
    boost::unordered_flat_map<std::string, uint64_t> edge_labels_map;
    boost::unordered_flat_map<std::string, uint64_t> node_keys_map;
    boost::unordered_flat_map<std::string, uint64_t> edge_keys_map;

    std::unique_ptr<DiskVector<3>> pending_node_properties;
    std::unique_ptr<DiskVector<3>> pending_edge_properties;

    DiskVector<2> node_labels; // { node_id, label_id }
    DiskVector<2> edge_labels; // { edge_id, label_id }
    DiskVector<3> node_properties; // { node_id, key_id, value_id }
    DiskVector<3> edge_properties; // { edge_id, key_id, value_id }
    DiskVector<3> directed_edges; // { from, to, edge_id }
    DiskVector<3> undirected_edges; // { node1, node2, edge_id}
    DiskVector<2> directed_equal_edges; // { nodes_id, edge_id }
    DiskVector<2> undirected_equal_edges; // { nodes_id, edge_id }

    // manager writing bytes to disk in a buffered manner
    std::unique_ptr<ExternalHelper> external_helper;

    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];

    int* state_transitions;

    void try_save_node_property(uint64_t node, uint64_t key, uint64_t value);
    void try_save_edge_property(uint64_t node, uint64_t key, uint64_t value);

    void create_automata();

    void reset_automata();

    void set_transition(int state, int token, int value, std::function<void()> func);

    int get_transition(int state, int token);

    void do_nothing() { }

    void save_header_column();

    void verify_anon();

    void verify_edge_file_header();

    void save_body_column_to_buffer();

    void save_empty_body_column_to_buffer();

    void process_node_line();

    void normalize_string_literal(CSVColumn& col);

    void save_edge_line();

    void go_to_next_line();

    uint64_t get_node_key_id(const std::string& column_name);

    uint64_t get_edge_key_id(const std::string& column_name);

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

    // processes a pending file by iterations, until no more pending tuples are available
    // the size of the tuples and a resolve+save function must be provided.
    // pending_vector is passed-by-reference in order to keep it valid when calling the
    // resolve_and_save_func
    template<std::size_t N, typename ResolveAndSaveFunc>
    inline void process_pending(
        std::unique_ptr<DiskVector<N>>& pending_vector,
        const std::string& name,
        const std::string& pending_filename_prefix,
        ResolveAndSaveFunc resolve_and_save_func
    );
};

}}} // namespace Import::GQL::CSV
