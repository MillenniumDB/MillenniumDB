#pragma once

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/streaming_query_executor.h"

namespace MQL {
/**
 * Executor for DESCRIBE queries which writes a single "hand-made" record with the
 * projection variables: labels, properties, outgoing and incoming
 */
class EdgeDescribeStreamingExecutor : public StreamingQueryExecutor {
public:
    EdgeDescribeStreamingExecutor(std::unique_ptr<BindingIter> key_value_iter,
                                  uint64_t                     properties_limit,
                                  std::vector<VarId>&&         virtual_vars,
                                  ObjectId                     object_id);

    const std::vector<VarId>& get_projection_vars() const override;

    uint64_t execute(MDBServer::StreamingResponseWriter& response_writer) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> key_value_iter;

    uint64_t properties_limit;

    ObjectId object_id;

    // Internal variables used in the operator
    std::vector<VarId> virtual_vars;

    std::vector<VarId> projection_vars;
};
} // namespace MQL
