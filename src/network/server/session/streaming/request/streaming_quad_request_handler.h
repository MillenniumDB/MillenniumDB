#include "streaming_request_handler.h"

#include <antlr4-runtime.h>

#include "network/server/session/streaming/response/streaming_quad_response_writer.h"
#include "query/optimizer/quad_model/streaming_executor_constructor.h"
#include "query/parser/mql_query_parser.h"

namespace MDBServer {

class StreamingQuadRequestHandler : public StreamingRequestHandler {
public:
    StreamingQuadRequestHandler(StreamingSession& session) :
        StreamingRequestHandler(session, std::make_unique<StreamingQuadResponseWriter>(session)) { }

    ~StreamingQuadRequestHandler() = default;

    std::unique_ptr<Op> create_readonly_logical_plan(const std::string& query) override {
        auto logical_plan = MQL::QueryParser::get_query_plan(query);
        return logical_plan;
    }

    std::unique_ptr<StreamingQueryExecutor> create_readonly_physical_plan(Op& logical_plan) override {
        MQL::StreamingExecutorConstructor query_optimizer;
        logical_plan.accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }
};
} // namespace MDBServer
