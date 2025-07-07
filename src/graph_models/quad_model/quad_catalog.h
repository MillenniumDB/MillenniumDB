#pragma once

#include <ostream>
#include <string>

#include "storage/catalog/catalog.h"
#include "storage/index/text_search/text_index_manager.h"
#include "storage/index/hnsw/hnsw_index_manager.h"

class QuadCatalog : public Catalog {
public:
    static constexpr uint8_t MODEL_ID = 0;
    static constexpr uint8_t MAJOR_VERSION = 1;
    static constexpr uint8_t MINOR_VERSION = 0;

    QuadCatalog(const std::string& filename);

    ~QuadCatalog();

    void print(std::ostream&);
    void save();

    uint64_t connections_with_type(uint64_t type_id) const;
    uint64_t equal_from_to_type_with_type(uint64_t type_id) const;
    uint64_t equal_from_to_with_type(uint64_t type_id) const;
    uint64_t equal_from_type_with_type(uint64_t type_id) const;
    uint64_t equal_to_type_with_type(uint64_t type_id) const;

    uint64_t insert_new_edge(uint64_t from, uint64_t to, uint64_t type);
    void insert_property(uint64_t key);
    void insert_label(uint64_t label);

    uint64_t nodes_count;

    uint64_t edge_count;
    uint64_t label_count;
    uint64_t properties_count;

    uint64_t equal_from_to_count;
    uint64_t equal_from_type_count;
    uint64_t equal_to_type_count;
    uint64_t equal_from_to_type_count;

    boost::unordered_flat_map<uint64_t, uint64_t> label2total_count;
    boost::unordered_flat_map<uint64_t, uint64_t> key2total_count;
    boost::unordered_flat_map<uint64_t, uint64_t> type2total_count;

    boost::unordered_flat_map<uint64_t, uint64_t> type2equal_from_to_type_count;
    boost::unordered_flat_map<uint64_t, uint64_t> type2equal_from_to_count;
    boost::unordered_flat_map<uint64_t, uint64_t> type2equal_from_type_count;
    boost::unordered_flat_map<uint64_t, uint64_t> type2equal_to_type_count;

    TextSearch::TextIndexManager text_index_manager;
    HNSW::HNSWIndexManager hnsw_index_manager;
};
