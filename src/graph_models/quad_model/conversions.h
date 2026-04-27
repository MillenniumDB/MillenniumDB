#pragma once

#include "graph_models/common/conversions.h" // IWYU pragma: export
#include "graph_models/inliner.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

#include <cstdint>
#include <sstream>
#include <string>

namespace MQL { namespace Conversions {
using namespace Common::Conversions;

// The order, int < flt < inv is important
constexpr uint8_t OPTYPE_INTEGER = 0x01;
constexpr uint8_t OPTYPE_FLOAT = 0x02;
constexpr uint8_t OPTYPE_INVALID = 0x03;

inline uint64_t unpack_anon(ObjectId oid)
{
    return oid.get_value();
}

inline uint64_t unpack_edge(ObjectId oid)
{
    return oid.get_value();
}

inline std::string unpack_named_node(ObjectId oid)
{
    switch (oid.type()) {
    case ObjectType::NamedNodeInl: {
        return Inliner::get_string_inlined<ObjectId::NAMED_NODE_INLINE_BYTES>(oid.get_value());
    }
    case ObjectType::NamedNodeExt: {
        std::stringstream ss;
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return ss.str();
    }
    case ObjectType::NamedNodeTmp: {
        std::stringstream ss;
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);
        return ss.str();
    }
    default: {
        throw LogicException(
            "Called unpack_named_node with incorrect ObjectId type, this should never happen"
        );
    }
    }
}

inline ObjectId pack_named_node(const std::string& str)
{
    uint64_t oid;
    if (str.size() <= ObjectId::NAMED_NODE_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_NAMED_NODE_INL;
    } else {
        const auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            oid = ObjectId::MASK_NAMED_NODE_EXT | str_id;
        } else {
            oid = ObjectId::MASK_NAMED_NODE_TMP | tmp_manager.get_str_id(str);
        }
    }
    return ObjectId(oid);
}

inline ObjectId pack_edge(uint64_t edge_id)
{
    return ObjectId(ObjectId::MASK_DIRECTED_EDGE | edge_id);
}

inline ObjectId pack_anon_tmp(uint64_t anon_id)
{
    return ObjectId(ObjectId::MASK_ANON_TMP | anon_id);
}

inline DateTime unpack_datetime(ObjectId oid)
{
    return DateTime(oid);
}

inline float to_float(ObjectId oid)
{
    switch (oid.subtype()) {
    case ObjectSubType::Int:
        return unpack_int(oid);
    case ObjectSubType::Float:
        return unpack_float(oid);
    case ObjectSubType::Double:
        return unpack_double(oid);
    default:
        throw LogicException("Called to_float with incorrect ObjectId type, this should never happen");
    }
}

// Returns a string with the lexical representation of the value
inline std::string to_lexical_str(ObjectId oid)
{
    switch (oid.type()) {
    case ObjectType::AnonInl:
        return "_a" + std::to_string(unpack_anon(oid));
    case ObjectType::AnonTmp:
        return "_t" + std::to_string(unpack_anon(oid));
    case ObjectType::NamedNodeInl:
    case ObjectType::NamedNodeExt:
    case ObjectType::NamedNodeTmp:
        return unpack_named_node(oid);
    case ObjectType::StringInl:
    case ObjectType::StringExt:
    case ObjectType::StringTmp:
        return unpack_string(oid);
    case ObjectType::NegativeInt56:
    case ObjectType::PositiveInt56: {
        const int64_t i = unpack_int(oid);
        return std::to_string(i);
    }
    case ObjectType::Float: {
        const float f = unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        return std::string(float_buffer);
    }
    case ObjectType::Date: {
        const DateTime datetime = unpack_date(oid);
        return "date(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectType::Datetime: {
        const DateTime datetime = unpack_date(oid);
        return "dateTime(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectType::Datetimestamp: {
        const DateTime datetime = unpack_date(oid);
        return "dateTimeStamp(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectType::Time: {
        const DateTime datetime = unpack_date(oid);
        return "time(\"" + datetime.get_value_string() + "\")";
    }
    case ObjectType::Bool:
        return unpack_bool(oid) ? "true" : "false";
    case ObjectType::DirectedEdge:
        return "_e" + std::to_string(unpack_edge(oid));

    case ObjectType::TensorFloatExt:
    case ObjectType::TensorFloatTmp: {
        const auto tensor = unpack_tensor<float>(oid);
        return tensor.to_string();
    }
    case ObjectType::TensorDoubleExt:
    case ObjectType::TensorDoubleTmp: {
        const auto tensor = unpack_tensor<double>(oid);
        return tensor.to_string();
    }
    case ObjectType::ListExt:
    case ObjectType::ListTmp: {
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
    case ObjectType::DictionaryExt:
    case ObjectType::DictionaryTmp: {
        std::unique_ptr<Dictionary> dict = unpack_dictionary(oid);
        std::stringstream ss;
        dict->to_string(ss);
        return ss.str();
    }
    case ObjectType::Path:
    case ObjectType::Null:
    case ObjectType::DoubleExt:
    case ObjectType::DoubleTmp:
    case ObjectType::DecimalInl:
    case ObjectType::DecimalExt:
    case ObjectType::DecimalTmp:
        // TODO:
        break;

    case ObjectType::StringXsdInl:
    case ObjectType::StringXsdExt:
    case ObjectType::StringXsdTmp:
    case ObjectType::StringLangInl:
    case ObjectType::StringLangExt:
    case ObjectType::StringLangTmp:
    case ObjectType::StringDatatypeInl:
    case ObjectType::StringDatatypeExt:
    case ObjectType::StringDatatypeTmp:
    case ObjectType::IriInl:
    case ObjectType::IriExt:
    case ObjectType::IriTmp:
    case ObjectType::IriUuidLowerTmp:
    case ObjectType::IriUuidLowerExt:
    case ObjectType::IriUuidUpperTmp:
    case ObjectType::IriUuidUpperExt:
    case ObjectType::IriHexLowerTmp:
    case ObjectType::IriHexLowerExt:
    case ObjectType::IriHexUpperTmp:
    case ObjectType::IriHexUpperExt:
    case ObjectType::UndirectedEdge:
    case ObjectType::EdgeKey:
    case ObjectType::NodeKey:
    case ObjectType::EdgeLabel:
    case ObjectType::NodeLabel:
    case ObjectType::NamedNodeHexExt:
    case ObjectType::NamedNodeHexTmp:
    case ObjectType::NamedNodeUuidExt:
    case ObjectType::NamedNodeUuidTmp:
    case ObjectType::Direction:
    case ObjectType::NotFound:
        assert(false);
        break;
    }
    return "";
}

inline ObjectId to_boolean(ObjectId oid)
{
    uint64_t value = oid.get_value();

    switch (oid.subtype()) {
    case ObjectSubType::Bool:
        return oid;
    // Note: Extern strings will never be empty
    // Note: This assumes 0 is never represented as 0.0, 0.00, etc
    // Note: Extern decimals will never be zero
    case ObjectSubType::Decimal:
    case ObjectSubType::String:
    case ObjectSubType::Int:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    case ObjectSubType::Float: {
        auto f = unpack_float(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(f != 0 && !std::isnan(f)));
    }
    case ObjectSubType::Double: {
        auto d = unpack_double(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(d != 0 && !std::isnan(d)));
    }

    // Note: This assumes empty tensors will never be extern/tmp
    // case ObjectSubType::TensorFloat:
    // case ObjectSubType::TensorDouble:
    //     return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    //     return ObjectId::get_true();
    // Can not be converted to boolean
    default:
        return ObjectId::get_null();
    }
}

// works for named nodes and strings
inline size_t print_string(ObjectId oid, char* out)
{
    const auto type = oid.type();
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
    switch (type) {
    case ObjectType::NamedNodeInl: {
        return Inliner::print_string_inlined<7>(out, unmasked_id);
    }
    case ObjectType::NamedNodeExt: {
        return string_manager.print_to_buffer(out, unmasked_id);
    }
    case ObjectType::NamedNodeTmp: {
        return tmp_manager.print_to_buffer(out, unmasked_id);
    }
    case ObjectType::StringInl: {
        return Inliner::print_string_inlined<7>(out, unmasked_id);
    }
    case ObjectType::StringExt: {
        return string_manager.print_to_buffer(out, unmasked_id);
    }
    case ObjectType::StringTmp: {
        return tmp_manager.print_to_buffer(out, unmasked_id);
    }
    default:
        throw std::logic_error("Unmanaged mask in MQL::Conversions::print_string: " + to_string(type));
    }
}

}} // namespace MQL::Conversions
