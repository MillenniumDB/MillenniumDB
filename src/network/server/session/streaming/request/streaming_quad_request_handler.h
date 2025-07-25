#pragma once

#include "streaming_request_handler.h"

#include "network/server/session/streaming/response/streaming_quad_response_writer.h"
#include "query/optimizer/quad_model/streaming_executor_constructor.h"
#include "query/parser/mql_query_parser.h"
#include "update/mql/update_executor.h"

namespace MDBServer {

class StreamingQuadRequestHandler : public StreamingRequestHandler {
public:
    StreamingQuadRequestHandler(StreamingSession& session) :
        StreamingRequestHandler(session, std::make_unique<StreamingQuadResponseWriter>(session))
    { }

    ~StreamingQuadRequestHandler() = default;

    OpUptr create_logical_plan(const std::string& query) override
    {
        auto logical_plan = MQL::QueryParser::get_query_plan(query);
        return logical_plan;
    }

    std::unique_ptr<StreamingQueryExecutor> create_readonly_physical_plan(OpUptr& logical_plan) override
    {
        MQL::StreamingExecutorConstructor query_optimizer;
        std::get<std::unique_ptr<MQL::Op>>(logical_plan)->accept_visitor(query_optimizer);
        return std::move(query_optimizer.executor);
    }

    void execute_update(OpUptr& logical_plan, BufferManager::VersionScope& version_scope) override
    {
        // Mutex to allow only one write query at a time
        std::lock_guard<std::mutex> lock(session.server.update_execution_mutex);

        response_writer->write_variables(
            {},
            get_query_ctx().thread_info.worker_index,
            get_query_ctx().cancellation_token
        );

        buffer_manager.upgrade_to_editable(version_scope);

        const auto execution_start = std::chrono::system_clock::now();
        MQL::UpdateExecutor update_executor;
        update_executor.execute(*std::get<std::unique_ptr<MQL::Op>>(logical_plan));
        execution_duration_ms = get_duration(execution_start);

        logger.log(Category::ExecutionStats, [&update_executor](std::ostream& os) {
            update_executor.print_stats(os);
        });

        response_writer->write_update_success(
            parser_duration_ms.count(),
            optimizer_duration_ms.count(),
            execution_duration_ms.count()
        );

        response_writer->flush();
    }
};
} // namespace MDBServer
