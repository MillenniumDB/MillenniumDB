#pragma once

#include "network/server/session/streaming/request/streaming_request_reader.h"
#include "query/exceptions.h"

namespace MDBServer {

// TODO:
class StreamingRdfRequestReader : public StreamingRequestReader {
public:
    ObjectId read_object() override
    {
        // const auto type = static_cast<Protocol::DataType>(read_uint8());
        const auto type = read_datatype(); // TODO:
        switch (type) {
        case Protocol::DataType::NULL_:
            return ObjectId::get_null();
        case Protocol::DataType::BOOL_FALSE:
            return ObjectId(ObjectId::BOOL_FALSE);
        case Protocol::DataType::BOOL_TRUE:
            return ObjectId(ObjectId::BOOL_TRUE);
        // case Protocol::DataType::STRING: {
        //     const auto str = read_string();
        //     return MQL::Conversions::pack_string(str);
        // }
        // case Protocol::DataType::INT64: {
        //     const auto i = read_int64();
        //     return MQL::Conversions::pack_int(i);
        // }
        // case Protocol::DataType::FLOAT: {
        //     const auto f = read_float();
        //     return MQL::Conversions::pack_float(f);
        // }
        default:
            throw QueryException(
                "Unsupported datatype received as parameter: " + Protocol::datatype_to_string(type)
            );
        };
    }
};

} // namespace MDBServer