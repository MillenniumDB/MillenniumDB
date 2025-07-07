#pragma once

#include <functional>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/gql/gql_catalog.h"
#include "import/disk_vector.h"
#include "import/gql/lexer/state.h"
#include "import/gql/lexer/token.h"
#include "import/gql/lexer/tokenizer.h"
#include "import/external_helper.h"
#include "misc/istream.h"

namespace Import { namespace GQL {
class OnDiskImport {
    enum class EdgeDir {
        LEFT,
        RIGHT,
        UNDIRECTED
    };

public:
    static constexpr char PENDING_NODE_LABELS_FILENAME_PREFIX[] = "tmp_pending_node_labels";
    static constexpr char PENDING_NODE_PROPERTIES_FILENAME_PREFIX[] = "tmp_pending_node_properties";
    static constexpr char PENDING_EDGE_PROPERTIES_FILENAME_PREFIX[] = "tmp_pending_edge_properties";
    static constexpr char PENDING_DIRECTED_EDGES_FILENAME_PREFIX[] = "tmp_pending_directed_edges";
    static constexpr char PENDING_UNDIRECTED_EDGES_FILENAME_PREFIX[] = "tmp_pending_undirected_edges";

    OnDiskImport(const std::string& db_folder, uint64_t strings_buffer_size, uint64_t tensors_buffer_size);

    ~OnDiskImport();

    void start_import(MDBIstream& in);

private:
    GQLCatalog catalog;

    uint64_t strings_buffer_size;
    uint64_t tensors_buffer_size;

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

    std::unique_ptr<DiskVector<2>> pending_node_labels;
    std::unique_ptr<DiskVector<3>> pending_node_properties;
    std::unique_ptr<DiskVector<3>> pending_edge_properties;
    std::unique_ptr<DiskVector<3>> pending_directed_edges;
    std::unique_ptr<DiskVector<3>> pending_undirected_edges;

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

    // manager writing bytes to disk in a buffered manner
    std::unique_ptr<ExternalHelper> external_helper;

    // map Id->InternalId
    boost::unordered_flat_map<uint64_t, uint64_t> node_ids_map;

    boost::unordered_flat_map<std::string, uint64_t> node_labels_map;

    boost::unordered_flat_map<std::string, uint64_t> edge_labels_map;

    boost::unordered_flat_map<std::string, uint64_t> node_keys_map;

    boost::unordered_flat_map<std::string, uint64_t> edge_keys_map;

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

    void try_save_node_label(uint64_t node, uint64_t label);
    void try_save_node_property(uint64_t node, uint64_t key, uint64_t value);
    void try_save_edge_property(uint64_t node, uint64_t key, uint64_t value);
    void try_save_undirected_edge();
    void try_save_directed_edge();

    // returns ObjectId::NULL_ID if invalid
    uint64_t get_datatype_value_id();

    void create_automata();

    void set_transition(int state, int token, int value, std::function<void()> func);

    int get_transition(int state, int token);

    // modifies contents of lexer.str and lexer.str_len. lexer.str points to the same place
    void normalize_string_literal();

    // uses string from lexer.str and lexer.str_len
    uint64_t get_str_id();

    uint64_t get_node_label_id(const std::string& label);

    uint64_t get_edge_label_id(const std::string& label);

    uint64_t get_node_key_id();

    uint64_t get_edge_key_id();

    // sets edge_id and push into undirected_edges or directed_edges
    void save_edge();

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
}} // namespace Import::GQL
