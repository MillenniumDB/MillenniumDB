#include "tcp_buffer.h"

#include <cassert>

#include "base/exceptions.h"

using boost::asio::ip::tcp;

TcpBuffer::TcpBuffer(tcp::socket& sock) :
    sock (sock)
{
    buffer[0] = static_cast<unsigned char>(db_server::MessageType::not_end);
    // skip first byte for type and 2 bytes for length
    current_pos = 3;
}


TcpBuffer::~TcpBuffer() {
    if (sock.is_open()) {
        if (error) {
            buffer[0] = static_cast<unsigned char>(db_server::MessageType::end_fail);
        } else {
            buffer[0] = static_cast<unsigned char>(db_server::MessageType::end_success);
        }
        send();
    }
}


int TcpBuffer::overflow(int i) {
    char c = static_cast<char>(i);
    assert(current_pos < db_server::BUFFER_SIZE);
    buffer[current_pos] = c;
    ++current_pos;
    if (current_pos == db_server::BUFFER_SIZE) {
        send(); // send() will reset current_pos
    }
    return i;
}


void TcpBuffer::set_error(db_server::ErrorCode error_code) {
    buffer[1] = static_cast<unsigned char>(error_code);
    error = true;
}


int TcpBuffer::sync() {
    send();
    return 0;
}


void TcpBuffer::send() {
    unsigned char b1 = current_pos & 0xFF;
    unsigned char b2 = (current_pos >> 8) & 0xFF;

    buffer[1] = b1;
    buffer[2] = b2;

    boost::system::error_code ec;
    boost::asio::write(sock, boost::asio::buffer(buffer, db_server::BUFFER_SIZE), ec);
    current_pos = 3;
    if (ec) {
        sock.close();
        throw ConnectionException(ec.message());
    }
}
