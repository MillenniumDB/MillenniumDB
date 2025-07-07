#pragma once

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/streaming_query_executor.h"

namespace SPARQL {
/**
 * Executor for SELECT queries which writes records to the Response Writer
 */
class SelectStreamingExecutor : public StreamingQueryExecutor {
public:
    SelectStreamingExecutor(std::unique_ptr<BindingIter> iter, std::vector<VarId>&& projection_vars);

    const std::vector<VarId>& get_projection_vars() const override;

    uint64_t execute(MDBServer::StreamingResponseWriter& response_writer) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> iter;

    std::vector<VarId> projection_vars;
};
} // namespace SPARQL
