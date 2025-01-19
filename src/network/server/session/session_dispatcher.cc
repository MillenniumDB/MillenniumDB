#include "session_dispatcher.h"

#include "misc/logger.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/http/http_gql_session.h"
#include "network/server/session/http/http_quad_session.h"
#include "network/server/session/http/http_rdf_session.h"
#include "network/server/session/streaming/streaming_tcp_session.h"
#include "network/server/session/streaming/streaming_websocket_session.h"

using namespace MDBServer;
using namespace boost;
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;

void SessionDispatcher::run()
{
    // Set the timeout for getting the query
    auto self = this->shared_from_this();

    // Peek initial bytes to determine the client type
    asio::async_read(
        socket,
        read_buffer.prepare(Protocol::DRIVER_PREAMBLE.size()),
        boost::asio::transfer_all(),
        [self](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                self->socket.close();
                logger(Category::Error) << "Could not read the client's preamble";
                return;
            }

            self->read_buffer.commit(Protocol::DRIVER_PREAMBLE.size());
            auto preamble = std::string(
                asio::buffer_cast<const char*>(self->read_buffer.data()),
                Protocol::DRIVER_PREAMBLE.size()
            );
            if (preamble == Protocol::DRIVER_PREAMBLE) {
                // The client is using the driver protocol, send the proper preamble
                asio::async_write(
                    self->socket,
                    asio::buffer(Protocol::SERVER_PREAMBLE),
                    [self = self->shared_from_this(
                     )](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
                        if (ec) {
                            self->socket.close();
                            logger(Category::Error) << "Could not write the server's preamble";
                            return;
                        }

                        std::make_shared<StreamingTCPSession>(
                            self->server,
                            std::move(self->socket),
                            self->query_timeout
                        )
                            ->run();
                    }
                );
                return;
            }

            self->read_http_header();
        }
    );
}

void SessionDispatcher::read_http_header()
{
    http_parser.eager(true);
    http_parser.body_limit(16 * 1024 * 1024);

    auto self = this->shared_from_this();
    asio::async_read_until(
        socket,
        read_buffer,
        "\r\n\r\n",
        [self](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                self->socket.close();
                logger(Category::Error) << "Could not read the HTTP header: " << ec.message();
                return;
            }
            auto tmp_buf = boost::asio::buffer(self->read_buffer.data(), self->read_buffer.size());
            boost::system::error_code ec2;
            self->http_parser.put(tmp_buf, ec2);
            self->read_buffer.consume(self->read_buffer.size());

            if (ec2) {
                self->socket.close();
                logger(Category::Error) << "Could not parse the HTTP header: " << ec2.message();
                return;
            }
            self->read_http_body();
        }
    );
}

void SessionDispatcher::read_http_body()
{
    if (!http_parser.is_done()) {
        auto self = this->shared_from_this();
        boost::asio::async_read(
            socket,
            read_buffer,
            boost::asio::transfer_at_least(1),
            [self](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
                if (ec) {
                    self->socket.close();
                    logger(Category::Error) << "Could not parse the HTTP body: " << ec.message();
                    return;
                }
                auto tmp_buf = boost::asio::buffer(self->read_buffer.data(), self->read_buffer.size());
                boost::system::error_code ec2;
                self->http_parser.put(tmp_buf, ec2);
                if (ec2) {
                    self->socket.close();
                    logger(Category::Error) << "Could not parse the HTTP: " << ec2.message();
                    return;
                }
                self->read_buffer.consume(self->read_buffer.size());
                self->read_http_body();
            }
        );
    } else {
        dispatch_http();
    }
}

void SessionDispatcher::dispatch_http()
{
    http::request<http::string_body> http_request = http_parser.release();
    beast::tcp_stream stream(std::move(socket));

    if (websocket::is_upgrade(http_request)) {
        auto ws_stream = std::make_unique<websocket::stream<beast::tcp_stream>>(std::move(stream));
        auto* ws_stream_p = ws_stream.get();

        // Try to handshake with the WebSocket client
        ws_stream_p->async_accept(
            http_request,
            [ws_stream = std::move(ws_stream),
             &server = server,
             query_timeout = query_timeout](const boost::system::error_code& ec) {
                if (ec) {
                    ws_stream->close(websocket::close_code::abnormal);
                    logger(Category::Error) << "Could not perform the WebSocket handshake with the client";
                    return;
                }

                logger(Category::Debug) << "Dispatching StreamingWebSocketSession";
                StreamingWebSocketSession::run(
                    std::make_unique<StreamingWebSocketSession>(server, std::move(*ws_stream), query_timeout)
                );
            }
        );
        return;
    }

    // Handle regular HTTP requests
    logger(Category::Debug) << "Dispatching HTTPSession";
    if (server.model_id == Protocol::QUAD_MODEL_ID) {
        HttpQuadSession::run(std::make_unique<HttpQuadSession>(
            server,
            std::move(stream),
            std::move(http_request),
            query_timeout
        ));
    } else if (server.model_id == Protocol::RDF_MODEL_ID) {
        HttpRdfSession::run(std::make_unique<HttpRdfSession>(
            server,
            std::move(stream),
            std::move(http_request),
            query_timeout
        ));
    } else if (server.model_id == Protocol::GQL_MODEL_ID) {
        HttpGQLSession::run(std::make_unique<HttpGQLSession>(
            server,
            std::move(stream),
            std::move(http_request),
            query_timeout
        ));
    } else {
        throw std::runtime_error("Unhandled ModelId: " + std::to_string(server.model_id));
    }
}
