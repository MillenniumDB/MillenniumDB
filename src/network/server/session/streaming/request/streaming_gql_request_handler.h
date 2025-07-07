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

    OpUptr create_logical_plan(const std::string& query) override
    {
        auto logical_plan = GQL::QueryParser::get_query_plan(query);
        return logical_plan;
    }

    std::unique_ptr<StreamingQueryExecutor> create_readonly_physical_plan(OpUptr& logical_plan) override
    {
        GQL::StreamingExecutorConstructor query_optimizer;
        std::get<std::unique_ptr<GQL::Op>>(logical_plan)->accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }

    void execute_update(OpUptr& logical_plan, BufferManager::VersionScope& version_scope) override
    {
        // TODO: GQL
    }
};
} // namespace MDBServer
