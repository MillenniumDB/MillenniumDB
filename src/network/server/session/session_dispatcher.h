#pragma once

#include <chrono>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

#include "network/server/server.h"

namespace MDBServer {

/**
 * Handle the initial connection and chooses the session based on the type of client that has connected
 */
template<typename stream_t>
class SessionDispatcher : public std::enable_shared_from_this<SessionDispatcher<stream_t>> {
public:
    SessionDispatcher(
        Server& server,
        stream_t&& stream,
        std::chrono::seconds query_timeout
    ) :
        server(server),
        stream(std::move(stream)),
        query_timeout(query_timeout)
    { }

    void run();

    void read_http_header();

    void read_http_body();

    void dispatch_http();

    std::pair<std::string, std::string>
        get_user_password(const boost::beast::http::request<boost::beast::http::string_body>& http_request);

private:
    Server& server;

    stream_t stream;

    boost::asio::streambuf read_buffer;

    std::chrono::seconds query_timeout;

    boost::beast::http::request_parser<boost::beast::http::string_body> http_parser;

    // helper that closes either raw tcp or ssl stream
    static void close_stream(stream_t& stream);
};

class SessionSSLDetector : public std::enable_shared_from_this<SessionSSLDetector> {
public:
    SessionSSLDetector(
        Server& server,
        boost::asio::ip::tcp::socket&& socket,
        std::optional<boost::asio::ssl::context>& ssl_ctx,
        std::chrono::seconds query_timeout
    ) :
        server(server),
        tcp_stream(std::move(socket)),
        ssl_ctx(ssl_ctx),
        query_timeout(query_timeout)
    { }

    void run();

private:
    Server& server;

    boost::asio::ip::tcp::socket tcp_stream;

    std::optional<boost::asio::ssl::context>& ssl_ctx;

    std::chrono::seconds query_timeout;

    boost::asio::streambuf read_buffer;

    std::unique_ptr<boost::beast::ssl_stream<boost::asio::ip::tcp::socket>> ssl_stream;

    // used to peek the request and check if it is encrypted or not
    uint8_t peek_byte = 0;
};
} // namespace MDBServer
