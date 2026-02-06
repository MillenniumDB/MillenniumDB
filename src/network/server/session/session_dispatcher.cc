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
namespace http = beast::http;
namespace websocket = beast::websocket;

template<typename stream_t>
void SessionDispatcher<stream_t>::run()
{
    read_http_header();
}

template<typename stream_t>
void SessionDispatcher<stream_t>::read_http_header()
{
    http_parser.eager(true);
    http_parser.body_limit(16 * 1024 * 1024);

    asio::async_read_until(
        stream,
        read_buffer,
        "\r\n\r\n",
        [self = this->shared_from_this()](const system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                logger.error() << "Could not read the HTTP header: " << ec.message();
                SessionDispatcher<stream_t>::close_stream(self->stream);
                return;
            }
            auto tmp_buf = asio::buffer(self->read_buffer.data(), self->read_buffer.size());
            system::error_code ec2;
            self->http_parser.put(tmp_buf, ec2);
            self->read_buffer.consume(self->read_buffer.size());

            if (ec2) {
                logger.error() << "Could not parse the HTTP header: " << ec2.message();
                SessionDispatcher<stream_t>::close_stream(self->stream);
                return;
            }
            self->read_http_body();
        }
    );
}

template<typename stream_t>
void SessionDispatcher<stream_t>::read_http_body()
{
    if (!http_parser.is_done()) {
        asio::async_read(
            stream,
            read_buffer,
            asio::transfer_at_least(1),
            [self =
                 this->shared_from_this()](const system::error_code& ec, std::size_t /*bytes_transferred*/) {
                if (ec) {
                    SessionDispatcher<stream_t>::close_stream(self->stream);
                    logger.error() << "Could not parse the HTTP body: " << ec.message();
                    return;
                }
                auto tmp_buf = asio::buffer(self->read_buffer.data(), self->read_buffer.size());
                system::error_code ec2;
                self->http_parser.put(tmp_buf, ec2);
                if (ec2) {
                    SessionDispatcher<stream_t>::close_stream(self->stream);
                    logger.error() << "Could not parse the HTTP: " << ec2.message();
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

template<typename stream_t>
void SessionDispatcher<stream_t>::dispatch_http()
{
    http::request<http::string_body> http_request = http_parser.release();

    bool write_authorized = !server.has_admin_user();
    const auto&& [user, password] = get_user_password(http_request);

    if (!user.empty() && !password.empty()) {
        const auto [auth_token, valid_until] = server.create_auth_token(user, password);
        if (!auth_token.empty()) {
            write_authorized = true;
        } else {
            // TODO: tell the user that authorization failed?
        }
    }

    if (websocket::is_upgrade(http_request)) {
        auto ws_stream = std::make_unique<websocket::stream<stream_t>>(std::move(stream));
        auto* ws_stream_p = ws_stream.get();

        // Try to handshake with the WebSocket client
        ws_stream_p->async_accept(
            http_request,
            [ws_stream = std::move(ws_stream),
             self = this->shared_from_this(),
             write_authorized](const system::error_code& ec) {
                if (ec) {
                    ws_stream->close(websocket::close_code::abnormal);
                    logger.error() << "Could not perform the WebSocket handshake with the client";
                    return;
                }
                logger.debug() << "Dispatching StreamingWebSocketSession";

                std::make_shared<StreamingWebSocketSession<beast::websocket::stream<stream_t>>>(
                    self->server,
                    std::move(*ws_stream),
                    self->query_timeout,
                    write_authorized
                )
                    ->run();
            }
        );
        return;
    }

    // Handle regular HTTP requests
    logger.debug() << "Dispatching HTTPSession";
    if (server.model_id == Protocol::QUAD_MODEL_ID) {
        HttpQuadSession<stream_t>::run(
            std::make_unique<HttpQuadSession<stream_t>>(
                server,
                std::move(stream),
                std::move(http_request),
                query_timeout
            )
        );
    } else if (server.model_id == Protocol::RDF_MODEL_ID) {
        HttpRdfSession<stream_t>::run(
            std::make_unique<HttpRdfSession<stream_t>>(
                server,
                std::move(stream),
                std::move(http_request),
                query_timeout
            )
        );
    } else if (server.model_id == Protocol::GQL_MODEL_ID) {
        HttpGQLSession<stream_t>::run(
            std::make_unique<HttpGQLSession<stream_t>>(
                server,
                std::move(stream),
                std::move(http_request),
                query_timeout
            )
        );
    } else {
        throw std::runtime_error("Unhandled ModelId: " + std::to_string(server.model_id));
    }
}

template<typename stream_t>
void SessionDispatcher<stream_t>::close_stream(stream_t& stream)
{
    if constexpr (std::is_same_v<stream_t, asio::ip::tcp::socket>) {
        stream.close();
    } else if constexpr (std::is_same_v<stream_t, beast::ssl_stream<asio::ip::tcp::socket>>) {
        stream.next_layer().close();
    }
}

template<typename stream_t>
std::pair<std::string, std::string> SessionDispatcher<stream_t>::get_user_password(
    const beast::http::request<beast::http::string_body>& request
)
{
    std::string_view url = request.target();

    // Find the beginning of the query string
    size_t qm = url.find('?');
    if (qm == std::string_view::npos) {
        return { "", "" }; // No query
    }

    std::string_view query = url.substr(qm + 1);

    std::string user;
    std::string password;

    while (!query.empty()) {
        // Find next '&'
        size_t amp = query.find('&');
        auto pair = (amp == std::string_view::npos) ? query : query.substr(0, amp);

        // Move to next part
        if (amp != std::string_view::npos)
            query.remove_prefix(amp + 1);
        else
            query = {};

        // Split "key=value"
        size_t eq = pair.find('=');
        if (eq == std::string_view::npos)
            continue;

        std::string_view key = pair.substr(0, eq);
        std::string_view val = pair.substr(eq + 1);

        if (key == "user") {
            user = std::string(val);
        } else if (key == "password") {
            password = std::string(val);
        }
    }

    return { user, password };
}

void SessionSSLDetector::run()
{
    tcp_stream.async_receive(
        asio::buffer(&peek_byte, 1),
        asio::socket_base::message_peek,
        [self = shared_from_this()](const system::error_code& ec, std::size_t bytes_transferred) {
            if (ec || bytes_transferred == 0) {
                logger.error() << "Could not read the client's preamble";
                self->tcp_stream.close();
                return;
            }

            const bool is_tls = self->peek_byte == 0x16;
            if (is_tls) {
                logger.debug() << "SSL detected.";

                if (!self->ssl_ctx.has_value()) {
                    logger.error() << "The server is not configured to accept SSL connections.";
                    self->tcp_stream.close();
                    return;
                }

                self->ssl_stream = std::make_unique<beast::ssl_stream<asio::ip::tcp::socket>>(
                    std::move(self->tcp_stream),
                    self->ssl_ctx.value()
                );

                self->ssl_stream->async_handshake(
                    asio::ssl::stream_base::server,
                    [self = self->shared_from_this()](beast::error_code ec /*, std::size_t bytes_used*/) {
                        if (ec) {
                            logger.error() << "Error in ssl handshake: " << ec.message();
                            return;
                        }

                        std::make_shared<SessionDispatcher<beast::ssl_stream<asio::ip::tcp::socket>>>(
                            self->server,
                            std::move(*self->ssl_stream),
                            self->query_timeout
                        )
                            ->run();
                    }
                );
            } else {
                logger.debug() << "SSL not detected.";
                std::make_shared<SessionDispatcher<asio::ip::tcp::socket>>(
                    self->server,
                    std::move(self->tcp_stream),
                    self->query_timeout
                )
                    ->run();
            }
        }
    );
}

template class MDBServer::SessionDispatcher<asio::ip::tcp::socket>;
template class MDBServer::SessionDispatcher<beast::ssl_stream<asio::ip::tcp::socket>>;
