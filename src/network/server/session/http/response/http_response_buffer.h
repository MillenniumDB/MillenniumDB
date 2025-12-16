#pragma once

#include <cstring>
#include <streambuf>

#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

#include "network/exceptions.h"
#include "network/server/protocol.h"

namespace MDBServer {

template<typename stream_t>
class HttpResponseBuffer : public std::streambuf {
public:
    explicit HttpResponseBuffer(stream_t& stream_) :
        stream(stream_),
        current_pos(0)
    { }

    ~HttpResponseBuffer()
    {
        if constexpr (std::is_same_v<stream_t, boost::asio::ip::tcp::socket>) {
            if (stream.is_open()) {
                flush();
                stream.close();
            }
        } else if constexpr (std::is_same_v<stream_t, boost::beast::ssl_stream<boost::asio::ip::tcp::socket>>)
        {
            if (stream.next_layer().is_open()) {
                flush();
                stream.next_layer().close();
            }
        }
    }

protected:
    int overflow(int c) override
    {
        ensure_write_space();

        buffer[current_pos++] = static_cast<uint8_t>(c);
        return c;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
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

    int sync() override
    {
        flush();
        return 0;
    }

private:
    stream_t& stream;

    uint_fast32_t current_pos;

    uint8_t buffer[Protocol::BUFFER_SIZE];

    inline void flush()
    {
        boost::system::error_code ec;
        boost::asio::write(stream, boost::asio::buffer(buffer, current_pos), ec);
        current_pos = 0;
        if (ec) {
            if constexpr (std::is_same_v<stream_t, boost::asio::ip::tcp::socket>) {
                stream.close();
            } else if constexpr (std::is_same_v<
                                     stream_t,
                                     boost::beast::ssl_stream<boost::asio::ip::tcp::socket>>)
            {
                stream.next_layer().close();
            }
            throw ConnectionException(ec.message());
        }
    }

    // Ensure that there is enough space to write something
    inline void ensure_write_space()
    {
        if (remaining_space() == 0) {
            flush();
        }
    }

    // Return the number of available bytes for writing in the buffer
    inline uint_fast32_t remaining_space() const
    {
        return sizeof(buffer) - current_pos;
    }
};
} // namespace MDBServer
