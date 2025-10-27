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

std::map<std::string, ObjectId> StreamingRequestReader::read_parameters()
{
    check_datatype(Protocol::DataType::MAP);
    const uint_fast32_t size = read_size();
    std::map<std::string, ObjectId> res;
    for (std::size_t i = 0; i < size; ++i) {
        check_datatype(Protocol::DataType::STRING);
        const auto var_name = read_string();
        const auto object_id = read_object();
        res.emplace(std::move(var_name), object_id);
    }
    return res;
}

uint32_t StreamingRequestReader::read_uint32()
{
    check_remaining_bytes(4);
    uint32_t value = request_bytes[current_pos++] << 24;
    value |= request_bytes[current_pos++] << 16;
    value |= request_bytes[current_pos++] << 8;
    value |= request_bytes[current_pos++];
    return value;
}

std::string StreamingRequestReader::read_string()
{
    uint_fast32_t size = read_size();
    check_remaining_bytes(size);
    std::string result(reinterpret_cast<const char*>(&request_bytes[current_pos]), size);
    current_pos += size;
    return result;
}

int64_t StreamingRequestReader::read_int64()
{
    check_remaining_bytes(8);
    int64_t value = static_cast<int64_t>(request_bytes[current_pos++]) << 56;
    value |= static_cast<int64_t>(request_bytes[current_pos++]) << 48;
    value |= static_cast<int64_t>(request_bytes[current_pos++]) << 40;
    value |= static_cast<int64_t>(request_bytes[current_pos++]) << 32;
    value |= static_cast<int64_t>(request_bytes[current_pos++]) << 24;
    value |= static_cast<int64_t>(request_bytes[current_pos++]) << 16;
    value |= static_cast<int64_t>(request_bytes[current_pos++]) << 8;
    value |= static_cast<int64_t>(request_bytes[current_pos++]);
    return value;
}

float StreamingRequestReader::read_float()
{
    check_remaining_bytes(4);
    float value;
    auto value_bytes = reinterpret_cast<uint8_t*>(&value);
    value_bytes[3] = request_bytes[current_pos++];
    value_bytes[2] = request_bytes[current_pos++];
    value_bytes[1] = request_bytes[current_pos++];
    value_bytes[0] = request_bytes[current_pos++];
    return value;
}

uint_fast32_t StreamingRequestReader::read_size() {
    check_remaining_bytes(4);
    uint32_t value { 0 };
    value |= static_cast<uint32_t>(request_bytes[current_pos++]) << 24;
    value |= static_cast<uint32_t>(request_bytes[current_pos++]) << 16;
    value |= static_cast<uint32_t>(request_bytes[current_pos++]) << 8;
    value |= static_cast<uint32_t>(request_bytes[current_pos++]);
    return value;
}


Protocol::DataType StreamingRequestReader::read_datatype() {
    check_remaining_bytes(1);
    return static_cast<Protocol::DataType>(request_bytes[current_pos++]);
}


void StreamingRequestReader::check_datatype(Protocol::DataType expected) {
    const auto actual = read_datatype();
    if (expected != actual) {
        const auto expected_str = Protocol::datatype_to_string(expected);
        const auto actual_str   = Protocol::datatype_to_string(actual);
        throw ProtocolException("Unexpected DataType: Expected " + expected_str + " but got " + actual_str);
    }
}


void StreamingRequestReader::check_remaining_bytes(uint_fast32_t expected) const {
    if (current_pos + expected > request_size) {
        throw ProtocolException("Not enough data in the request: Request is incomplete");
    }
}
