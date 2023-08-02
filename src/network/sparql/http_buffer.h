#pragma once

#include <string>

#include <boost/asio.hpp>

#include "network/exceptions.h"
#include "network/mql/communication_protocol.h"

class HttpBuffer : public std::stringbuf {
public:
    HttpBuffer(boost::asio::ip::tcp::socket& sock) :
        sock (sock)
    {
        current_pos = 0;
    }

    void finish() {
        if (sock.is_open() ) {
            boost::system::error_code ec;
            if (current_pos > 0) {
                boost::asio::write(sock, boost::asio::buffer(buffer, current_pos), ec);
            }
            if (ec) {
                sock.close();
                throw ConnectionException(ec.message());
            }
            sock.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        }
    }

protected:
    int overflow(int i) override {
        char c = static_cast<char>(i);
        buffer[current_pos] = c;
        ++current_pos;
        if (current_pos == CommunicationProtocol::BUFFER_SIZE) {
            send(); // send() will reset current_pos
        }
        return i;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override {
        int remaining = n;

        while (remaining > 0) {
            auto max = CommunicationProtocol::BUFFER_SIZE - current_pos;
            if (remaining < max) {
                memcpy(&buffer[current_pos], s, remaining);
                current_pos += remaining;
                break;
            } else {
                memcpy(&buffer[current_pos], s, max);
                current_pos += max;
                s += max;
                remaining -= max;
                send(); // send() will reset current_pos
            }
        }
        return n;
    }

    int sync() override {
        send();
        return 0;
    }

private:
    int current_pos;

    unsigned char buffer[CommunicationProtocol::BUFFER_SIZE];

    boost::asio::ip::tcp::socket& sock;

    void send() {
        boost::system::error_code ec;
        boost::asio::write(sock, boost::asio::buffer(buffer, CommunicationProtocol::BUFFER_SIZE), ec);

        current_pos = 0;
        if (ec) {
            sock.close();
            throw ConnectionException(ec.message());
        }
    }
};
