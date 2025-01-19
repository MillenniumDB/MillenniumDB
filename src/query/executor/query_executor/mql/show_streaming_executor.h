#pragma once

#include "query/executor/query_executor/streaming_query_executor.h"
#include "query/parser/op/mql/op_show.h"

namespace MQL {
/**
 * Executor for Show queries which writes records to the Response Writer
 */
template<OpShow::Type type>
class ShowStreamingExecutor : public StreamingQueryExecutor {
public:
    ShowStreamingExecutor();

    const std::vector<VarId>& get_projection_vars() const override;

    uint64_t execute(MDBServer::StreamingResponseWriter& response_writer) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    std::vector<VarId> projection_vars;
};
} // namespace MQL
