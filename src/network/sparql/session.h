#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <ostream>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "network/sparql/response_type.h"
#include "network/sparql/server.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/query_context.h"

class Op;

namespace SPARQL {

class Server;

// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session> {
    Server& server;

    boost::beast::tcp_stream stream;
    boost::beast::flat_buffer buffer;
    boost::beast::http::request<boost::beast::http::string_body> req;
    std::chrono::seconds timeout;

public:
    // Take ownership of the stream
    Session(
        Server& server,
        boost::asio::ip::tcp::socket&& socket,
        std::chrono::seconds timeout
    ) :
        server  (server),
        stream  (std::move(socket)),
        timeout (timeout) { }

    // Start the asynchronous operation
    void run();

    void do_read();

    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

    void fail(boost::beast::error_code ec, char const* what) {
        std::cerr << what << ": " << ec.message() << "\n";
    }

private:
    std::unique_ptr<Op> create_logical_plan(const std::string& query,
                                            std::chrono::duration<float, std::milli>* duration);

    std::unique_ptr<QueryExecutor> create_readonly_physical_plan(Op& logical_plan,
                                                                 std::chrono::duration<float, std::milli>* duration,
                                                                 ResponseType response_type);

    void execute_plan(QueryExecutor& physical_plan,
                      std::ostream& os,
                      std::chrono::duration<float, std::milli>* duration,
                      ResponseType response_type);
};
}