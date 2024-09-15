#pragma once

#include <cstdint>
#include <memory>

#include "network/server/session/streaming/request/streaming_request_reader.h"
#include "network/server/session/streaming/response/streaming_response_writer.h"
#include "query/executor/query_executor/streaming_query_executor.h"
#include "query/parser/op/op.h"

namespace MDBServer {

class StreamingSession;

class StreamingRequestHandler {
public:
    using DurationMS = std::chrono::duration<float, std::milli>;

    StreamingSession& session;

    std::unique_ptr<StreamingResponseWriter> response_writer;

    StreamingRequestHandler(StreamingSession& session, std::unique_ptr<StreamingResponseWriter> response_writer) :
        session         (session),
        response_writer (std::move(response_writer)) { }

    virtual ~StreamingRequestHandler() = default;

    void handle(const uint8_t* request_bytes, std::size_t request_size);

private:
    DurationMS parser_duration_ms;
    DurationMS optimizer_duration_ms;
    DurationMS execution_duration_ms;

    StreamingRequestReader request_reader;

    std::unique_ptr<StreamingQueryExecutor> current_physical_plan;

    virtual std::unique_ptr<Op> create_readonly_logical_plan(const std::string& query) = 0;

    virtual std::unique_ptr<StreamingQueryExecutor> create_readonly_physical_plan(Op& logical_plan) = 0;

    // Build the logical and physical plan. On success store the result in current_physical_plan and transition to
    // STREAMING state.
    void handle_run(const std::string& query);

    // Send a catalog response to the client
    void handle_catalog();

    void handle_cancel();
};
} // namespace MDBServer
