#include "comparisons.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/inliner.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

using namespace GQL;

// returns negative number if lhs < rhs,
// returns 0 if lhs == rhs
// returns positive number if lhs > rhs
// For operators use Normal mode (default).
// For expressions use Strict mode.
template<Comparisons::Mode mode, bool null_first>
int64_t Comparisons::_compare(ObjectId lhs_oid, ObjectId rhs_oid, bool* error)
{
    if constexpr (mode == Comparisons::Mode::Strict) {
        assert(error != nullptr);
        *error = false;
    }

    if (lhs_oid == rhs_oid) {
        return 0;
    }
    if (null_first) {
        if (lhs_oid.is_null()) {
            return -1;
        } else if (rhs_oid.is_null()) {
            return 1;
        }
    } else {
        if (lhs_oid.is_null()) {
            return 1;
        } else if (rhs_oid.is_null()) {
            return -1;
        }
    }

    auto lhs_gen_t = GQL_OID::get_generic_type(lhs_oid);
    auto rhs_gen_t = GQL_OID::get_generic_type(rhs_oid);

    if (lhs_gen_t != rhs_gen_t) {
        if constexpr (mode == Comparisons::Mode::Strict) {
            *error = true;
            return 0;
        } else {
            return static_cast<int64_t>(lhs_gen_t) - static_cast<int64_t>(rhs_gen_t);
        }
    }

    switch (lhs_gen_t) {
    case GQL_OID::GenericType::STRING: {
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
        case ObjectId::MASK_STRING_SIMPLE: {
            switch (lhs_mod) {
            case ObjectId::MOD_INLINE: {
                lhs_string_iter = std::make_unique<StringInlineIter>(
                    Inliner::decode<ObjectId::STR_INLINE_BYTES>(lhs_value)
                );
                break;
            }
            case ObjectId::MOD_EXTERNAL: {
                uint64_t external_id = lhs_oid.id & ObjectId::VALUE_MASK;
                lhs_string_iter = string_manager.get_char_iter(external_id);
                break;
            }
            case ObjectId::MOD_TMP: {
                uint64_t external_id = lhs_oid.id & ObjectId::VALUE_MASK;
                lhs_string_iter = tmp_manager.get_str_char_iter(external_id);
                break;
            }
            }

            switch (rhs_mod) {
            case ObjectId::MOD_INLINE: {
                rhs_string_iter = std::make_unique<StringInlineIter>(
                    Inliner::decode<ObjectId::STR_INLINE_BYTES>(rhs_value)
                );
                break;
            }
            case ObjectId::MOD_EXTERNAL: {
                uint64_t external_id = rhs_oid.id & ObjectId::VALUE_MASK;
                rhs_string_iter = string_manager.get_char_iter(external_id);
                break;
            }
            case ObjectId::MOD_TMP: {
                uint64_t external_id = rhs_oid.id & ObjectId::VALUE_MASK;
                rhs_string_iter = tmp_manager.get_str_char_iter(external_id);
                break;
            }
            }
            break;
        }
        }

        return string_manager.compare(*lhs_string_iter, *rhs_string_iter);
    }
    case GQL_OID::GenericType::NUMERIC: {
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

                int64_t diff = lhs_sep - rhs_sep;
                int64_t div = static_cast<int64_t>(std::pow(10, diff));
                int64_t division = lhs_num / div;
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
    case GQL_OID::GenericType::DATE: {
        DateTime lhs_dt(lhs_oid);
        DateTime rhs_dt(rhs_oid);

        if constexpr (mode == Comparisons::Mode::Strict) {
            return lhs_dt.compare<DateTimeComparisonMode::Strict>(rhs_dt, error);
        } else if constexpr (mode == Comparisons::Mode::Normal) {
            return lhs_dt.compare<DateTimeComparisonMode::Normal>(rhs_dt, error);
        }
    }
    case GQL_OID::GenericType::BOOL: {
        return static_cast<int64_t>(lhs_oid.id & 1) - static_cast<int64_t>(rhs_oid.id & 1);
    }
    default: {
        return static_cast<int64_t>(lhs_oid.id & ObjectId::VALUE_MASK)
             - static_cast<int64_t>(rhs_oid.id & ObjectId::VALUE_MASK);
    }
    }
}

template int64_t
    Comparisons::_compare<Comparisons::Mode::Normal, true>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
template int64_t
    Comparisons::_compare<Comparisons::Mode::Strict, true>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
template int64_t
    Comparisons::_compare<Comparisons::Mode::Normal, false>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
template int64_t
    Comparisons::_compare<Comparisons::Mode::Strict, false>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
