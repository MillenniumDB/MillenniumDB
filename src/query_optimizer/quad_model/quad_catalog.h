#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "base/ids/object_id.h"
#include "storage/catalog/catalog.h"
#include "third_party/robin_hood/robin_hood.h"

class QuadCatalog : public Catalog {
friend class QuadModel;
friend class BulkImport;
public:
    QuadCatalog(const std::string& filename);

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

    robin_hood::unordered_map<uint64_t, uint64_t> label2total_count;
    robin_hood::unordered_map<uint64_t, uint64_t> key2total_count;
    robin_hood::unordered_map<uint64_t, uint64_t> key2distinct;
    robin_hood::unordered_map<uint64_t, uint64_t> type2total_count;

    robin_hood::unordered_map<uint64_t, uint64_t> type2equal_from_to_type_count;
    robin_hood::unordered_map<uint64_t, uint64_t> type2equal_from_to_count;
    robin_hood::unordered_map<uint64_t, uint64_t> type2equal_from_type_count;
    robin_hood::unordered_map<uint64_t, uint64_t> type2equal_to_type_count;
};
