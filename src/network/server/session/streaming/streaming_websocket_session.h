#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>

#include "network/server/session/streaming/request/streaming_request_handler.h"
#include "network/server/session/streaming/streaming_session.h"

namespace MDBServer {

class Server;

class StreamingWebSocketSession : public StreamingSession {
    using websocket_stream_type = boost::beast::websocket::stream<boost::beast::tcp_stream>;

public:
    explicit StreamingWebSocketSession(Server&                 server,
                                       websocket_stream_type&& stream,
                                       std::chrono::seconds    query_timeout);

    ~StreamingWebSocketSession();

    static void run(std::unique_ptr<StreamingWebSocketSession>);

    void write(const uint8_t* bytes, std::size_t num_bytes) override;

    std::mutex& get_thread_info_vec_mutex() override;

    std::chrono::seconds get_timeout() override;

    bool try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token) override;

private:
    boost::beast::error_code ec;

    std::chrono::seconds query_timeout;

    websocket_stream_type stream;

    boost::beast::flat_buffer request_buffer;

    std::unique_ptr<StreamingRequestHandler> request_handler;
};
} // namespace MDBServer
