#pragma once

#include <mutex>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "network/sparql/response_type.h"
#include "query/executor/query_executor/gql/return_executor.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/gql/op.h"

namespace MDBServer {

class Server;

class HttpGQLSession {
    using stream_type = boost::beast::tcp_stream;

public:
    using DurationMS = std::chrono::duration<float, std::milli>;

    DurationMS parser_duration;
    DurationMS optimizer_duration;
    DurationMS execution_duration;

    explicit HttpGQLSession(
        Server& server,
        stream_type&& stream,
        boost::beast::http::request<boost::beast::http::string_body>&& request,
        std::chrono::seconds query_timeout
    );

    virtual ~HttpGQLSession();

    static void run(std::unique_ptr<HttpGQLSession>);

private:
    boost::beast::error_code ec;

    Server& server;

    stream_type stream;

    boost::beast::http::request<boost::beast::http::string_body> request;

    std::chrono::seconds query_timeout;

    void execute_readonly_query(const std::string& query, std::ostream& os, GQL::ReturnType response_type);

    std::unique_ptr<GQL::Op> create_readonly_logical_plan(const std::string& query);

    std::unique_ptr<QueryExecutor>
        create_readonly_physical_plan(GQL::Op& logical_plan, GQL::ReturnType response_type);

    void execute_readonly_query_plan(
        QueryExecutor& physical_plan,
        std::ostream& os,
        GQL::ReturnType response_type
    );

    void execute_update_query(const std::string& query, std::ostream& os);
};
} // namespace MDBServer
