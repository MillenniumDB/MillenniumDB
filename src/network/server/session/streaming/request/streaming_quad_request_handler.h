#pragma once

#include "network/server/session/streaming/request/streaming_quad_request_reader.h"
#include "network/server/session/streaming/request/streaming_request_handler.h"
#include "network/server/session/streaming/response/streaming_quad_response_writer.h"
#include "query/optimizer/quad_model/streaming_executor_constructor.h"
#include "query/parser/mql_query_parser.h"

namespace MDBServer {

class StreamingQuadRequestHandler : public StreamingRequestHandler {
public:
    StreamingQuadRequestHandler(StreamingSession& session) :
        StreamingRequestHandler(
            session,
            std::make_unique<StreamingQuadRequestReader>(),
            std::make_unique<StreamingQuadResponseWriter>(session)
        )
    { }

    ~StreamingQuadRequestHandler() = default;

    void initial_parse(const std::string& query) override
    {
        parser = std::make_unique<MQL::QueryParser>(query);
    }

    bool is_update() override
    {
        return parser->is_update();
    }

    void create_logical_plan(const std::map<std::string, ObjectId>& input_parameters) override
    {
        logical_plan = parser->get_query_plan(input_parameters);
    }

    std::unique_ptr<StreamingQueryExecutor> create_streaming_executor() override
    {
        MQL::StreamingExecutorConstructor query_optimizer;
        logical_plan->accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }

    std::unique_ptr<MQL::QueryParser> parser;

    std::unique_ptr<MQL::Op> logical_plan;

};
} // namespace MDBServer
