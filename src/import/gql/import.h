#pragma once

#include <cctype>
#include <functional>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/gql/gql_catalog.h"
#include "import/disk_vector.h"
#include "import/external_string.h"
#include "import/gql/lexer/state.h"
#include "import/gql/lexer/token.h"
#include "import/gql/lexer/tokenizer.h"
#include "misc/istream.h"

namespace Import { namespace GQL {
class OnDiskImport {
    enum class EdgeDir {
        LEFT,
        RIGHT,
        UNDIRECTED
    };

public:
    OnDiskImport(const std::string& db_folder, uint64_t buffer_size);

    ~OnDiskImport();

    void start_import(MDBIstream& in);

private:
    GQLCatalog catalog;

    uint64_t buffer_size;

    std::string db_folder;

    int* state_transitions;

    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];

    GQLTokenizer lexer;

    int current_line;

    uint64_t parsing_errors = 0;

    // used for node id or lhs in edges
    uint64_t id1;

    // used for rhs in edges
    uint64_t id2;

    uint64_t edge_id;

    std::string saved_key;

    EdgeDir edge_dir;

    uint64_t current_node_label = 0;

    uint64_t current_node_key = 0;

    uint64_t current_edge_label = 0;

    uint64_t current_edge_key = 0;

    // { node_id, label_id }
    DiskVector<2> node_labels;

    // { edge_id, label_id }
    DiskVector<2> edge_labels;

    // { node_id, key_id, value_id }
    DiskVector<3> node_properties;

    // { edge_id, key_id, value_id }
    DiskVector<3> edge_properties;

    // { from, to, edge_id }
    DiskVector<3> directed_edges;

    // { node1, node2, edge_id }
    DiskVector<3> undirected_edges;

    // { nodes_id, edge_id }
    DiskVector<2> directed_equal_edges;

    // { nodes_id, edge_id }
    DiskVector<2> undirected_equal_edges;

    // map Id->InternalId
    boost::unordered_flat_map<uint64_t, uint64_t> node_ids_map;

    boost::unordered_flat_map<std::string, uint64_t> node_labels_map;

    boost::unordered_flat_map<std::string, uint64_t> edge_labels_map;

    boost::unordered_flat_map<std::string, uint64_t> node_keys_map;

    boost::unordered_flat_map<std::string, uint64_t> edge_keys_map;

    boost::unordered_flat_set<ExternalString, std::hash<ExternalString>> external_strings_set;

    char* external_strings;

    uint64_t external_strings_capacity;

    uint64_t external_strings_end;

    void do_nothing() { }

    void save_direction(EdgeDir);

    void save_first_id_identifier();

    void save_first_id_string();

    void save_first_id_int();

    void save_prop_key();

    void add_node_label();

    void add_node_prop_string();

    void add_node_prop_int();

    void add_node_prop_float();

    void add_node_prop_true();

    void add_node_prop_false();

    void add_node_prop_datatype();

    void save_second_id_identifier();

    void save_second_id_string();

    void save_second_id_int();

    void add_edge_label();

    void add_edge_prop_datatype();

    void add_edge_prop_string();

    void add_edge_prop_int();

    void add_edge_prop_float();

    void add_edge_prop_true();

    void add_edge_prop_false();

    void finish_line();

    void print_error();

    void add_prop_datatype(uint64_t obj_id, DiskVector<3>& obj_properties);

    void create_automata();

    void set_transition(int state, int token, int value, std::function<void()> func);

    int get_transition(int state, int token);

    // modifies contents of lexer.str and lexer.str_len. lexer.str points to the same place
    void normalize_string_literal();

    uint64_t get_or_create_external_string_id();

    // uses string from lexer.str and lexer.str_len
    uint64_t get_str_id();

    uint64_t get_node_id(uint64_t id);

    uint64_t get_node_label_id(const std::string& label);

    uint64_t get_edge_label_id(const std::string& label);

    uint64_t get_node_key_id();

    uint64_t get_edge_key_id();

    // sets edge_id and push into undirected_edges or directed_edges
    void save_edge();
};
}} // namespace Import::GQL
