#ifndef RELATIONAL_MODEL__QUAD_BULK_IMPORT_H_
#define RELATIONAL_MODEL__QUAD_BULK_IMPORT_H_

#include <string>
#include <fstream>
#include <list>
#include <memory>
#include <unordered_set>

#include "base/parser/grammar/import/import_ast.h"
#include "relational_model/models/quad_model/quad_model.h"
#include "relational_model/models/quad_model/quad_catalog.h"
#include "storage/index/object_file/object_file.h"
#include "storage/index/ordered_file/ordered_file.h"
#include "storage/index/hash/object_file_hash/object_file_hash.h"

class BulkImport : public boost::static_visitor<uint64_t> {
public:
    BulkImport(const std::string& filename, QuadModel& model);
    ~BulkImport() = default;

    void start_import();

    uint64_t operator()(const std::string&);
    uint64_t operator()(bool);
    uint64_t operator()(int64_t);
    uint64_t operator()(float);

private:
    std::ifstream import_file;
    QuadModel& model;
    QuadCatalog& catalog;

    OrderedFile<1> nodes_ordered_file;              // (node_id)
    OrderedFile<2> labels_ordered_file;             // (node_id, label_id)
    OrderedFile<3> properties_ordered_file;         // (object_id, key_id, value_id)
    OrderedFile<4> connections_ordered_file;        // (from_id, to_id, type_id, edge_id)

    // To create indexes for special cases
    OrderedFile<3> equal_from_to_ordered_file;      // (from=to, type, edge)
    OrderedFile<3> equal_from_type_ordered_file;    // (from=type, to, edge)
    OrderedFile<3> equal_to_type_ordered_file;      // (to=type, from, edge)
    OrderedFile<2> equal_from_to_type_ordered_file; // (from=to=type,  edge)

    std::unordered_set<uint64_t> named_node_ids;
    // std::unordered_set<uint64_t> inlined_ids;
    // std::unordered_set<uint64_t> external_ids;

    uint64_t process_node(const import::ast::Node& node);
    uint64_t process_edge(const import::ast::Edge& edge);
    uint64_t process_implicit_edge(const import::ast::ImplicitEdge& implicit_edge, const uint64_t implicit_object_id);

    uint64_t create_connection(const uint64_t from_id, const uint64_t to_id, const uint64_t type_id);

    uint64_t get_node_id(const boost::variant<std::string, bool, int64_t, float>& node_id);

    template <std::size_t N>
    void set_distinct_type_stats(OrderedFile<N>& ordered_file, std::map<uint64_t, uint64_t>& m);

    void index_labels();
    void index_properties();
    void index_connections();
    void index_special_cases();
};

#endif // RELATIONAL_MODEL__QUAD_BULK_IMPORT_H_
