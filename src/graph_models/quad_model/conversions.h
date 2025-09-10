#pragma once

#include <cstdint>
#include <sstream>
#include <string>

#include "graph_models/common/conversions.h" // IWYU pragma: export
#include "graph_models/inliner.h"
#include "query/executor/query_executor/mql/return_executor.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

namespace MQL { namespace Conversions {
using namespace Common::Conversions;

// The order, int < flt < inv is important
constexpr uint8_t OPTYPE_INTEGER = 0x01;
constexpr uint8_t OPTYPE_FLOAT = 0x02;
constexpr uint8_t OPTYPE_INVALID = 0x03;

constexpr uint64_t LIST_FILE_ID_MASK = 0x00FF'FF00'0000'0000UL;
constexpr uint64_t LIST_OFFSET_MASK = 0x0000'00FF'FFFF'FFFFUL;

inline uint64_t unpack_blank(ObjectId oid)
{
    return oid.get_value();
}

inline uint64_t unpack_edge(ObjectId oid)
{
    return oid.get_value();
}

inline std::string unpack_string(ObjectId oid)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        return Inliner::get_string_inlined<ObjectId::STR_INLINE_BYTES>(oid.get_value());
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        std::stringstream ss;
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return ss.str();
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        std::stringstream ss;
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);
        return ss.str();
    }
    default: {
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
    }
    }
}

inline ObjectId pack_string(const std::string& str)
{
    uint64_t oid;
    if (str.size() == 0) {
        return ObjectId(ObjectId::MASK_STRING_SIMPLE_INLINED);
    } else if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_STRING_SIMPLE_INLINED;
    } else {
        const auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            oid = ObjectId::MASK_STRING_SIMPLE_EXTERN | str_id;
        } else {
            oid = ObjectId::MASK_STRING_SIMPLE_TMP | tmp_manager.get_str_id(str);
        }
    }
    return ObjectId(oid);
}

inline std::string unpack_named_node(ObjectId oid)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_NAMED_NODE_INLINED: {
        return Inliner::get_string_inlined<ObjectId::NAMED_NODE_INLINE_BYTES>(oid.get_value());
    }
    case ObjectId::MASK_NAMED_NODE_EXTERN: {
        std::stringstream ss;
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return ss.str();
    }
    case ObjectId::MASK_NAMED_NODE_TMP: {
        std::stringstream ss;
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);
        return ss.str();
    }
    default: {
        throw LogicException("Called unpack_named_node with incorrect ObjectId type, this should never happen"
        );
    }
    }
}

inline DateTime unpack_datetime(ObjectId oid)
{
    return DateTime(oid);
}

inline float to_float(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    case ObjectId::MASK_FLOAT:
        return unpack_float(oid);
    default:
        throw LogicException("Called to_float with incorrect ObjectId type, this should never happen");
    }
}

inline ObjectId pack_list(const std::vector<ObjectId>& list)
{
    TmpLists& tmp_list = tmp_manager.get_tmp_list();
    uint32_t file_id = tmp_list.get_file_id();
    uint64_t list_offset = tmp_list.insert(list);
    return ObjectId(ObjectId::MASK_LIST | (uint64_t(file_id) << 40) | list_offset);
}

inline void unpack_list(ObjectId list_id, std::vector<ObjectId>& out)
{
    auto& lists = tmp_manager.get_tmp_list();
    assert((LIST_FILE_ID_MASK & list_id.id) >> 40 == lists.get_file_id());

    lists.get(out, list_id.id & LIST_OFFSET_MASK);
}

inline std::vector<ObjectId> unpack_list(ObjectId list_id)
{
    std::vector<ObjectId> list;
    unpack_list(list_id, list);
    return list;
}

// Returns a string with the lexical representation of the value
inline std::string to_lexical_str(ObjectId oid)
{
    const auto type = oid.id & ObjectId::TYPE_MASK;

    switch (type) {
    case ObjectId::MASK_ANON_INLINED:
        return "_:b" + std::to_string(unpack_blank(oid));
    case ObjectId::MASK_ANON_TMP:
        return "_:c" + std::to_string(unpack_blank(oid));
    case ObjectId::MASK_NAMED_NODE_INLINED:
    case ObjectId::MASK_NAMED_NODE_EXTERN:
    case ObjectId::MASK_NAMED_NODE_TMP:
        return unpack_named_node(oid);
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
    case ObjectId::MASK_STRING_SIMPLE_TMP:
        return unpack_string(oid);
    case ObjectId::MASK_NEGATIVE_INT:
    case ObjectId::MASK_POSITIVE_INT: {
        const int64_t i = unpack_int(oid);
        return std::to_string(i);
    }
    case ObjectId::MASK_FLOAT: {
        const float f = unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        return std::string(float_buffer);
    }
    case ObjectId::MASK_DT_DATE: {
        const DateTime datetime = unpack_date(oid);
        return "date(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectId::MASK_DT_DATETIME: {
        const DateTime datetime = unpack_date(oid);
        return "dateTime(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectId::MASK_DT_DATETIMESTAMP: {
        const DateTime datetime = unpack_date(oid);
        return "dateTimeStamp(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectId::MASK_DT_TIME: {
        const DateTime datetime = unpack_date(oid);
        return "time(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectId::MASK_BOOL:
        return unpack_bool(oid) ? "true" : "false";
    case ObjectId::MASK_EDGE:
        return "_e" + std::to_string(unpack_edge(oid));

    case ObjectId::MASK_TENSOR_FLOAT_INLINED:
    case ObjectId::MASK_TENSOR_FLOAT_EXTERN:
    case ObjectId::MASK_TENSOR_FLOAT_TMP: {
        const auto tensor = unpack_tensor<float>(oid);
        return tensor.to_string();
    }
    case ObjectId::MASK_TENSOR_DOUBLE_INLINED:
    case ObjectId::MASK_TENSOR_DOUBLE_EXTERN:
    case ObjectId::MASK_TENSOR_DOUBLE_TMP: {
        const auto tensor = unpack_tensor<double>(oid);
        return tensor.to_string();
    }
    case ObjectId::MASK_LIST: {
        auto list = unpack_list(oid);
        std::stringstream ss;
        ss << "[";
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it != list.begin()) {
                ss << ",";
            }
            to_lexical_str(*it);
        }
        ss << "]";
        return ss.str();
    }
    case ObjectId::MASK_DICTIONARY:
    case ObjectId::MASK_DICTIONARY_TMP: {
        std::unique_ptr<Dictionary> dict = unpack_dictionary(oid);
        std::stringstream ss;
        dict->to_string(ss);
        return ss.str();
    }
    default:
        assert(false);
        return "";
    }
}

inline ObjectId to_boolean(ObjectId oid)
{
    uint64_t value = oid.get_value();

    switch (oid.get_sub_type()) {
    case ObjectId::MASK_BOOL:
        return oid;
    // String
    // Note: Extern strings will never be empty
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
        return ObjectId(ObjectId::BOOL_TRUE);
    // Integer
    case ObjectId::MASK_NEGATIVE_INT:
    case ObjectId::MASK_POSITIVE_INT:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Float
    case ObjectId::MASK_FLOAT: {
        auto f = unpack_float(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(f != 0 && !std::isnan(f)));
    }
    // Double
    case ObjectId::MASK_DOUBLE: {
        auto d = unpack_double(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(d != 0 && !std::isnan(d)));
    }
    // Decimal
    // Note: This assumes 0 is never represented as 0.0, 0.00, etc
    case ObjectId::MASK_DECIMAL_INLINED:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Note: Extern decimals will never be zero
    case ObjectId::MASK_DECIMAL_EXTERN:
        return ObjectId(ObjectId::BOOL_TRUE);
        // Note: This assumes empty tensors will never be extern/tmp
    case ObjectId::MASK_TENSOR_FLOAT_INLINED:
    case ObjectId::MASK_TENSOR_DOUBLE_INLINED:
        return ObjectId(ObjectId::BOOL_FALSE);
    // Note: extern/tmp tensors will never be empty
    case ObjectId::MASK_TENSOR_FLOAT_EXTERN:
    case ObjectId::MASK_TENSOR_FLOAT_TMP:
    case ObjectId::MASK_TENSOR_DOUBLE_EXTERN:
    case ObjectId::MASK_TENSOR_DOUBLE_TMP:
        return ObjectId(ObjectId::BOOL_TRUE);
    // Can not be converted to boolean
    default:
        return ObjectId::get_null();
    }
}

// works for named nodes and strings
inline size_t print_string(ObjectId oid, char* out)
{
    const auto mask = oid.id & ObjectId::TYPE_MASK;
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
    switch (mask) {
    case ObjectId::MASK_NAMED_NODE_INLINED: {
        return Inliner::print_string_inlined<7>(out, unmasked_id);
    }
    case ObjectId::MASK_NAMED_NODE_EXTERN: {
        return string_manager.print_to_buffer(out, unmasked_id);
    }
    case ObjectId::MASK_NAMED_NODE_TMP: {
        return tmp_manager.print_to_buffer(out, unmasked_id);
    }
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        return Inliner::print_string_inlined<7>(out, unmasked_id);
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        return string_manager.print_to_buffer(out, unmasked_id);
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        return tmp_manager.print_to_buffer(out, unmasked_id);
    }
    default:
        throw std::logic_error("Unmanaged mask in MQL::Conversions::print_string: " + std::to_string(mask));
    }
}

}} // namespace MQL::Conversions
