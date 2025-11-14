#include "streaming_response_writer.h"

#include "query/query_context.h"
#include "system/path_manager.h"

using namespace MDBServer;

void StreamingResponseWriter::flush()
{
    response_ostream.flush();
}

void StreamingResponseWriter::write_variables(
    const std::vector<VarId>& projection_vars,
    uint_fast32_t worker_idx,
    const std::string& cancellation_token
)
{
    write_map_header(2UL);
    write_typed_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::VARIABLES));

    write_typed_string("payload", Protocol::DataType::STRING);
    write_map_header(2UL);
    write_typed_string("variables", Protocol::DataType::STRING);
    write_list_header(projection_vars.size());
    for (const auto& var_id : projection_vars) {
        const auto var_name = get_query_ctx().get_var_name(var_id);
        write_typed_string(var_name, Protocol::DataType::STRING);
    }
    write_typed_string("queryPreamble", Protocol::DataType::STRING);
    write_map_header(2UL);
    write_typed_string("workerIndex", Protocol::DataType::STRING);
    write_uint32(worker_idx);
    write_typed_string("cancellationToken", Protocol::DataType::STRING);
    write_typed_string(cancellation_token, Protocol::DataType::STRING);

    seal();
}

void StreamingResponseWriter::write_records_success(
    uint64_t result_count,
    double parser_duration_ms,
    double optimizer_duration_ms,
    double execution_duration_ms
)
{
    write_map_header(2UL);
    write_typed_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_typed_string("payload", Protocol::DataType::STRING);
    write_map_header(5UL);
    write_typed_string("update", Protocol::DataType::STRING);
    write_bool(false);
    write_typed_string("resultCount", Protocol::DataType::STRING);
    write_uint64(result_count);
    write_typed_string("parserDurationMs", Protocol::DataType::STRING);
    write_double(parser_duration_ms);
    write_typed_string("optimizerDurationMs", Protocol::DataType::STRING);
    write_double(optimizer_duration_ms);
    write_typed_string("executionDurationMs", Protocol::DataType::STRING);
    write_double(execution_duration_ms);

    seal();
}

void StreamingResponseWriter::write_update_success(
    double parser_duration_ms,
    double optimizer_duration_ms,
    double execution_duration_ms
)
{
    write_map_header(2UL);
    write_typed_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_typed_string("payload", Protocol::DataType::STRING);
    write_map_header(4UL);

    write_typed_string("update", Protocol::DataType::STRING);
    write_bool(true);
    write_typed_string("parserDurationMs", Protocol::DataType::STRING);
    write_double(parser_duration_ms);
    write_typed_string("optimizerDurationMs", Protocol::DataType::STRING);
    write_double(optimizer_duration_ms);
    write_typed_string("executionDurationMs", Protocol::DataType::STRING);
    write_double(execution_duration_ms);

    // TODO: Send update data

    seal();
}

void StreamingResponseWriter::write_catalog_success()
{
    write_map_header(2UL);
    write_typed_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_typed_string("payload", Protocol::DataType::STRING);
    write_map_header(3UL);
    write_typed_string("modelId", Protocol::DataType::STRING);
    write_uint64(get_model_id());
    write_typed_string("version", Protocol::DataType::STRING);
    write_uint64(get_catalog_version());

    // // TODO: Pass useful additional metadata about the catalog and/or database
    write_typed_string("metadata", Protocol::DataType::STRING);
    write_null();

    seal();
}

void StreamingResponseWriter::write_cancel_success()
{
    write_map_header(2UL);
    write_typed_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_typed_string("payload", Protocol::DataType::STRING);
    write_null();

    seal();
}

void StreamingResponseWriter::write_record(const std::vector<VarId>& projection_vars, const Binding& binding)
{
    write_map_header(2UL);
    write_typed_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::RECORD));

    write_typed_string("payload",Protocol::DataType::STRING);
    write_list_header(projection_vars.size());
    for (const auto& var : projection_vars) {
        write_object_id(binding[var]);
    }

    seal();
}

void StreamingResponseWriter::write_error(const std::string& message)
{
    write_map_header(2UL);
    write_typed_string("type",Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::ERROR));

    write_typed_string("payload", Protocol::DataType::STRING);
    write_typed_string(message, Protocol::DataType::STRING);

    seal();
}

void StreamingResponseWriter::write_null()
{
    response_ostream.put(static_cast<char>(Protocol::DataType::NULL_));
}

void StreamingResponseWriter::write_bool(bool value)
{
    response_ostream.put(
        static_cast<char>(value ? Protocol::DataType::BOOL_TRUE : Protocol::DataType::BOOL_FALSE)
    );
}

void StreamingResponseWriter::write_uint8(uint8_t value)
{
    uint8_t bytes[2];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::UINT8);
    bytes[1] = static_cast<uint8_t>(value);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_float(float value)
{
    static_assert(sizeof(float) == sizeof(uint32_t));

    uint32_t u;
    std::memcpy(&u, &value, sizeof(float));

    uint8_t bytes[5];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::FLOAT);
    bytes[1] = static_cast<uint8_t>((u >> 24) & 0xFF);
    bytes[2] = static_cast<uint8_t>((u >> 16) & 0xFF);
    bytes[3] = static_cast<uint8_t>((u >> 8) & 0xFF);
    bytes[4] = static_cast<uint8_t>(u & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_float_raw(float value)
{
    static_assert(sizeof(float) == sizeof(uint32_t));

    uint32_t u;
    std::memcpy(&u, &value, sizeof(float));

    uint8_t bytes[4];
    bytes[0] = static_cast<uint8_t>((u >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((u >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((u >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(u & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_double(double value)
{
    static_assert(sizeof(double) == sizeof(uint64_t));

    uint64_t u;
    std::memcpy(&u, &value, sizeof(double));

    uint8_t bytes[9];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::DOUBLE);
    bytes[1] = static_cast<uint8_t>((u >> 56) & 0xFF);
    bytes[2] = static_cast<uint8_t>((u >> 48) & 0xFF);
    bytes[3] = static_cast<uint8_t>((u >> 40) & 0xFF);
    bytes[4] = static_cast<uint8_t>((u >> 32) & 0xFF);
    bytes[5] = static_cast<uint8_t>((u >> 24) & 0xFF);
    bytes[6] = static_cast<uint8_t>((u >> 16) & 0xFF);
    bytes[7] = static_cast<uint8_t>((u >> 8) & 0xFF);
    bytes[8] = static_cast<uint8_t>(u & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_double_raw(double value)
{
     static_assert(sizeof(double) == sizeof(uint64_t));

    uint64_t u;
    std::memcpy(&u, &value, sizeof(double));

    uint8_t bytes[8];
    bytes[0] = static_cast<uint8_t>((u >> 56) & 0xFF);
    bytes[1] = static_cast<uint8_t>((u >> 48) & 0xFF);
    bytes[2] = static_cast<uint8_t>((u >> 40) & 0xFF);
    bytes[3] = static_cast<uint8_t>((u >> 32) & 0xFF);
    bytes[4] = static_cast<uint8_t>((u >> 24) & 0xFF);
    bytes[5] = static_cast<uint8_t>((u >> 16) & 0xFF);
    bytes[6] = static_cast<uint8_t>((u >> 8) & 0xFF);
    bytes[7] = static_cast<uint8_t>(u & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_string_raw(const std::string& value)
{
    write_size(value.size());
    response_ostream.write(value.data(), value.size());
}

void StreamingResponseWriter::write_uint32(uint32_t value)
{
    uint8_t bytes[5];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::UINT32);
    bytes[1] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[3] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[4] = static_cast<uint8_t>(value & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_uint64(uint64_t value)
{
    uint8_t bytes[9];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::UINT64);
    bytes[1] = static_cast<uint8_t>((value >> 56) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 48) & 0xFF);
    bytes[3] = static_cast<uint8_t>((value >> 40) & 0xFF);
    bytes[4] = static_cast<uint8_t>((value >> 32) & 0xFF);
    bytes[5] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[6] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[7] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[8] = static_cast<uint8_t>(value & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_int64(int64_t value)
{
    uint8_t bytes[9];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::INT64);
    bytes[1] = static_cast<uint8_t>((value >> 56) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 48) & 0xFF);
    bytes[3] = static_cast<uint8_t>((value >> 40) & 0xFF);
    bytes[4] = static_cast<uint8_t>((value >> 32) & 0xFF);
    bytes[5] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[6] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[7] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[8] = static_cast<uint8_t>(value & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_typed_string(const std::string& value, Protocol::DataType datatype)
{
    response_ostream.put(static_cast<char>(datatype));
    write_size(value.size());
    response_ostream.write(value.data(), value.size());
}

void StreamingResponseWriter::write_path(uint64_t path_id)
{
    using namespace std::placeholders;

    std::vector<ObjectId> nodes;
    std::vector<std::pair<ObjectId, bool>> edges;
    path_manager.for_each(
        path_id,
        [&](ObjectId oid) { nodes.emplace_back(oid); },
        [&](ObjectId oid, bool reverse) { edges.emplace_back(oid, reverse); }
    );
    assert(nodes.size() == edges.size() + 1);

    response_ostream.put(static_cast<char>(Protocol::DataType::PATH));
    write_size(edges.size());

    auto nodes_it = nodes.begin();
    auto edges_it = edges.begin();

    write_object_id(*nodes_it);
    ++nodes_it;

    while (edges_it != edges.end()) {
        write_path_edge(edges_it->first, edges_it->second);
        write_object_id(*nodes_it);
        ++edges_it;
        ++nodes_it;
    }
}

void StreamingResponseWriter::write_date(DateTime datetime)
{
    bool error;
    const int64_t year = datetime.get_year(&error);
    const int64_t month = datetime.get_month(&error);
    const int64_t day = datetime.get_day(&error);
    const int64_t tz_min_offset = datetime.get_tz_min_offset(&error);

    response_ostream.put(static_cast<char>(Protocol::DataType::DATE));
    write_int64_raw(year);
    write_int64_raw(month);
    write_int64_raw(day);
    write_int64_raw(error ? 0 : tz_min_offset);
}

void StreamingResponseWriter::write_time(DateTime datetime)
{
    bool error;
    const int64_t hour = datetime.get_hour(&error);
    const int64_t minute = datetime.get_minute(&error);
    const int64_t second = datetime.get_second(&error);
    const int64_t tz_min_offset = datetime.get_tz_min_offset(&error);

    response_ostream.put(static_cast<char>(Protocol::DataType::TIME));
    write_int64_raw(hour);
    write_int64_raw(minute);
    write_int64_raw(second);
    write_int64_raw(error ? 0 : tz_min_offset);
}

void StreamingResponseWriter::write_datetime(DateTime datetime)
{
    bool error;
    const int64_t year = datetime.get_year(&error);
    const int64_t month = datetime.get_month(&error);
    const int64_t day = datetime.get_day(&error);
    const int64_t hour = datetime.get_hour(&error);
    const int64_t minute = datetime.get_minute(&error);
    const int64_t second = datetime.get_second(&error);
    const int64_t tz_min_offset = datetime.get_tz_min_offset(&error);

    response_ostream.put(static_cast<char>(Protocol::DataType::DATETIME));
    write_int64_raw(year);
    write_int64_raw(month);
    write_int64_raw(day);
    write_int64_raw(hour);
    write_int64_raw(minute);
    write_int64_raw(second);
    write_int64_raw(error ? 0 : tz_min_offset);
}

template<typename T>
void StreamingResponseWriter::write_tensor(const tensor::Tensor<T>& tensor)
{
    response_ostream.put(static_cast<char>(Protocol::DataType::TENSOR));

    if constexpr (std::is_same_v<T, float>) {
        response_ostream.put(static_cast<char>(Protocol::DataType::FLOAT));
    } else if constexpr (std::is_same_v<T, double>) {
        response_ostream.put(static_cast<char>(Protocol::DataType::DOUBLE));
    }

    write_size(tensor.size());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        if constexpr (std::is_same_v<T, float>) {
            write_float_raw(tensor[i]);
        } else if constexpr (std::is_same_v<T, double>) {
            write_double_raw(tensor[i]);
        }
    }
}

void StreamingResponseWriter::write_list(const std::vector<ObjectId>& list)
{
    response_ostream.put(static_cast<char>(Protocol::DataType::LIST));
    write_size(list.size());
    for (const auto& oid : list) {
        write_object_id(oid);
    }
}

void StreamingResponseWriter::write_size(uint32_t value)
{
    uint8_t bytes[4];
    bytes[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(value & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::write_int64_raw(int64_t value)
{
    uint8_t bytes[8];
    bytes[0] = static_cast<uint8_t>((value >> 56) & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 48) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 40) & 0xFF);
    bytes[3] = static_cast<uint8_t>((value >> 32) & 0xFF);
    bytes[4] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[5] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[6] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[7] = static_cast<uint8_t>(value & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::seal()
{
    response_buffer.seal();
}

void StreamingResponseWriter::get_path_node(ObjectId oid, std::vector<ObjectId>& nodes)
{
    nodes.emplace_back(oid);
}

void StreamingResponseWriter::get_path_edge(
    ObjectId oid,
    bool reverse,
    std::vector<std::pair<ObjectId, bool>>& edges
)
{
    edges.emplace_back(oid, reverse);
}

void StreamingResponseWriter::write_path_edge(ObjectId oid, bool reverse)
{
    write_string_raw(reverse ? "left" : "right");
    write_object_id(oid);
}

void StreamingResponseWriter::write_dictionary(const Dictionary& dictionary)
{
    if (auto literal = dynamic_cast<DictionaryLiteral*>(dictionary.dictionary.get())) {
        write_object_id(literal->object_id);
    } else if (auto array = dynamic_cast<DictionaryArray*>(dictionary.dictionary.get())) {
        write_dictionary_array(*array);
    } else if (auto object = dynamic_cast<DictionaryObject*>(dictionary.dictionary.get())) {
        write_dictionary_object(*object);
    } else {
        write_null(); // should throw error instead?
    }
}

void StreamingResponseWriter::write_dictionary_object(const DictionaryObject& dictionary)
{
    response_ostream.put(static_cast<char>(Protocol::DataType::MAP));
    write_size(dictionary.keys.size());

    for (const auto& [key, value] : dictionary.keys) {
        write_dictionary_key(key);

        if (auto literal = dynamic_cast<DictionaryLiteral*>(value.get())) {
            write_object_id(literal->object_id);
        } else if (auto array = dynamic_cast<DictionaryArray*>(value.get())) {
            write_dictionary_array(*array);
        } else if (auto object = dynamic_cast<DictionaryObject*>(value.get())) {
            write_dictionary_object(*object);
        }
    }
}

void StreamingResponseWriter::write_dictionary_array(const DictionaryArray& dictionary_array)
{
    response_ostream.put(static_cast<char>(Protocol::DataType::LIST));
    write_size(dictionary_array.values.size());

    for (auto& elem : dictionary_array.values) {
        if (auto literal = dynamic_cast<DictionaryLiteral*>(elem.get())) {
            write_object_id(literal->object_id);
        } else if (auto array = dynamic_cast<DictionaryArray*>(elem.get())) {
            write_dictionary_array(*array);
        } else if (auto object = dynamic_cast<DictionaryObject*>(elem.get())) {
            write_dictionary_object(*object);
        }
    }
}

void StreamingResponseWriter::write_edge(uint64_t edge_id)
{
    response_ostream.put(static_cast<char>(Protocol::DataType::EDGE));
    write_int64_raw(edge_id);
}


void StreamingResponseWriter::write_anon(uint64_t anon_id)
{
    response_ostream.put(static_cast<char>(Protocol::DataType::ANON));
    write_int64_raw(anon_id);
}

template void StreamingResponseWriter::write_tensor<float>(const tensor::Tensor<float>& tensor);
template void StreamingResponseWriter::write_tensor<double>(const tensor::Tensor<double>& tensor);
