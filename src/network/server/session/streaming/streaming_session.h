#pragma once

#include <chrono>
#include <mutex>

#include "network/server/server.h"

namespace MDBServer {

/**
 * This class is meant to be overriden by any streaming protocol. For example, if in the future we would like to
 * implement raw TCP, this class should be inherited from. Then the other streaming classes should be able to be
 * reused.
 */
class StreamingSession {
public:
    StreamingSession(Server& server) :
        server(server)
    { }

    Server& server;

    virtual ~StreamingSession() = default;

    // Write bytes to the stream
    virtual void write(const uint8_t* bytes, std::size_t num_bytes) = 0;

    // Getters used in query execution
    virtual std::mutex& get_thread_info_vec_mutex() = 0;

    virtual std::chrono::seconds get_timeout() = 0;

    virtual bool try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token) = 0;
};
} // namespace MDBServer
