#pragma once

#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/common/conversions.h"
#include "graph_models/object_id.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "import/disk_vector.h"
#include "import/quad_model/csv/aux_structs.h"
#include "import/quad_model/csv/lexer/state.h"
#include "import/quad_model/csv/lexer/token.h"
#include "import/quad_model/csv/lexer/tokenizer.h"
#include "import/external_helper.h"

namespace Import { namespace QuadModel { namespace CSV {

class OnDiskImport {
public:
    static constexpr char PENDING_DECLARED_NODES_FILENAME_PREFIX[] = "tmp_pending_declared_nodes";
    static constexpr char PENDING_LABELS_FILENAME_PREFIX[] = "tmp_pending_labels";
    static constexpr char PENDING_PROPERTIES_FILENAME_PREFIX[] = "tmp_pending_properties";
    static constexpr char PENDING_EDGES_FILENAME_PREFIX[] = "tmp_pending_edges";

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

    void print_error_msg(const std::string& msg);

    void print_fatal_error_msg(const std::string& msg);

    std::vector<std::string> split(const std::string& input, const std::string& delimiter);

private:
    uint64_t strings_buffer_size;
    uint64_t tensors_buffer_size;

    std::string db_folder;

    CSVTokenizer lexer;
    int current_line = 1;
    int current_anon_id = 0;
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

    boost::unordered_flat_map<std::string, uint64_t> csvid_global;
    uint64_t group_count = 0;
    std::vector<boost::unordered_flat_map<std::string, uint64_t>> csvid_groups;
    boost::unordered_flat_map<std::string, uint64_t> csvid_groups_index;

    std::unique_ptr<DiskVector<1>> pending_declared_nodes;
    std::unique_ptr<DiskVector<2>> pending_labels;
    std::unique_ptr<DiskVector<3>> pending_properties;
    std::unique_ptr<DiskVector<4>> pending_edges;

    DiskVector<1> declared_nodes;
    DiskVector<2> labels;
    DiskVector<3> properties;
    DiskVector<4> edges;
    DiskVector<3> equal_from_to;
    DiskVector<3> equal_from_type;
    DiskVector<3> equal_to_type;
    DiskVector<2> equal_from_to_type;

    // manager writing bytes to disk in a buffered manner
    std::unique_ptr<ExternalHelper> external_helper;

    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];

    int* state_transitions;

    void save_headers(std::vector<std::unique_ptr<MDBIstreamFile>>& files);
    void parse_node_files(std::vector<std::unique_ptr<MDBIstreamFile>>& in_nodes);
    void parse_edge_files(std::vector<std::unique_ptr<MDBIstreamFile>>& in_edges);

    void create_automata();

    void reset_automata();

    void set_transition(int state, int token, int value, std::function<void()> func);

    int get_transition(int state, int token);

    void save_header_column();

    void verify_anon();

    void verify_edge_file_header();

    void save_body_column_to_buffer();

    void save_empty_body_column_to_buffer();

    void process_node_line();

    void save_edge_line();

    void go_to_next_line();

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

    void try_save_declared_node(uint64_t node_id);

    void try_save_label(uint64_t node_id, uint64_t label_id);

    void try_save_property(uint64_t id1, uint64_t key_id, uint64_t value_id);

    void try_save_quad(uint64_t from_id, uint64_t to_id, uint64_t type_id, uint64_t edge_id);
};

}}} // namespace Import::QuadModel::CSV
