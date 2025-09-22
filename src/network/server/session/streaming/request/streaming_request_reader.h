#pragma once

#include <cstdint>
#include <string>

#include "network/server/protocol.h"
#include "query/query_context.h"

namespace MDBServer {

class StreamingRequestReader {
public:
    StreamingRequestReader() :
        request_bytes(nullptr),
        request_size(0),
        current_pos(0)
    { }

    // Set a new request for reading
    void set_request(const uint8_t* request_bytes, const size_t request_size);

    Protocol::RequestType read_request_type();

    std::map<std::string, ObjectId> read_parameters()
    {
        check_datatype(Protocol::DataType::MAP);
        const uint_fast32_t size = read_size();
        std::map<std::string, ObjectId> res;
        for (std::size_t i = 0; i < size; ++i) {
            const auto var_name =  read_string();
            const auto object_id = read_object();
            res.emplace(std::move(var_name), object_id);
        }
        return res;
    }

    ObjectId read_object()
    {
        // TODO: Should be per-model, this is just a test
        // TODO: handle more types
        // check_remaining_bytes(size);
        const auto type = static_cast<Protocol::DataType>(request_bytes[current_pos++]);
        switch (type) {
        case Protocol::DataType::NULL_:
            return ObjectId::get_null();
        case Protocol::DataType::BOOL_FALSE:
            return ObjectId(ObjectId::BOOL_FALSE);
        case Protocol::DataType::BOOL_TRUE:
            return ObjectId(ObjectId::BOOL_TRUE);
        // case Protocol::DataType::STRING:
        // const auto str = read_string();
        // pack-per-model
        default:
            throw std::logic_error(
                "Unmanaged datatype in StreamingQuadResponseWriter::encode_object_id: "
                + std::to_string(static_cast<uint8_t>(type))
            );
        };
    }

    uint8_t read_uint8();
    uint32_t read_uint32();
    std::string read_string();

private:
    const uint8_t* request_bytes;
    uint_fast32_t request_size;
    uint_fast32_t current_pos;

    // Read the size value for variable-sized data types
    uint_fast32_t read_size();

    // Validate the data type at the current cursor position
    void check_datatype(Protocol::DataType expected);

    // Validate the remaining number of bytes is at least num_bytes
    void check_remaining_bytes(uint_fast32_t num_bytes) const;
};
} // namespace MDBServer
