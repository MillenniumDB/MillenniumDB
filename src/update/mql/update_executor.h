#pragma once

#include <map>

#include <boost/unordered_map.hpp>

#include "query/parser/op/op.h"
#include "storage/index/tensor_store/tensor_store_update_data.h"
#include "storage/index/text_search/text_search_index_update_data.h"

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

    void visit(OpInsert&) override;
    void visit(OpInsertTensors&) override;
    void visit(OpDeleteTensors&) override;
    void visit(OpCreateTensorStore&) override;
    void visit(OpCreateTextSearchIndex&) override;

    void execute(Op& op);
    void print_stats(std::ostream& os);

private:
    std::map<ObjectId, ObjectId> created_ids;

    GraphUpdateData graph_update_data;

    boost::unordered_map<std::string, TextSearchIndexUpdateData> name2text_search_index_update_data;

    boost::unordered_map<std::string, TensorStoreUpdateData> name2tensor_store_update_data;

    // returns true if oid was transformed
    bool transform_if_tmp(ObjectId& oid);

    void insert_text_search_index_update_data(TextSearchIndexUpdateData&& text_search_index_update_data);
    void insert_tensor_store_update_data(TensorStoreUpdateData&& tensor_store_update_data);
};
} // namespace MQL
