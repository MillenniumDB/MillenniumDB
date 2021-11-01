#ifndef SERVER__SERVER_H_
#define SERVER__SERVER_H_

namespace db_server {
    enum class MessageType : unsigned char {
        not_end     = 0,
        end_success = 1,
        end_fail    = 2,
    };

    enum class ErrorCode : unsigned char {
        // success = 0
        query_error = 1,
        connection_error = 2,
        unexpected_error = 3,
        timeout = 4
    };

    constexpr auto DEFAULT_PORT = 8080;
    constexpr auto BUFFER_SIZE = 4096; // MUST BE LESS THAN 65536 (2^16)
    constexpr auto BYTES_FOR_QUERY_LENGTH = 4;
}

#endif // SERVER__SERVER_H_
