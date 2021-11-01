#ifndef SERVER__TCP_BUFFER_H_
#define SERVER__TCP_BUFFER_H_

#include <string>
#include <boost/asio.hpp>

#include "server/server.h"

class TcpBuffer : public std::stringbuf {
public:
    TcpBuffer(boost::asio::ip::tcp::socket& sock);
    ~TcpBuffer();

    void set_error(db_server::ErrorCode error_code);

protected:
    int overflow(int c) override;
    int sync() override;

private:
    int current_pos;
    bool error = false;
    unsigned char buffer[db_server::BUFFER_SIZE];
    boost::asio::ip::tcp::socket& sock;
    void send();
};

#endif // SERVER__TCP_BUFFER_H_
