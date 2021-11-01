#ifndef RELATIONAL_MODEL__QUAD_CATALOG_H_
#define RELATIONAL_MODEL__QUAD_CATALOG_H_

#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <vector>

#include "base/ids/object_id.h"
#include "storage/catalog/catalog.h"

class QuadCatalog : public Catalog {
friend class QuadModel;
friend class BulkImport;
public:
    QuadCatalog(const std::string& filename);
    ~QuadCatalog() = default;

    void print();
    void save_changes();

    uint64_t connections_with_type        (uint64_t type_id);
    uint64_t equal_from_to_type_with_type (uint64_t type_id);
    uint64_t equal_from_to_with_type      (uint64_t type_id);
    uint64_t equal_from_type_with_type    (uint64_t type_id);
    uint64_t equal_to_type_with_type      (uint64_t type_id);

// private:
    uint64_t identifiable_nodes_count; // Does not consider the literals
    uint64_t anonymous_nodes_count;
    uint64_t connections_count;

    uint64_t label_count;
    // type_count not needed because it's equal to connections_count
    uint64_t properties_count;

    uint64_t distinct_labels;
    uint64_t distinct_keys;

    uint64_t distinct_from;
    uint64_t distinct_to;
    uint64_t distinct_type;

    uint64_t equal_from_to_count;
    uint64_t equal_from_type_count;
    uint64_t equal_to_type_count;
    uint64_t equal_from_to_type_count;

    std::map<uint64_t, uint64_t> label2total_count;
    std::map<uint64_t, uint64_t> key2total_count;
    std::map<uint64_t, uint64_t> key2distinct;
    std::map<uint64_t, uint64_t> type2total_count;

    std::map<uint64_t, uint64_t> type2equal_from_to_type_count;
    std::map<uint64_t, uint64_t> type2equal_from_to_count;
    std::map<uint64_t, uint64_t> type2equal_from_type_count;
    std::map<uint64_t, uint64_t> type2equal_to_type_count;
};

#endif // RELATIONAL_MODEL__QUAD_CATALOG_H_
