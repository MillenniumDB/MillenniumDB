#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "network/server/server.h"

namespace MDBServer {

class Listener {
public:
    Server& server;
    boost::asio::io_context& io_context;
    std::optional<boost::asio::ssl::context>& ssl_ctx;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::endpoint endpoint;
    std::chrono::seconds query_timeout;

    explicit Listener(
        Server& server,
        boost::asio::io_context& io_context,
        std::optional<boost::asio::ssl::context>& ssl_ctx,
        boost::asio::ip::tcp::endpoint endpoint,
        std::chrono::seconds query_timeout
    );

    void run();

private:
    inline void fail(const boost::system::error_code& ec, char const* what) const;
};
} // namespace MDBServer
