#pragma once

#include "network/server/session/streaming/response/streaming_response_writer.h"


class StreamingQueryExecutor {
public:
    virtual ~StreamingQueryExecutor() = default;

    // Returns the projection variables
    virtual const std::vector<VarId>& get_projection_vars() const = 0;

    // Write each result of the query to the given ResponseWriter. Returns the number of results written
    virtual uint64_t execute(MDBServer::StreamingResponseWriter& response_writer) = 0;

    // Analyze the query executor
    virtual void analyze(std::ostream&, bool print_stats, int indent = 0) const = 0;
};