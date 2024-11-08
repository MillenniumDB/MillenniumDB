#pragma once

#include <chrono>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "network/server/protocol.h"


namespace MDBServer {

class Server;

/**
 * Handle the initial connection and chooses the session based on the type of client that has connected
 */
class SessionDispatcher : public std::enable_shared_from_this<SessionDispatcher> {
public:
    SessionDispatcher(Server&                        server,
                      boost::asio::ip::tcp::socket&& socket,
                      std::chrono::seconds           query_timeout) :
        server        (server),
        socket        (std::move(socket)),
        query_timeout (query_timeout) { }

    void run();

    void read_http_header();

    void read_http_body();

    void dispatch_http();


private:
    Server& server;

    boost::asio::ip::tcp::socket socket;

    std::chrono::seconds query_timeout;

    boost::asio::streambuf read_buffer;

    boost::beast::http::request_parser<boost::beast::http::string_body> http_parser;

};
} // namespace MDBServer
