#pragma once

#include <mutex>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "network/server/protocol.h"
#include "query/executor/query_executor/mql/return_executor.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/op.h"

namespace MDBServer {

class Server;

class HttpQuadSession : public std::enable_shared_from_this<HttpQuadSession> {
    using stream_type = boost::beast::tcp_stream;

public:
    static inline std::mutex update_mutex;

    using DurationMS = std::chrono::duration<float, std::milli>;

    DurationMS parser_duration;
    DurationMS optimizer_duration;
    DurationMS execution_duration;

    explicit HttpQuadSession(Server&                                                        server,
                             stream_type&&                                                  stream,
                             boost::beast::http::request<boost::beast::http::string_body>&& request,
                             std::chrono::seconds                                           query_timeout);

    virtual ~HttpQuadSession();

    static void run(std::unique_ptr<HttpQuadSession>);

private:
    boost::beast::error_code ec;

    Server& server;

    stream_type stream;

    boost::beast::http::request<boost::beast::http::string_body> request;

    std::chrono::seconds query_timeout;

    void execute_query(const std::string& query, std::ostream& os, MQL::ReturnType response_type);

    std::unique_ptr<Op> create_logical_plan(const std::string& query);

    std::unique_ptr<QueryExecutor> create_readonly_physical_plan(Op& logical_plan, MQL::ReturnType response_type);

    void execute_readonly_query_plan(QueryExecutor& physical_plan, std::ostream& os, MQL::ReturnType response_type);

    void execute_update(Op& logical_plan, BufferManager::VersionScope& version_scope, std::ostream& os);
};
} // namespace MDBServer
