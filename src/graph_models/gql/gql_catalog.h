#pragma once

#include <ostream>
#include <string>

#include "storage/catalog/catalog.h"

class GQLCatalog : public Catalog {
public:
    static constexpr uint64_t MODEL_ID = 2;
    static constexpr uint64_t VERSION = 1;

    GQLCatalog(const std::string& filename);

    ~GQLCatalog();

    void print(std::ostream&);

    void save();

    std::vector<std::string> convert_map_to_vec(boost::unordered_map<uint64_t, uint64_t> map);

    bool has_changes = false;

    uint64_t nodes_count = 0;

    uint64_t directed_edges_count = 0;

    uint64_t undirected_edges_count = 0;

    uint64_t node_labels_count = 0;

    uint64_t edge_labels_count = 0;

    uint64_t node_properties_count = 0;

    uint64_t edge_properties_count = 0;

    uint64_t equal_directed_edges_count = 0;

    uint64_t equal_undirected_edges_count = 0;

    boost::unordered_map<uint64_t, uint64_t> node_label2total_count;

    boost::unordered_map<uint64_t, uint64_t> edge_label2total_count;

    boost::unordered_map<uint64_t, uint64_t> node_key2total_count;

    boost::unordered_map<uint64_t, uint64_t> edge_key2total_count;

    std::vector<std::string> node_labels_str;
    boost::unordered_map<std::string, uint64_t> node_labels2id;

    std::vector<std::string> edge_labels_str;
    boost::unordered_map<std::string, uint64_t> edge_labels2id;

    std::vector<std::string> node_keys_str;
    boost::unordered_map<std::string, uint64_t> node_keys2id;

    std::vector<std::string> edge_keys_str;
    boost::unordered_map<std::string, uint64_t> edge_keys2id;
};
