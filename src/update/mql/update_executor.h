#pragma once

#include <map>

#include "query/parser/op/op.h"
#include "storage/index/text_search/text_search_index_update_data.h"

namespace MQL {

class UpdateExecutor : public OpVisitor {
public:
    struct TensorUpdateData {
        std::string tensor_store_name {};
        bool created { false };
        uint64_t new_entries { 0 };
        uint64_t overwritten_entries { 0 };
        uint64_t deleted_entries { 0 };

        bool empty() const
        {
            return tensor_store_name.empty();
        }

        friend std::ostream& operator<<(std::ostream& os, const TensorUpdateData& data)
        {
            if (data.created) {
                os << "Created TensorStore \"" << data.tensor_store_name << "\"\n";
                return os;
            }

            if (data.new_entries > 0) {
                os << "Inserted " << data.new_entries << " new entries into TensorStore \""
                   << data.tensor_store_name << "\"\n";
            }
            if (data.overwritten_entries > 0) {
                os << "Overwritten " << data.overwritten_entries << " entries in TensorStore \""
                   << data.tensor_store_name << "\"\n";
            }
            if (data.deleted_entries > 0) {
                os << "Deleted " << data.deleted_entries << " entries from TensorStore \""
                   << data.tensor_store_name << "\"\n";
            }
            return os;
        }
    };

    struct GraphUpdateData {
        uint64_t new_edges = 0;
        uint64_t new_labels = 0;
        uint64_t new_properties = 0;
        uint64_t new_nodes = 0;

        bool empty() const
        {
            return new_edges == 0 && new_labels == 0 && new_properties == 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const GraphUpdateData& data)
        {
            if (data.new_nodes > 0) {
                os << "Inserted " << data.new_nodes << " new nodes\n";
            }

            if (data.new_edges > 0) {
                os << "Inserted " << data.new_edges << " new edges\n";
            }

            if (data.new_labels > 0) {
                os << "Inserted " << data.new_labels << " new labels\n";
            }

            if (data.new_properties > 0) {
                os << "Inserted " << data.new_properties << " new properties\n";
            }

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

    const GraphUpdateData& get_graph_update_data() const
    {
        return graph_update_data;
    }

    const TensorUpdateData& get_tensor_update_data() const
    {
        return tensor_update_data;
    }

private:
    std::map<ObjectId, ObjectId> created_ids;

    // returns true if oid was transformed
    bool transform_if_tmp(ObjectId& oid);

    GraphUpdateData graph_update_data;

    TensorUpdateData tensor_update_data;

    std::vector<TextSearchIndexUpdateData> text_search_index_updates;
};
} // namespace MQL
