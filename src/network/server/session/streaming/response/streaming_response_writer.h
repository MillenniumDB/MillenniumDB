#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/common/datatypes/tensor/tensor.h"
#include "graph_models/object_id.h"
#include "network/server/protocol.h"
#include "network/server/session/streaming/response/streaming_response_buffer.h"
#include "query/executor/binding.h"
#include "query/var_id.h"
#include "storage/dictionary/dictionary.h"

namespace MDBServer {

/**
 * Response Writer is the class that exposes the methods for writing data to the client. Derived classes must implement
 * the ObjectId encoding for its specific data model.
 */
class StreamingResponseWriter {
public:
    // Write any object id to the response_ostream
    virtual void write_object_id(const ObjectId& oid) = 0;

    virtual uint64_t get_model_id() const = 0;

    virtual uint64_t get_catalog_version() const = 0;

    StreamingResponseWriter(StreamingSession& session_) :
        response_buffer { session_ },
        response_ostream { &response_buffer }
    { }

    virtual ~StreamingResponseWriter() = default;

    // Force the buffer to be flushed to the stream
    void flush();

    // Primitive types encoding
    void write_null();
    void write_bool(bool value);
    void write_uint8(uint8_t value);
    void write_float(float value);
    void write_double(double value);
    void write_uint32(uint32_t value);
    void write_uint64(uint64_t value);
    void write_int64(int64_t value);
    void write_typed_string(const std::string& value, Protocol::DataType datatype);
    void write_path(uint64_t path_id);
    void write_date(DateTime datetime);
    void write_time(DateTime datetime);
    void write_datetime(DateTime datetime);
    void write_dictionary(const Dictionary& dictionary);
    void write_dictionary_object(const DictionaryObject& dictionary);
    void write_dictionary_array(const DictionaryArray& dictionary);
    void write_edge(uint64_t edge_id);
    void write_anon(uint64_t anon_id);
    template<typename T>
    void write_tensor(const tensor::Tensor<T>& tensor);
    void write_list(const std::vector<ObjectId>& list);

    virtual void write_dictionary_key(const ObjectId&) {
        assert(false); // should be overriden if used
    }

    // utilities for composed types
    void write_int64_raw(int64_t value);
    void write_float_raw(float value);
    void write_double_raw(double value);
    void write_string_raw(const std::string& value);
    void write_size(uint32_t value);

    // Helpers writing responses
    void write_variables(
        const std::vector<VarId>& projection_vars,
        uint_fast32_t worker_idx,
        const std::string& cancellation_token
    );
    void write_records_success(
        uint64_t result_count,
        double parser_duration_ms,
        double optimizer_duration_ms,
        double execution_duration
    );
    void write_update_success(
        double parser_duration_ms,
        double optimizer_duration_ms,
        double execution_duration_ms
    );
    void write_catalog_success();
    void write_cancel_success();
    void write_record(const std::vector<VarId>& projection_vars, const Binding& binding);
    void write_error(const std::string& message);

    void write_map_header(uint32_t size)
    {
        response_ostream.put(static_cast<char>(Protocol::DataType::MAP));
        write_size(size);
    }

    void write_list_header(uint32_t size)
    {
        response_ostream.put(static_cast<char>(Protocol::DataType::LIST));
        write_size(size);
    }

    // Seals the buffer by calling ResponseBuffer::seal()
    void seal();

private:
    StreamingResponseBuffer response_buffer;

    // Path helpers to extract path data
    void get_path_node(ObjectId oid, std::vector<ObjectId>& nodes);
    void get_path_edge(ObjectId oid, bool reverse, std::vector<std::pair<ObjectId, bool>>& edges);
    void write_path_edge(ObjectId oid, bool reverse);

protected:
    std::ostream response_ostream;
};
} // namespace MDBServer
