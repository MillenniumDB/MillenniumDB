#pragma once

#include "network/server/session/streaming/response/streaming_response_writer.h"

using DurationMS = std::chrono::duration<float, std::milli>;

class StreamingQueryExecutor {
public:
    virtual ~StreamingQueryExecutor() = default;

    std::vector<VarId> projection_vars;

    // Write each result of the query to the given ResponseWriter. Returns the number of results written
    virtual void execute(MDBServer::StreamingResponseWriter& response_writer) = 0;

    virtual void finish_success(
        MDBServer::StreamingResponseWriter& response_writer,
        DurationMS parser_duration_ms,
        DurationMS optimizer_duration_ms
    ) = 0;

    // Analyze the query executor
    virtual void analyze(std::ostream&, bool print_stats, int indent = 0) const = 0;
};
