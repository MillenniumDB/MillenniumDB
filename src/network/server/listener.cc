#include "listener.h"

#include <boost/asio/strand.hpp>

#include "misc/fatal_error.h"
#include "misc/logger.h"
#include "network/server/session/session_dispatcher.h"

using namespace MDBServer;
using namespace boost;

Listener::Listener(
    Server& server,
    asio::io_context& io_context,
    std::optional<asio::ssl::context>& ssl_ctx,
    asio::ip::tcp::endpoint endpoint,
    std::chrono::seconds query_timeout
) :
    server(server),
    io_context(io_context),
    ssl_ctx(ssl_ctx),
    acceptor(boost::asio::make_strand(io_context)),
    endpoint(endpoint),
    query_timeout(query_timeout)
{
    boost::system::error_code ec;

    // Open the acceptor
    acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "open");
    }

    // Allow address reuse
    acceptor.set_option(asio::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set options");
    }

    // Bind to the server address
    acceptor.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
    }

    // Start listening for connections
    acceptor.listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
    }
}

void Listener::run()
{
    acceptor.async_accept(boost::asio::make_strand(io_context), [&](const boost::system::error_code& ec, asio::ip::tcp::socket socket) {
        // A new connection is accepted
        logger.debug() << "New client connected";
        if (!ec) {
            // Disable Nagle's Algorithm to reduce latency, as our protocol may flush many small
            // messages when answering requests
            socket.set_option(asio::ip::tcp::no_delay(true));
            std::make_shared<SessionSSLDetector>(server, std::move(socket), ssl_ctx, query_timeout)->run();
        }

        // Accept another connection
        run();
    });
}

void Listener::fail(const boost::system::error_code& ec, char const* what) const
{
    if (ec == boost::asio::error::address_in_use) {
        FATAL_ERROR("Port ", endpoint.port(), " already in use, try using a different port");
    } else {
        FATAL_ERROR("MillenniumDB::Listener error while trying to ", what, ": ", ec.message());
    }
}
