#pragma once

#include "network/server/session/streaming/request/streaming_rdf_request_reader.h"
#include "network/server/session/streaming/response/streaming_rdf_response_writer.h"
#include "query/optimizer/rdf_model/streaming_executor_constructor.h"
#include "query/parser/sparql_query_parser.h"
#include "streaming_request_handler.h"

namespace MDBServer {

class StreamingRdfRequestHandler : public StreamingRequestHandler {
public:
    StreamingRdfRequestHandler(StreamingSession& session) :
        StreamingRequestHandler(
            session,
            std::make_unique<StreamingRdfRequestReader>(),
            std::make_unique<StreamingRdfResponseWriter>(session)
        )
    { }

    ~StreamingRdfRequestHandler() = default;

    void initial_parse(const std::string& query) override
    {
        parser = std::make_unique<SPARQL::QueryParser>(query);
    }

    bool is_update() override
    {
        return false; // TODO: for now streaming does not support updates in SPARQL
    }

    void create_logical_plan(const std::map<std::string, ObjectId>& input_parameters) override
    {
        logical_plan = parser->get_query_plan(input_parameters);
    }

    std::unique_ptr<StreamingQueryExecutor> create_streaming_executor() override
    {
        SPARQL::StreamingExecutorConstructor query_optimizer;
        logical_plan->accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }

    std::unique_ptr<SPARQL::QueryParser> parser;

    std::unique_ptr<SPARQL::Op> logical_plan;
};
} // namespace MDBServer
