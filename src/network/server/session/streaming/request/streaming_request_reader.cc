#include "streaming_request_reader.h"

#include "network/exceptions.h"

using namespace MDBServer;

void StreamingRequestReader::set_request(const uint8_t* request_bytes_, const size_t request_size_) {
    request_bytes = request_bytes_;
    request_size  = request_size_;
    current_pos   = 0;
}


Protocol::RequestType StreamingRequestReader::read_request_type() {
    check_remaining_bytes(1);
    uint8_t value = request_bytes[current_pos++];
    if (value >= static_cast<uint8_t>(Protocol::RequestType::TOTAL)) {
        throw ProtocolException("Invalid request type: " + std::to_string(value));
    }
    return static_cast<Protocol::RequestType>(value);
}


uint8_t StreamingRequestReader::read_uint8() {
    check_datatype(Protocol::DataType::UINT8);
    check_remaining_bytes(1);
    uint8_t value = request_bytes[current_pos++];
    return value;
}


uint32_t StreamingRequestReader::read_uint32() {
    check_datatype(Protocol::DataType::UINT32);
    check_remaining_bytes(4);
    uint32_t value = 0;
    for (auto i = 0; i < 4; ++i) {
        value <<= 8;
        value |= request_bytes[current_pos++];
    }
    return value;
}


std::string StreamingRequestReader::read_string() {
    check_datatype(Protocol::DataType::STRING);
    uint_fast32_t size = read_size();
    check_remaining_bytes(size);
    std::string result(reinterpret_cast<const char*>(&request_bytes[current_pos]), size);
    current_pos += size;
    return result;
}


uint_fast32_t StreamingRequestReader::read_size() {
    check_remaining_bytes(4);
    uint32_t value = 0;
    for (auto i = 0; i < 4; ++i) {
        value <<= 8;
        value |= request_bytes[current_pos++];
    }
    return value;
}


void StreamingRequestReader::check_datatype(Protocol::DataType expected) {
    check_remaining_bytes(1);
    auto actual = static_cast<Protocol::DataType>(request_bytes[current_pos++]);
    if (expected != actual) {
        const auto expected_str = Protocol::datatype_to_string(expected);
        const auto actual_str   = Protocol::datatype_to_string(actual);
        throw ProtocolException("Unexpected DataType: Expected " + expected_str + " but got " + actual_str);
    }
}


void StreamingRequestReader::check_remaining_bytes(uint_fast32_t expected) const {
    if (current_pos + expected > request_size) {
        throw ProtocolException("Not enough data in the request");
    }
}
