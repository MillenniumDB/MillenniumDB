#include "streaming_response_writer.h"

#include <sstream>

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
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::VARIABLES));

    write_string("payload", Protocol::DataType::STRING);
    write_map_header(2UL);
    write_string("variables", Protocol::DataType::STRING);
    write_list_header(projection_vars.size());
    for (const auto& var_id : projection_vars) {
        const auto var_name = get_query_ctx().get_var_name(var_id);
        write_string(var_name, Protocol::DataType::STRING);
    }
    write_string("queryPreamble", Protocol::DataType::STRING);
    write_map_header(2UL);
    write_string("workerIndex", Protocol::DataType::STRING);
    write_uint32(worker_idx);
    write_string("cancellationToken", Protocol::DataType::STRING);
    write_string(cancellation_token, Protocol::DataType::STRING);

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
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_string("payload", Protocol::DataType::STRING);
    write_map_header(5UL);
    write_string("update", Protocol::DataType::STRING);
    write_bool(false);
    write_string("resultCount", Protocol::DataType::STRING);
    write_uint64(result_count);
    write_string("parserDurationMs", Protocol::DataType::STRING);
    write_double(parser_duration_ms);
    write_string("optimizerDurationMs", Protocol::DataType::STRING);
    write_double(optimizer_duration_ms);
    write_string("executionDurationMs", Protocol::DataType::STRING);
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
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_string("payload", Protocol::DataType::STRING);
    write_map_header(4UL);

    write_string("update", Protocol::DataType::STRING);
    write_bool(true);
    write_string("parserDurationMs", Protocol::DataType::STRING);
    write_double(parser_duration_ms);
    write_string("optimizerDurationMs", Protocol::DataType::STRING);
    write_double(optimizer_duration_ms);
    write_string("executionDurationMs", Protocol::DataType::STRING);
    write_double(execution_duration_ms);

    // TODO: Send update data

    seal();
}

void StreamingResponseWriter::write_catalog_success()
{
    write_map_header(2UL);
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_string("payload", Protocol::DataType::STRING);
    write_map_header(3UL);
    write_string("modelId", Protocol::DataType::STRING);
    write_uint64(get_model_id());
    write_string("version", Protocol::DataType::STRING);
    write_uint64(get_catalog_version());

    // TODO: Pass useful additional metadata about the catalog and/or database
    write_string("metadata", Protocol::DataType::STRING);
    write_null();

    seal();
}

void StreamingResponseWriter::write_cancel_success()
{
    write_map_header(2UL);
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::SUCCESS));

    write_string("payload", Protocol::DataType::STRING);
    write_null();

    seal();
}

void StreamingResponseWriter::write_record(const std::vector<VarId>& projection_vars, const Binding& binding)
{
    write_map_header(2UL);
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::RECORD));

    write_string("payload", Protocol::DataType::STRING);
    write_list_header(projection_vars.size());
    for (const auto& var : projection_vars) {
        write_object_id(binding[var]);
    }

    seal();
}

void StreamingResponseWriter::write_error(const std::string& message)
{
    write_map_header(2UL);
    write_string("type", Protocol::DataType::STRING);
    write_uint8(static_cast<uint8_t>(Protocol::ResponseType::ERROR));

    write_string("payload", Protocol::DataType::STRING);
    write_string(message, Protocol::DataType::STRING);

    seal();
}

std::string StreamingResponseWriter::encode_null() const
{
    return std::string(1, static_cast<char>(Protocol::DataType::NULL_));
}

std::string StreamingResponseWriter::encode_bool(bool value) const
{
    return std::string(
        1,
        static_cast<char>(value ? Protocol::DataType::BOOL_TRUE : Protocol::DataType::BOOL_FALSE)
    );
}

std::string StreamingResponseWriter::encode_uint8(uint8_t value) const
{
    uint8_t bytes[2];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::UINT8);
    bytes[1] = static_cast<uint8_t>(value);
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string StreamingResponseWriter::encode_float(float value_) const
{
    auto* value = reinterpret_cast<uint32_t*>(&value_);
    uint8_t bytes[5];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::FLOAT);
    bytes[1] = static_cast<uint8_t>((*value >> 24) & 0xFF);
    bytes[2] = static_cast<uint8_t>((*value >> 16) & 0xFF);
    bytes[3] = static_cast<uint8_t>((*value >> 8) & 0xFF);
    bytes[4] = static_cast<uint8_t>(*value & 0xFF);
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string StreamingResponseWriter::encode_double(double value_) const
{
    auto* value = reinterpret_cast<uint64_t*>(&value_);
    uint8_t bytes[9];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::DOUBLE);
    bytes[1] = static_cast<uint8_t>((*value >> 56) & 0xFF);
    bytes[2] = static_cast<uint8_t>((*value >> 48) & 0xFF);
    bytes[3] = static_cast<uint8_t>((*value >> 40) & 0xFF);
    bytes[4] = static_cast<uint8_t>((*value >> 32) & 0xFF);
    bytes[5] = static_cast<uint8_t>((*value >> 24) & 0xFF);
    bytes[6] = static_cast<uint8_t>((*value >> 16) & 0xFF);
    bytes[7] = static_cast<uint8_t>((*value >> 8) & 0xFF);
    bytes[8] = static_cast<uint8_t>(*value & 0xFF);
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string StreamingResponseWriter::encode_uint32(uint32_t value) const
{
    uint8_t bytes[5];
    bytes[0] = static_cast<uint8_t>(Protocol::DataType::UINT32);
    bytes[1] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[3] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[4] = static_cast<uint8_t>(value & 0xFF);
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string StreamingResponseWriter::encode_uint64(uint64_t value) const
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
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string StreamingResponseWriter::encode_int64(int64_t value) const
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
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string
    StreamingResponseWriter::encode_string(const std::string& value, Protocol::DataType data_type) const
{
    std::string res;
    res += static_cast<char>(data_type);
    res += encode_size(value.size());
    res += value;
    return res;
}

std::string StreamingResponseWriter::encode_path(uint64_t path_id) const
{
    using namespace std::placeholders;
    uint_fast32_t path_length = 0UL;
    std::stringstream ss;
    path_manager.print(
        ss,
        path_id,
        std::bind(&StreamingResponseWriter::write_path_node, this, _1, _2),
        std::bind(&StreamingResponseWriter::write_path_edge, this, _1, _2, _3, &path_length)
    );

    std::string res;
    res += static_cast<char>(Protocol::DataType::PATH);
    res += encode_size(path_length);
    res += ss.str();
    return res;
}

std::string StreamingResponseWriter::encode_date(DateTime datetime) const
{
    bool error;
    const int64_t year = datetime.get_year(&error);
    const int64_t month = datetime.get_month(&error);
    const int64_t day = datetime.get_day(&error);
    const int64_t tz_min_offset = datetime.get_tz_min_offset(&error);

    std::string res;
    res += static_cast<char>(Protocol::DataType::DATE);
    res += encode_int64_raw(year);
    res += encode_int64_raw(month);
    res += encode_int64_raw(day);
    res += encode_int64_raw(error ? 0 : tz_min_offset);

    return res;
}

std::string StreamingResponseWriter::encode_time(DateTime datetime) const
{
    bool error;
    const int64_t hour = datetime.get_hour(&error);
    const int64_t minute = datetime.get_minute(&error);
    const int64_t second = datetime.get_second(&error);
    const int64_t tz_min_offset = datetime.get_tz_min_offset(&error);

    std::string res;
    res += static_cast<char>(Protocol::DataType::TIME);
    res += encode_int64_raw(hour);
    res += encode_int64_raw(minute);
    res += encode_int64_raw(second);
    res += encode_int64_raw(error ? 0 : tz_min_offset);
    return res;
}

std::string StreamingResponseWriter::encode_datetime(DateTime datetime) const
{
    bool error;
    const int64_t year = datetime.get_year(&error);
    const int64_t month = datetime.get_month(&error);
    const int64_t day = datetime.get_day(&error);
    const int64_t hour = datetime.get_hour(&error);
    const int64_t minute = datetime.get_minute(&error);
    const int64_t second = datetime.get_second(&error);
    const int64_t tz_min_offset = datetime.get_tz_min_offset(&error);

    std::string res;
    res += static_cast<char>(Protocol::DataType::DATETIME);
    res += encode_int64_raw(year);
    res += encode_int64_raw(month);
    res += encode_int64_raw(day);
    res += encode_int64_raw(hour);
    res += encode_int64_raw(minute);
    res += encode_int64_raw(second);
    res += encode_int64_raw(error ? 0 : tz_min_offset);
    return res;
}

template<typename T>
std::string StreamingResponseWriter::encode_tensor(const tensor::Tensor<T>& tensor) const
{
    std::string res;
    res += static_cast<char>(Protocol::DataType::LIST);
    res += encode_size(tensor.size());
    for (std::size_t i = 0; i < tensor.size(); ++i) {
        if constexpr (std::is_same_v<T, float>) {
            res += encode_float(tensor[i]);
        } else if constexpr (std::is_same_v<T, double>) {
            res += encode_double(tensor[i]);
        } else {
            throw std::runtime_error("Unhandled tensor type");
        }
    }
    return res;
}

std::string StreamingResponseWriter::encode_size(uint32_t value) const
{
    uint8_t bytes[4];
    bytes[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(value & 0xFF);
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

std::string StreamingResponseWriter::encode_int64_raw(int64_t value) const
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
    return std::string(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void StreamingResponseWriter::seal()
{
    response_buffer.seal();
}

void StreamingResponseWriter::write_path_node(std::ostream& os, ObjectId oid) const
{
    const auto encoded_oid = encode_object_id(oid);
    os.write(encoded_oid.c_str(), encoded_oid.size());
}

void StreamingResponseWriter::write_path_edge(
    std::ostream& os,
    ObjectId oid,
    bool reverse,
    uint_fast32_t* path_length
) const
{
    const std::string direction = reverse ? "left" : "right";

    const std::string direction_size = encode_size(direction.size());
    os.write(direction_size.c_str(), direction_size.size());

    os.write(direction.c_str(), direction.size());

    const auto encoded_oid = encode_object_id(oid);
    os.write(encoded_oid.c_str(), encoded_oid.size());

    ++(*path_length);
}

void StreamingResponseWriter::write_size(uint_fast32_t value)
{
    uint8_t bytes[4];
    bytes[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(value & 0xFF);
    response_ostream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

template std::string StreamingResponseWriter::encode_tensor<float>(const tensor::Tensor<float>& tensor) const;
template std::string StreamingResponseWriter::encode_tensor<double>(const tensor::Tensor<double>& tensor
) const;
