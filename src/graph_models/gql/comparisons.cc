#include "comparisons.h"

#include "graph_models/gql/conversions.h"
#include "query/query_context.h"
#include "system/string_manager.h"

#include <cassert>

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

    auto lhs_gen_t = lhs_oid.generic_type();
    auto rhs_gen_t = rhs_oid.generic_type();

    if (lhs_gen_t != rhs_gen_t) {
        if constexpr (mode == Comparisons::Mode::Strict) {
            *error = true;
            return 0;
        } else {
            return static_cast<int64_t>(lhs_gen_t) - static_cast<int64_t>(rhs_gen_t);
        }
    }

    switch (lhs_gen_t) {
    case ObjectGenType::String: {
        auto lhs_buffer = get_query_ctx().get_buffer1();
        auto rhs_buffer = get_query_ctx().get_buffer2();

        auto lhs_size = Conversions::print_string(lhs_oid, lhs_buffer);
        auto rhs_size = Conversions::print_string(rhs_oid, rhs_buffer);

        return StringManager::compare(lhs_buffer, rhs_buffer, lhs_size, rhs_size);
    }
    case ObjectGenType::Numeric: {
        auto lhs_sub_t = lhs_oid.subtype();
        auto rhs_sub_t = rhs_oid.subtype();
        // Integer optimization
        if (lhs_sub_t == ObjectSubType::Int && rhs_sub_t == ObjectSubType::Int) {
            return static_cast<int64_t>(lhs_oid.id) - static_cast<int64_t>(rhs_oid.id);
        }

        auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case Conversions::OpType::INTEGER: {
            throw LogicException("This should have been handled by the Integer optimization.");
        }
        case Conversions::OpType::DECIMAL: {
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
        case Conversions::OpType::FLOAT: {
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
        case Conversions::OpType::DOUBLE: {
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
    case ObjectGenType::TemporalLiteral: {
        DateTime lhs_dt(lhs_oid);
        DateTime rhs_dt(rhs_oid);

        if constexpr (mode == Comparisons::Mode::Strict) {
            return lhs_dt.compare<DTCompare::Strict>(rhs_dt, error);
        } else if constexpr (mode == Comparisons::Mode::Normal) {
            return lhs_dt.compare<DTCompare::Normal>(rhs_dt, error);
        }
    }
    case ObjectGenType::Bool: {
        return static_cast<int64_t>(lhs_oid.id & 1) - static_cast<int64_t>(rhs_oid.id & 1);
    }
    case ObjectGenType::List: {
        std::vector<ObjectId> lhs_list = Conversions::unpack_list(lhs_oid);
        std::vector<ObjectId> rhs_list = Conversions::unpack_list(rhs_oid);

        if (lhs_list == rhs_list) {
            return 0;
        } else {
            return static_cast<int64_t>(lhs_oid.id & ObjectId::VALUE_MASK)
                 - static_cast<int64_t>(rhs_oid.id & ObjectId::VALUE_MASK);
        }
    }
    case ObjectGenType::Dict: {
        std::unique_ptr<Dictionary> lhs_dict = Common::Conversions::unpack_dictionary(lhs_oid);
        std::unique_ptr<Dictionary> rhs_dict = Common::Conversions::unpack_dictionary(rhs_oid);
        Dictionary& lhs(*lhs_dict);
        Dictionary& rhs(*rhs_dict);
        if (lhs == rhs) {
            return 0;
        } else {
            return static_cast<int64_t>(lhs_oid.id & ObjectId::VALUE_MASK)
                 - static_cast<int64_t>(rhs_oid.id & ObjectId::VALUE_MASK);
        }
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
