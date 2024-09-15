#pragma once

#include <cstdint>
#include <string>

#include "network/server/protocol.h"

namespace MDBServer {

class StreamingRequestReader {
public:
    StreamingRequestReader() : request_bytes(nullptr), request_size(0), current_pos(0) { }

    // Set a new request for reading
    void set_request(const uint8_t* request_bytes, const size_t request_size);

    Protocol::RequestType read_request_type();
    uint8_t               read_uint8();
    uint32_t              read_uint32();
    std::string           read_string();

private:
    const uint8_t* request_bytes;
    uint_fast32_t  request_size;
    uint_fast32_t  current_pos;

    // Read the size value for variable-sized data types
    uint_fast32_t read_size();

    // Validate the data type at the current cursor position
    void check_datatype(Protocol::DataType expected);

    // Validate the remaining number of bytes is at least num_bytes
    void check_remaining_bytes(uint_fast32_t num_bytes) const;
};
} // namespace MDBServer
