#pragma once

#include <boost/asio.hpp>

#include "network/server/session/streaming/request/streaming_request_handler.h"
#include "network/server/session/streaming/streaming_session.h"

namespace MDBServer {

class Server;

class StreamingTCPSession : public StreamingSession, public std::enable_shared_from_this<StreamingTCPSession> {

public:
    explicit StreamingTCPSession(Server& server,
                                 boost::asio::ip::tcp::socket&& socket,
                                 std::chrono::seconds query_timeout);

    ~StreamingTCPSession();

    void run();

    void write(const uint8_t* bytes, std::size_t num_bytes) override;

    std::mutex& get_thread_info_vec_mutex() override;

    std::chrono::seconds get_timeout() override;

    bool try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token) override;

private:
    boost::system::error_code ec;

    Server& server;

    std::chrono::seconds query_timeout;

    boost::asio::ip::tcp::socket socket;

    boost::asio::streambuf request_buffer;

    std::unique_ptr<StreamingRequestHandler> request_handler;

    void read_request(uint32_t request_size);
};
} // namespace MDBServer
