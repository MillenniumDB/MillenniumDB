#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "storage/catalog/catalog.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "third_party/robin_hood/robin_hood.h"

class QuadCatalog : public Catalog {
friend class QuadModel;
friend class BulkImport;
public:
    static constexpr uint64_t MODEL_ID = 0;
    static constexpr uint64_t VERSION = 1;

    QuadCatalog(const std::string& filename);

    ~QuadCatalog();

    bool has_changes = false;

    void print(std::ostream&);
    void save();

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

    // TODO: This should be moved as it is not persistent. It is only used for a single query.
    //       It also should be cleared after the query or after reaching a certain amount of
    //       loaded stores / memory usage.
    robin_hood::unordered_map<std::string, std::unique_ptr<TensorStore>> name2tensor_store;
};
