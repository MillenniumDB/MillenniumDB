#include "comparisons.h"

#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

using namespace MQL;

// TODO: same as in SPARQL::Comparisons, should be extracted somewhere else
// TODO: make n template?
uint64_t decode_first_n_bytes(uint64_t val, int n) {
    assert(n > 0 && n <= ObjectId::MAX_INLINED_BYTES);
    uint64_t res = 0; // Ensure null-termination.
    uint8_t* c = reinterpret_cast<uint8_t*>(&res);
    int  shift_size = (n - 1) * 8;
    for (int i = 0; i < n; i++) {
        uint8_t byte = (val >> shift_size) & 0xFF;
        c[i]         = byte;
        shift_size -= 8;
    }
    return res;
}

int64_t Comparisons::compare(ObjectId lhs, ObjectId rhs) {
    const auto lhs_generic_type = lhs.id & ObjectId::GENERIC_TYPE_MASK;
    const auto rhs_generic_type = rhs.id & ObjectId::GENERIC_TYPE_MASK;

    if (lhs_generic_type != rhs_generic_type) {
        return lhs_generic_type - rhs_generic_type;
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
                decode_first_n_bytes(lhs_unmasked_id, ObjectId::NAMED_NODE_INLINE_BYTES)
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
                decode_first_n_bytes(rhs_unmasked_id, ObjectId::NAMED_NODE_INLINE_BYTES)
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
            float f;
            uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
            dest[0] =  lhs.id        & 0xFF;
            dest[1] = (lhs.id >> 8)  & 0xFF;
            dest[2] = (lhs.id >> 16) & 0xFF;
            dest[3] = (lhs.id >> 24) & 0xFF;

            lhs_value = f;
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
            float f;
            uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
            dest[0] =  rhs.id        & 0xFF;
            dest[1] = (rhs.id >> 8)  & 0xFF;
            dest[2] = (rhs.id >> 16) & 0xFF;
            dest[3] = (rhs.id >> 24) & 0xFF;

            rhs_value = f;
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
    case ObjectId::MASK_ANON_INLINED:
    case ObjectId::MASK_BOOL:
    case ObjectId::MASK_PATH:
    case ObjectId::MASK_EDGE:
        return lhs_unmasked_id - rhs_unmasked_id;
    default:
        throw std::logic_error("Unmanaged generic mask in Quad Comparisons "
            + std::to_string(lhs_generic_type));
    }
}