#pragma once

#include <map>

#include <boost/unordered/unordered_flat_map.hpp>

#include "graph_models/object_id.h"
#include "query/update/mql/update_action.h"
#include "query/update/hnsw_index_update_stats.h"
#include "query/update/text_index_update_stats.h"

namespace MQL {

class UpdateExecutor {
public:
    struct GraphUpdateData {
        uint_fast32_t new_nodes = 0;
        uint_fast32_t new_edges = 0;
        uint_fast32_t new_labels = 0;
        uint_fast32_t new_properties = 0;
        uint_fast32_t overwritten_properties = 0;

        bool empty() const
        {
            return new_nodes == 0 && new_edges == 0 && new_labels == 0 && new_properties == 0
                && overwritten_properties == 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const GraphUpdateData& data)
        {
            os << "{\"new_nodes\": " << data.new_nodes;
            os << ", \"new_edges\": " << data.new_edges;
            os << ", \"new_labels\": " << data.new_labels;
            os << ", \"new_properties\": " << data.new_properties;
            os << ", \"overwritten_properties\": " << data.overwritten_properties;
            os << "}";
            return os;
        }
    };

    ~UpdateExecutor();

    void execute(const UpdateContext& update_context);
    void update_indexes();
    void print_stats(std::ostream& os);

private:
    GraphUpdateData graph_update_data;

    boost::unordered_flat_map<std::string, TextIndexUpdateStats> name2text_index_update_data;
    boost::unordered_flat_map<std::string, HNSWIndexUpdateStats> name2hnsw_index_update_data;

    // returns a non-tmp ObjectId
    ObjectId transform_if_tmp(ObjectId oid);

    void process_new_property(ObjectId obj, ObjectId key, ObjectId val);
    void process_deleted_property(ObjectId obj, ObjectId key, ObjectId val);

    void insert_text_index_update_data(TextIndexUpdateStats&& text_index_update_data);
    void insert_hnsw_index_update_data(HNSWIndexUpdateStats&& hnsw_index_update_data);
};
} // namespace MQL
