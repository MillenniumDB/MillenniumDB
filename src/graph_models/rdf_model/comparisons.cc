#include "comparisons.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "storage/unified_storage.h"

using namespace SPARQL;

static inline uint64_t decode_first_n_bytes(uint64_t val, int n) {
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

// returns negative number if lhs < rhs,
// returns 0 if lhs == rhs
// returns positive number if lhs > rhs
// For operators use Normal mode (default).
// For expressions use Strict mode.
template<Comparisons::Mode mode>
int64_t Comparisons::_compare(ObjectId lhs_oid, ObjectId rhs_oid, bool* error) {
    if constexpr (mode == Comparisons::Mode::Strict) {
        assert(error != nullptr);
        *error = false;
    }

    if (lhs_oid == rhs_oid) {
        return 0;
    }

    auto lhs_gen_t = lhs_oid.get_generic_type();
    auto rhs_gen_t = rhs_oid.get_generic_type();

    if (lhs_gen_t != rhs_gen_t) {
        if constexpr (mode == Comparisons::Mode::Strict) {
            *error = true;
            return 0;
        } else {
            // The bit shift is to ensure the MSB is not set when casting to int64_t
            return static_cast<int64_t>(lhs_gen_t >> 1) - static_cast<int64_t>(rhs_gen_t >> 1);
        }
    }

    switch (lhs_gen_t) {
    case ObjectId::MASK_IRI: {
        auto lhs_mod = lhs_oid.get_mod();
        auto rhs_mod = rhs_oid.get_mod();

        auto lhs_prefix_id = (lhs_oid.id & ObjectId::MASK_IRI_PREFIX) >> ObjectId::IRI_INLINE_BYTES * 8;
        auto rhs_prefix_id = (rhs_oid.id & ObjectId::MASK_IRI_PREFIX) >> ObjectId::IRI_INLINE_BYTES * 8;

        auto& lhs_prefix = rdf_model.catalog().prefixes[lhs_prefix_id];
        auto& rhs_prefix = rdf_model.catalog().prefixes[rhs_prefix_id];

        auto lhs_content = lhs_oid.id & ObjectId::MASK_IRI_CONTENT;
        auto rhs_content = rhs_oid.id & ObjectId::MASK_IRI_CONTENT;

        std::unique_ptr<CharIter> lhs_iter;
        std::unique_ptr<CharIter> rhs_iter;

        switch (lhs_mod) {
        case ObjectId::MOD_INLINE: {
            lhs_iter = std::make_unique<IriInlineIter>(lhs_prefix, decode_first_n_bytes(lhs_content, ObjectId::IRI_INLINE_BYTES));
            break;
        }
        case ObjectId::MOD_EXTERNAL: {
            lhs_iter = std::make_unique<IriExternalIter>(lhs_prefix, lhs_content);
            break;
        }
        case ObjectId::MOD_TMP: {
            lhs_iter = std::make_unique<IriTmpIter>(lhs_prefix, lhs_content);
            break;
        }
        }

        switch (rhs_mod) {
        case ObjectId::MOD_INLINE: {
            rhs_iter = std::make_unique<IriInlineIter>(rhs_prefix, decode_first_n_bytes(rhs_content, ObjectId::IRI_INLINE_BYTES));
            break;
        }
        case ObjectId::MOD_EXTERNAL: {
            rhs_iter = std::make_unique<IriExternalIter>(rhs_prefix, rhs_content);
            break;
        }
        case ObjectId::MOD_TMP: {
            rhs_iter = std::make_unique<IriTmpIter>(rhs_prefix, rhs_content);
            break;
        }
        }

        return string_manager.compare(*lhs_iter, *rhs_iter);
    }
    case ObjectId::MASK_STRING: {
        auto lhs_sub_t = lhs_oid.get_sub_type();
        auto rhs_sub_t = rhs_oid.get_sub_type();

        if (lhs_sub_t != rhs_sub_t) {
            return static_cast<int64_t>(lhs_sub_t) - static_cast<int64_t>(rhs_sub_t);
        }
        auto lhs_mod = lhs_oid.get_mod();
        auto rhs_mod = rhs_oid.get_mod();

        auto lhs_value = lhs_oid.get_value();
        auto rhs_value = rhs_oid.get_value();

        std::unique_ptr<CharIter> lhs_string_iter;
        std::unique_ptr<CharIter> rhs_string_iter;

        switch (lhs_sub_t) {
        case ObjectId::MASK_STRING_SIMPLE:
        case ObjectId::MASK_STRING_XSD: {
            switch (lhs_mod) {
            case ObjectId::MOD_INLINE: {
                lhs_string_iter = std::make_unique<StringInlineIter>(decode_first_n_bytes(lhs_value, ObjectId::STR_INLINE_BYTES));
                break;
            }
            case ObjectId::MOD_EXTERNAL:
            case ObjectId::MOD_TMP: {
                lhs_string_iter = UnifiedStorage::get_str_char_iter(lhs_oid);
            }
            }

            switch (rhs_mod) {
            case ObjectId::MOD_INLINE: {
                rhs_string_iter = std::make_unique<StringInlineIter>(decode_first_n_bytes(rhs_value, ObjectId::STR_INLINE_BYTES));
                break;
            }
            case ObjectId::MOD_EXTERNAL:
            case ObjectId::MOD_TMP: {
                rhs_string_iter = UnifiedStorage::get_str_char_iter(rhs_oid);
            }
            }
            break;
        }
        case ObjectId::MASK_STRING_LANG:
        case ObjectId::MASK_STRING_DATATYPE: {
            auto lhs_tag = lhs_oid.id & ObjectId::MASK_LITERAL_TAG;
            auto rhs_tag = rhs_oid.id & ObjectId::MASK_LITERAL_TAG;

            if (lhs_tag != rhs_tag) {
                return static_cast<int64_t>(lhs_tag) - static_cast<int64_t>(rhs_tag);
            }

            switch (lhs_mod) {
            case ObjectId::MOD_INLINE: {
                lhs_string_iter = std::make_unique<StringInlineIter>(decode_first_n_bytes(lhs_value, ObjectId::STR_DT_INLINE_BYTES));
                break;
            }
            case ObjectId::MOD_EXTERNAL:
            case ObjectId::MOD_TMP: {
                lhs_string_iter = UnifiedStorage::get_str_char_iter(lhs_oid);
            }
            }

            switch (rhs_mod) {
            case ObjectId::MOD_INLINE: {
                rhs_string_iter = std::make_unique<StringInlineIter>(decode_first_n_bytes(rhs_value, ObjectId::STR_DT_INLINE_BYTES));
                break;
            }
            case ObjectId::MOD_EXTERNAL:
            case ObjectId::MOD_TMP: {
                rhs_string_iter = UnifiedStorage::get_str_char_iter(rhs_oid);
            }
            }
        }
        }

        return string_manager.compare(*lhs_string_iter, *rhs_string_iter);
    }
    case ObjectId::MASK_NUMERIC: {
        auto lhs_sub_t = lhs_oid.get_sub_type();
        auto rhs_sub_t = rhs_oid.get_sub_type();
        // Integer optimization
        if (lhs_sub_t == ObjectId::MASK_INT && rhs_sub_t == ObjectId::MASK_INT) {
            return static_cast<int64_t>(lhs_oid.id) - static_cast<int64_t>(rhs_oid.id);
        }
        // Decimal inlined optimization
        if (lhs_oid.get_type() == ObjectId::MASK_DECIMAL_INLINED
            && rhs_oid.get_type() == ObjectId::MASK_DECIMAL_INLINED)
        {
            auto lhs_sep = lhs_oid.id & Conversions::DECIMAL_SEPARATOR_MASK;
            auto rhs_sep = rhs_oid.id & Conversions::DECIMAL_SEPARATOR_MASK;

            auto lhs_num = static_cast<int64_t>((lhs_oid.id & Conversions::DECIMAL_NUMBER_MASK) >> 4);
            auto rhs_num = static_cast<int64_t>((rhs_oid.id & Conversions::DECIMAL_NUMBER_MASK) >> 4);

            auto lhs_sign = static_cast<int64_t>((lhs_oid.id & Conversions::DECIMAL_SIGN_MASK) >> 55);
            auto rhs_sign = static_cast<int64_t>((rhs_oid.id & Conversions::DECIMAL_SIGN_MASK) >> 55);

            lhs_num *= (1 - 2 * lhs_sign);
            rhs_num *= (1 - 2 * rhs_sign);

            if (lhs_sep == rhs_sep) {
                return lhs_num - rhs_num;
            } else {
                int64_t swap = 1;
                if (rhs_sep > lhs_sep) {
                    std::swap(lhs_sep, rhs_sep);
                    std::swap(lhs_num, rhs_num);
                    swap = -1;
                }

                int64_t diff      = lhs_sep - rhs_sep;
                int64_t div       = static_cast<int64_t>(std::pow(10, diff));
                int64_t division  = lhs_num / div;
                int64_t remainder = lhs_num % div;
                if (division == rhs_num) {
                    return remainder * swap;
                } else {
                    return (division - rhs_num) * swap;
                }
            }
        }

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case Conversions::OPTYPE_INTEGER: {
            throw LogicException("This should have been handled by the Integer optimization.");
        }
        case Conversions::OPTYPE_DECIMAL: {
            auto lhs = Conversions::to_decimal(lhs_oid);
            auto rhs = Conversions::to_decimal(rhs_oid);
            if (lhs == rhs) {
                return 0;
            } else if (lhs < rhs) {
                return -1;
            } else {
                return 1;
            }
        }
        case Conversions::OPTYPE_FLOAT: {
            auto lhs = Conversions::to_float(lhs_oid);
            auto rhs = Conversions::to_float(rhs_oid);
            // Cant just return subtraction and implicit cast to int
            // because of overflow for big floats when casting to in.
            if (lhs == rhs) {
                return 0;
            } else if (lhs < rhs) {
                return -1;
            } else {
                return 1;
            }
        }
        case Conversions::OPTYPE_DOUBLE: {
            auto lhs = Conversions::to_double(lhs_oid);
            auto rhs = Conversions::to_double(rhs_oid);
            // Cant just return subtraction and implicit cast to int
            // because of overflow for big floats when casting to in.
            if (lhs == rhs) {
                return 0;
            } else if (lhs < rhs) {
                return -1;
            } else {
                return 1;
            }
        }
        default:
            throw LogicException("This should never happen");
        }
    }
    case ObjectId::MASK_DT: {
        DateTime lhs_dt(lhs_oid);
        DateTime rhs_dt(rhs_oid);

        if constexpr (mode == Comparisons::Mode::Strict) {
            return lhs_dt.compare<DateTimeComparisonMode::Strict>(rhs_dt, error);
        } else if constexpr (mode == Comparisons::Mode::Normal){
            return lhs_dt.compare<DateTimeComparisonMode::Normal>(rhs_dt, error);
        }
    }
    case ObjectId::MASK_BOOL: {
        return static_cast<int64_t>(lhs_oid.id & 1) - static_cast<int64_t>(rhs_oid.id & 1);
    }
    default: {
        return static_cast<int64_t>(lhs_oid.id & ObjectId::VALUE_MASK)
             - static_cast<int64_t>(rhs_oid.id & ObjectId::VALUE_MASK);
    }
    }
}


template int64_t Comparisons::_compare<Comparisons::Mode::Normal>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
template int64_t Comparisons::_compare<Comparisons::Mode::Strict>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
