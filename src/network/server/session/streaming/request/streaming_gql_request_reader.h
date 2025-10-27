#pragma once

#include "network/server/session/streaming/request/streaming_request_reader.h"
#include "query/exceptions.h"

namespace MDBServer {

// TODO: support more types
class StreamingGQLRequestReader : public StreamingRequestReader {
public:
    ObjectId read_object() override
    {
        const auto type = read_datatype();
        switch (type) {
        case Protocol::DataType::NULL_:
            return ObjectId::get_null();
        case Protocol::DataType::BOOL_FALSE:
            return ObjectId(ObjectId::BOOL_FALSE);
        case Protocol::DataType::BOOL_TRUE:
            return ObjectId(ObjectId::BOOL_TRUE);
        default:
            throw QueryException(
                "Unsupported datatype received as parameter: " + Protocol::datatype_to_string(type)
            );
        };
    }
};

} // namespace MDBServer
