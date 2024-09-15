#pragma once

#include <cstring>
#include <streambuf>

#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>

#include "network/server/protocol.h"
#include "network/server/session/streaming/streaming_session.h"

namespace MDBServer {

/**
 * Packet format:
 * [chunk_size][      chunk     ]
 * [ 2 bytes  ][chunk_size bytes]
 *
 * The seal that marks the end of a message is a "zero-sized chunk" [0x00 0x00]
 */

class StreamingResponseBuffer : public std::streambuf {
public:
    explicit StreamingResponseBuffer(StreamingSession& session_) :
        session(session_), current_pos(0), chunk_open(false), current_chunk_start_pos(0) { }

    // Write down the chunk seal [0x00 0x00]. It cannot be sent in two separate messages
    void seal() {
        if (chunk_open) {
            close_chunk();
        }

        if (remaining_space() < Protocol::CHUNK_HEADER_SIZE) {
            flush();
        }

        buffer[current_pos++] = 0x00;
        buffer[current_pos++] = 0x00;
    }

protected:
    int overflow(int c) override {
        ensure_write_space();

        buffer[current_pos++] = static_cast<uint8_t>(c);
        return c;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override {
        uint_fast32_t remaining_write = n;
        while (remaining_write > 0) {
            ensure_write_space();

            const auto max = remaining_space();
            if (remaining_write > max) {
                // Flush the data that fits in the buffer and continue with the rest
                memcpy(&buffer[current_pos], s, max);
                current_pos += max;
                s += max;
                remaining_write -= max;
                flush();
            } else {
                // All the data fits in the buffer
                memcpy(&buffer[current_pos], s, remaining_write);
                current_pos += remaining_write;
                break;
            }
        }
        return n;
    }

    int sync() override {
        flush();
        return 0;
    }

private:
    StreamingSession& session;

    uint_fast32_t current_pos;

    uint8_t buffer[Protocol::BUFFER_SIZE];

    bool chunk_open;

    uint_fast32_t current_chunk_start_pos;

    // Write the current buffer to the stream from the beginning to the current_pos
    void write(std::size_t num_bytes) {
        session.write(buffer, num_bytes);
    }

    inline void flush() {
        if (chunk_open) {
            close_chunk();
        }
        write(current_pos);
        current_pos = 0;
    }

    // Reserve the bytes for the chunk header and open the chunk
    inline void open_chunk() {
        current_chunk_start_pos = current_pos;
        current_pos += Protocol::CHUNK_HEADER_SIZE;
        chunk_open = true;
    }

    // Write down the chunk header and close the chunk
    inline void close_chunk() {
        auto chunk_size = current_pos - current_chunk_start_pos - Protocol::CHUNK_HEADER_SIZE;

        buffer[current_chunk_start_pos]     = static_cast<uint8_t>((chunk_size >> 8) & 0xFF);
        buffer[current_chunk_start_pos + 1] = static_cast<uint8_t>(chunk_size & 0xFF);

        chunk_open = false;
    }

    // Ensure that the chunk has enough space to write something, opening it if necessary
    inline void ensure_write_space() {
        const auto num_bytes = chunk_open ? 1U : Protocol::CHUNK_HEADER_SIZE + 1U;
        if (remaining_space() < num_bytes) {
            flush();
        }

        if (!chunk_open) {
            open_chunk();
        }
    }

    // Return the number of available bytes for writing in the buffer
    inline uint_fast32_t remaining_space() const {
        return sizeof(buffer) - current_pos;
    }
};
} // namespace MDBServer
