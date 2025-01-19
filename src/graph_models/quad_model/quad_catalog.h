#pragma once

#include <ostream>
#include <string>

#include <boost/unordered_map.hpp>

#include "storage/catalog/catalog.h"
#include "storage/index/text_search/text_search_index_manager.h"

class QuadCatalog : public Catalog {
public:
    static constexpr uint64_t MODEL_ID = 0;
    static constexpr uint64_t VERSION = 4;

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

    bool has_changes = false;

    uint64_t identifiable_nodes_count; // Does not consider the literals
    uint64_t anonymous_nodes_count; // TODO: delete?

    uint64_t edge_count;
    uint64_t label_count;
    uint64_t properties_count;

    uint64_t equal_from_to_count;
    uint64_t equal_from_type_count;
    uint64_t equal_to_type_count;
    uint64_t equal_from_to_type_count;

    boost::unordered_map<uint64_t, uint64_t> label2total_count;
    boost::unordered_map<uint64_t, uint64_t> key2total_count;
    boost::unordered_map<uint64_t, uint64_t> type2total_count;

    boost::unordered_map<uint64_t, uint64_t> type2equal_from_to_type_count;
    boost::unordered_map<uint64_t, uint64_t> type2equal_from_to_count;
    boost::unordered_map<uint64_t, uint64_t> type2equal_from_type_count;
    boost::unordered_map<uint64_t, uint64_t> type2equal_to_type_count;

    TextSearch::TextSearchIndexManager text_search_index_manager;
};
