#include "comparisons.h"

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/inliner.h"
#include "graph_models/quad_model/conversions.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

using namespace MQL;

int64_t Comparisons::compare(ObjectId lhs, ObjectId rhs) {
    const auto lhs_generic_type = lhs.id & ObjectId::GENERIC_TYPE_MASK;
    const auto rhs_generic_type = rhs.id & ObjectId::GENERIC_TYPE_MASK;

    if (lhs_generic_type != rhs_generic_type) {
        // The bit shift is to ensure the MSB is not set when casting to int64_t
        return static_cast<int64_t>(lhs_generic_type >> 56) - static_cast<int64_t>(rhs_generic_type >> 56);
    }

    const auto lhs_unmasked_id = lhs.id & ObjectId::VALUE_MASK;
    const auto rhs_unmasked_id = rhs.id & ObjectId::VALUE_MASK;
    /*
    GENERIC:
        MASK_NULL
        MASK_ANON
        MASK_NAMED_NODE
        MASK_STRING_SIMPLE
        MASK_NUMERIC
        MASK_BOOL
        MASK_EDGE
        MASK_PATH
    */

    switch (lhs_generic_type) {
    case ObjectId::MASK_NULL: {
        return 0;
    }
    case ObjectId::MASK_STRING:
    case ObjectId::MASK_NAMED_NODE: {
        std::unique_ptr<CharIter> lhs_string_iter;
        std::unique_ptr<CharIter> rhs_string_iter;

        const auto lhs_mod = lhs.id & ObjectId::MOD_MASK;
        const auto rhs_mod = rhs.id & ObjectId::MOD_MASK;

        switch (lhs_mod) {
        case ObjectId::MOD_INLINE: {
            lhs_string_iter = std::make_unique<StringInlineIter>(
                Inliner::decode<ObjectId::NAMED_NODE_INLINE_BYTES>(lhs_unmasked_id)
            );
            break;
        }
        case ObjectId::MOD_EXTERNAL: {
            lhs_string_iter = string_manager.get_char_iter(lhs_unmasked_id);
            break;
        }
        case ObjectId::MOD_TMP: {
            lhs_string_iter = tmp_manager.get_str_char_iter(lhs_unmasked_id);
            break;
        }
        default:
        throw std::logic_error("Unmanaged mod in Quad Comparisons"
            + std::to_string(lhs_mod));
        }

        switch (rhs_mod) {
        case ObjectId::MOD_INLINE: {
            rhs_string_iter = std::make_unique<StringInlineIter>(
                Inliner::decode<ObjectId::NAMED_NODE_INLINE_BYTES>(rhs_unmasked_id)
            );
            break;
        }
        case ObjectId::MOD_EXTERNAL: {
            rhs_string_iter = string_manager.get_char_iter(rhs_unmasked_id);
            break;
        }
        case ObjectId::MOD_TMP: {
            rhs_string_iter = tmp_manager.get_str_char_iter(rhs_unmasked_id);
            break;
        }
        default:
        throw std::logic_error("Unmanaged mod in Quad Comparisons"
            + std::to_string(rhs_mod));
        }
        return StringManager::compare(*lhs_string_iter, *rhs_string_iter);
    }
    case ObjectId::MASK_NUMERIC: {
        double lhs_value;
        double rhs_value;

        const auto lhs_type = lhs.id & ObjectId::TYPE_MASK;
        const auto rhs_type = rhs.id & ObjectId::TYPE_MASK;

        switch (lhs_type) {
        case ObjectId::MASK_NEGATIVE_INT: {
            int64_t i = (~lhs.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            lhs_value =  i*-1;
            break;
        }
        case ObjectId::MASK_POSITIVE_INT: {
            int64_t i = lhs_unmasked_id;
            lhs_value = i;
            break;
        }
        case ObjectId::MASK_FLOAT: {
            lhs_value = Conversions::unpack_float(lhs);
            break;
        }
        default:
        throw std::logic_error("Unmanaged NUMERIC type "
            + std::to_string(lhs_type));
        }


        switch (rhs_type) {
        case ObjectId::MASK_NEGATIVE_INT: {
            int64_t i = (~rhs.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            rhs_value =  i*-1;
            break;
        }
        case ObjectId::MASK_POSITIVE_INT: {
            int64_t i = rhs_unmasked_id;
            rhs_value = i;
            break;
        }
        case ObjectId::MASK_FLOAT: {
            rhs_value = Conversions::unpack_float(rhs);
            break;
        }
        default:
        throw std::logic_error("Unmanaged NUMERIC type "
            + std::to_string(lhs_type));
        }
        if (lhs_value == rhs_value) {
            return 0;
        }
        return lhs_value < rhs_value ? -1 : 1;
    }
    case ObjectId::MASK_DT: {
        DateTime lhs_dt(lhs);
        DateTime rhs_dt(rhs);
        return lhs_dt.MQL_compare(rhs_dt);
    }
    case ObjectId::MASK_ANON:
    case ObjectId::MASK_BOOL:
    case ObjectId::MASK_PATH:
    case ObjectId::MASK_EDGE:
        return lhs_unmasked_id - rhs_unmasked_id;
    default:
        throw std::logic_error("Unmanaged generic mask in Quad Comparisons "
            + std::to_string(lhs_generic_type));
    }
}