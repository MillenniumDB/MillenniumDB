#pragma once

#include <cstdint>
#include <string>
#include <sstream>

#include "graph_models/common/conversions.h" // IWYU pragma: export
#include "query/executor/query_executor/mql/return_executor.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "graph_models/inliner.h"

namespace MQL { namespace Conversions {
    using namespace Common::Conversions;

    // The order, int < flt < inv is important
    constexpr uint8_t OPTYPE_INTEGER = 0x01;
    constexpr uint8_t OPTYPE_FLOAT   = 0x02;
    constexpr uint8_t OPTYPE_INVALID = 0x03;

    inline uint64_t unpack_blank(ObjectId oid) {
        return oid.get_value();
    }

    inline uint64_t unpack_edge(ObjectId oid) {
        return oid.get_value();
    }

    inline std::string unpack_string(ObjectId oid) {
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

    inline std::string unpack_named_node(ObjectId oid) {
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
            throw LogicException("Called unpack_named_node with incorrect ObjectId type, this should never happen");
        }
        }
    }

    inline DateTime unpack_datetime(ObjectId oid) {
        return DateTime(oid);
    }

    inline float to_float(ObjectId oid) {
        switch (oid.get_sub_type()) {
        case ObjectId::MASK_INT:
            return unpack_int(oid);
        case ObjectId::MASK_FLOAT:
            return unpack_float(oid);
        default:
            throw LogicException("Called to_float with incorrect ObjectId type, this should never happen");
        }
    }

    // Returns a string with the lexical representation of the value
    inline std::string to_lexical_str(ObjectId oid) {
        std::stringstream ss;
        MQL::ReturnExecutor<MQL::ReturnType::TSV>::print(ss, oid);
        return ss.str();
    }

    inline ObjectId to_boolean(ObjectId oid) {
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
    inline size_t print_string(ObjectId oid, char* out) {
        const auto mask = oid.id & ObjectId::TYPE_MASK;
        const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
        switch (mask) {
        case ObjectId::MASK_NAMED_NODE_INLINED: {
            return Inliner::print_string_inlined<7>(out , unmasked_id);
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
            throw std::logic_error("Unmanaged mask in MQL::Conversions::print_string: "
                + std::to_string(mask));
        }
    }
}
} // namespace MQL::Conversions
