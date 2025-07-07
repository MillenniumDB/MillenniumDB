#pragma once

#include "streaming_request_handler.h"

#include "network/server/session/streaming/response/streaming_rdf_response_writer.h"
#include "query/optimizer/rdf_model/streaming_executor_constructor.h"
#include "query/parser/sparql_query_parser.h"

namespace MDBServer {

class StreamingRdfRequestHandler : public StreamingRequestHandler {
public:
    StreamingRdfRequestHandler(StreamingSession& session) :
        StreamingRequestHandler(session, std::make_unique<StreamingRdfResponseWriter>(session))
    { }

    ~StreamingRdfRequestHandler() = default;

    OpUptr create_logical_plan(const std::string& query) override
    {
        // TODO: Support updates
        auto logical_plan = SPARQL::QueryParser::get_query_plan(query);
        return logical_plan;
    }

    std::unique_ptr<StreamingQueryExecutor> create_readonly_physical_plan(OpUptr& logical_plan) override
    {
        SPARQL::StreamingExecutorConstructor query_optimizer;
        std::get<std::unique_ptr<SPARQL::Op>>(logical_plan)->accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }

    void execute_update(OpUptr& /*logical_plan*/, BufferManager::VersionScope& /*version_scope*/) override
    {
        response_writer->write_error("Updates not supported in RDF streaming mode yet");
        response_writer->flush();
    }
};
} // namespace MDBServer
