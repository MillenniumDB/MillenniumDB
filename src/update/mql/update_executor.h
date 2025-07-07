#pragma once

#include <map>

#include <boost/unordered/unordered_flat_map.hpp>

#include "query/parser/op/mql/op.h"
#include "storage/index/hnsw/hnsw_index_update_data.h"
#include "storage/index/text_search/text_index_update_data.h"

namespace MQL {

class UpdateExecutor : public OpVisitor {
public:
    struct GraphUpdateData {
        uint_fast32_t new_nodes { 0 };
        uint_fast32_t new_edges { 0 };
        uint_fast32_t new_labels { 0 };
        uint_fast32_t new_properties { 0 };
        uint_fast32_t overwritten_properties { 0 };

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

    // TODO: OpDelete?
    void visit(OpInsert&) override;
    void visit(OpCreateHNSWIndex&) override;
    void visit(OpCreateTextIndex&) override;

    void execute(Op& op);
    void print_stats(std::ostream& os);

private:
    std::map<ObjectId, ObjectId> created_ids;

    GraphUpdateData graph_update_data;

    boost::unordered_flat_map<std::string, TextIndexUpdateData> name2text_index_update_data;
    boost::unordered_flat_map<std::string, HNSWIndexUpdateData> name2hnsw_index_update_data;

    // returns true if oid was transformed
    bool transform_if_tmp(ObjectId& oid);

    void insert_text_index_update_data(TextIndexUpdateData&& text_index_update_data);
    void insert_hnsw_index_update_data(HNSWIndexUpdateData&& hnsw_index_update_data);
};
} // namespace MQL
