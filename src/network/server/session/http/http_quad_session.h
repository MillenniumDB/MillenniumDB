#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "query/executor/query_executor/mql/return_type.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/mql_query_parser.h"
#include "query/parser/op/mql/op.h"

namespace MDBServer {

class Server;

template<typename stream_t>
class HttpQuadSession {
public:
    using DurationMS = std::chrono::duration<float, std::milli>;

    DurationMS parser_duration;
    DurationMS optimizer_duration;
    DurationMS execution_duration;

    explicit HttpQuadSession(
        Server& server,
        stream_t&& stream,
        boost::beast::http::request<boost::beast::http::string_body>&& request,
        std::chrono::seconds query_timeout
    );

    virtual ~HttpQuadSession();

    static void run(std::unique_ptr<HttpQuadSession>);

private:
    boost::beast::error_code ec;

    Server& server;

    stream_t stream;

    boost::beast::http::request<boost::beast::http::string_body> request;

    std::chrono::seconds query_timeout;

    void execute_query(const std::string& query, std::ostream& os, MQL::ReturnType response_type);

    std::unique_ptr<QueryExecutor>
        create_query_executor(MQL::Op& logical_plan, MQL::ReturnType response_type);

    void run_read_query(MQL::QueryParser& parser, std::ostream& os, MQL::ReturnType response_type);

    void run_write_query(MQL::QueryParser& parser, std::ostream& os);
};
} // namespace MDBServer
