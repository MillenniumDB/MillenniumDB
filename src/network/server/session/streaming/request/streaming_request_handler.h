#pragma once

#include <cstdint>
#include <memory>

#include "network/server/session/streaming/request/streaming_request_reader.h"
#include "network/server/session/streaming/response/streaming_response_writer.h"
#include "query/executor/query_executor/streaming_query_executor.h"

namespace MDBServer {

class StreamingSession;

class StreamingRequestHandler {
public:
    using DurationMS = std::chrono::duration<float, std::milli>;

    StreamingSession& session;

    std::unique_ptr<StreamingResponseWriter> response_writer;

    StreamingRequestHandler(
        StreamingSession& session,
        std::unique_ptr<StreamingResponseWriter> response_writer
    ) :
        session(session),
        response_writer(std::move(response_writer))
    { }

    virtual ~StreamingRequestHandler() = default;

    void handle(const uint8_t* request_bytes, std::size_t request_size);

protected:
    StreamingRequestReader request_reader;

    virtual void initial_parse(const std::string& query) = 0;

    // is supposed to be called after initial_parse
    virtual bool is_update() = 0;

    virtual void create_logical_plan() = 0;

    virtual std::unique_ptr<StreamingQueryExecutor> create_streaming_executor() = 0;

    void handle_run(const std::string& query);

    void handle_readonly_run();

    void handle_update_run();

    // Send a catalog response to the client
    void handle_catalog();

    void handle_cancel();

    static inline DurationMS get_duration(std::chrono::system_clock::time_point start)
    {
        return std::chrono::system_clock::now() - start;
    }
};
} // namespace MDBServer
