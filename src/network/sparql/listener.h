#pragma once

#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>

#include "network/sparql/session.h"

namespace SPARQL {

class Server;

class Listener : public std::enable_shared_from_this<Listener> {
    Server& server;
    boost::asio::io_context& io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    std::chrono::seconds timeout;

public:
    Listener(
        Server& server,
        boost::asio::io_context& io_context,
        boost::asio::ip::tcp::endpoint endpoint,
        std::chrono::seconds timeout
    ) :
        server     (server),
        io_context (io_context),
        acceptor   (boost::asio::make_strand(io_context)),
        timeout    (timeout)
    {
        boost::beast::error_code ec;

        // Open the acceptor
        acceptor.open(endpoint.protocol(), ec);
        if (ec) {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
        if (ec) {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor.bind(endpoint, ec);
        if (ec) {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
        if (ec) {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void run() {
        do_accept();
    }

private:
    void do_accept() {
        // The new connection gets its own strand
        acceptor.async_accept(
            boost::asio::make_strand(io_context),
            boost::beast::bind_front_handler(
                &Listener::on_accept,
                shared_from_this()
            )
        );
    }

    void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (ec) {
            fail(ec, "accept");
            return; // To avoid infinite loop
        } else {
            // Create the session and run it
            std::make_shared<Session>(server, std::move(socket), timeout)->run();
        }

        // Accept another connection
        do_accept();
    }

    void fail(boost::beast::error_code ec, char const* what) {
        std::cerr << what << ": " << ec.message() << "\n";
    }
};
} // namespace SPARQL
