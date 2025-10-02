#pragma once

#include "streaming_request_handler.h"

#include "network/server/session/streaming/response/streaming_gql_response_writer.h"
#include "query/optimizer/property_graph_model/streaming_executor_constructor.h"
#include "query/optimizer/rdf_model/streaming_executor_constructor.h"
#include "query/parser/gql_query_parser.h"

namespace MDBServer {

class StreamingGQLRequestHandler : public StreamingRequestHandler {
public:
    StreamingGQLRequestHandler(StreamingSession& session) :
        StreamingRequestHandler(session, std::make_unique<StreamingGQLResponseWriter>(session))
    { }

    ~StreamingGQLRequestHandler() = default;

    void initial_parse(const std::string& query) override
    {
        logical_plan = GQL::QueryParser::get_query_plan(query);
    }

    bool is_update() override
    {
        return false; // TODO:
    }

    void create_logical_plan() override
    {
        // TODO:
    }

    std::unique_ptr<StreamingQueryExecutor> create_streaming_executor() override
    {
        GQL::StreamingExecutorConstructor query_optimizer;
        logical_plan->accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }

    std::unique_ptr<GQL::Op> logical_plan;
};
} // namespace MDBServer
