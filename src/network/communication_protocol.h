#pragma once

/*
* There are 2 types of communication:
* 1.- Client sending the query.
* 2.- Server sending the query output or an error.
*
* When the client sends the query, 2 messages are sent:
* - First it sends the query length in a short message (`BYTES_FOR_QUERY_LENGTH` bytes).
*   We are supposing the query doesn't need more than 2^(8*BYTES_FOR_QUERY_LENGTH) bytes.
* - Then it sends a message containing the query.
* This way, the server knows exactly how many bytes it should read.
*
* When the server is sending the output (or error), the communication is divided into
* (potentially many) messages of size `BUFFER_SIZE`. Each of this messages will have a
* header of 3 bytes. The first byte have the status and a bit indicating if that is the
* last message or there are more messages comming. The next 2 bytes tells how long is
* the message.
*/

namespace CommunicationProtocol {
    constexpr unsigned char end_mask    = 0x80; // 1000'0000
    constexpr unsigned char status_mask = 0x7F; // 0111'1111

    enum class StatusCodes : unsigned char {
        success          = 0x00,

        // Something is wrong with the query received
        query_error      = 0x01,

        // Something went wrong in the communication between client adn server
        connection_error = 0x02,

        // Something is wrong with the implementation or the data is corrupted
        logic_error      = 0x03,

        // Query execution took too long
        timeout          = 0x04,

        // Some exception not propertly catched
        unexpected_error = 0x05
    };

    static constexpr bool last_message(unsigned char status) noexcept {
        return (status & end_mask) != 0;
    }

    static constexpr unsigned char decode_status(unsigned char status) noexcept {
        return status & status_mask;
    }

    constexpr auto DEFAULT_PORT = 8080;
    constexpr auto BUFFER_SIZE = 4096; // MUST BE LESS THAN 65536 (2^16)
    constexpr auto BYTES_FOR_QUERY_LENGTH = 4;
}
