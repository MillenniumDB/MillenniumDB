#pragma once

#include <boost/unordered/unordered_flat_map.hpp>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/executor/query_executor/streaming_query_executor.h"
#include "query/update/hnsw_index_update_stats.h"
#include "query/update/mql/update_action.h"
#include "query/update/text_index_update_stats.h"

namespace MQL {

class UpdateExecutor : public QueryExecutor, public StreamingQueryExecutor {
public:
    struct Stats {
        uint_fast32_t new_nodes = 0;
        uint_fast32_t new_edges = 0;
        uint_fast32_t new_labels = 0;
        uint_fast32_t new_properties = 0;
        uint_fast32_t deleted_nodes = 0;
        uint_fast32_t deleted_edges = 0;
        uint_fast32_t deleted_labels = 0;
        uint_fast32_t deleted_properties = 0;
        uint_fast32_t overwritten_properties = 0;

        boost::unordered_flat_map<uint64_t, int64_t> label2total_count;
        boost::unordered_flat_map<uint64_t, int64_t> key2total_count;
        boost::unordered_flat_map<uint64_t, int64_t> type2total_count;

        boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_to_type_count;
        boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_to_count;
        boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_type_count;
        boost::unordered_flat_map<uint64_t, int64_t> type2equal_to_type_count;

        bool empty() const
        {
            return new_nodes == 0 && new_edges == 0 && new_labels == 0 && new_properties == 0
                && overwritten_properties == 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const Stats& data)
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

    UpdateExecutor(
        std::unique_ptr<BindingIter> iter,
        std::unique_ptr<UpdateContext> update_context,
        std::vector<std::unique_ptr<UpdateAction>>&& update_actions
    ) :
        iter(std::move(iter)),
        update_context(std::move(update_context)),
        update_actions(std::move(update_actions))
    { }

    ~UpdateExecutor();

    uint64_t execute();

    uint64_t execute(std::ostream&) override
    {
        return execute();
    }

    uint64_t execute(MDBServer::StreamingResponseWriter&) override
    {
        return execute();
    }

    void analyze(std::ostream& os, bool print_stats, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> iter;
    std::unique_ptr<UpdateContext> update_context;
    std::vector<std::unique_ptr<UpdateAction>> update_actions;
    // Stats stats;

    // returns a non-tmp ObjectId
    ObjectId transform_if_tmp(ObjectId oid);

    void process_new_property(ObjectId obj, ObjectId key, ObjectId val);
    void process_deleted_property(ObjectId obj, ObjectId key, ObjectId val);

    void update_indexes();

    // boost::unordered_flat_map<std::string, TextIndexUpdateStats> name2text_index_update_data;
    // boost::unordered_flat_map<std::string, HNSWIndexUpdateStats> name2hnsw_index_update_data;

    // void insert_text_index_update_data(TextIndexUpdateStats&& text_index_update_data);
    // void insert_hnsw_index_update_data(HNSWIndexUpdateStats&& hnsw_index_update_data);
};
} // namespace MQL
