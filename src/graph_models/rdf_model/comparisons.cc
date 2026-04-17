#include "comparisons.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/query_context.h"
#include "system/string_manager.h"

#include <algorithm>
#include <cassert>

using namespace SPARQL;

// returns negative number if lhs < rhs,
// returns 0 if lhs == rhs
// returns positive number if lhs > rhs
// For operators use Normal mode (default).
// For expressions use Strict mode.
template<CompMode mode>
int64_t Comparisons::_compare(ObjectId lhs_oid, ObjectId rhs_oid, bool* error)
{
    if constexpr (mode == CompMode::Strict) {
        assert(error != nullptr);
        *error = false;
    }

    if (lhs_oid == rhs_oid) {
        return 0;
    }

    auto lhs_gen_t = lhs_oid.generic_type();
    auto rhs_gen_t = rhs_oid.generic_type();

    if (lhs_gen_t != rhs_gen_t) {
        if constexpr (mode == CompMode::Strict) {
            *error = true;
            return 0;
        } else {
            return static_cast<int64_t>(lhs_gen_t) - static_cast<int64_t>(rhs_gen_t);
        }
    }

    switch (lhs_gen_t) {
    case ObjectGenType::Iri: {
        // if different prefix we dont have to reconstruct the string
        auto lhs_prefix_id = lhs_oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);
        auto rhs_prefix_id = rhs_oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);

        auto& lhs_prefix = rdf_model.catalog.prefixes.get_prefix(lhs_prefix_id);
        auto& rhs_prefix = rdf_model.catalog.prefixes.get_prefix(rhs_prefix_id);

        auto shortest_prefix_size = std::min(lhs_prefix.size(), rhs_prefix.size());
        for (size_t i = 0; i < shortest_prefix_size; i++) {
            if (lhs_prefix[i] != rhs_prefix[i]) {
                return static_cast<int64_t>(lhs_prefix[i]) - static_cast<int64_t>(rhs_prefix[i]);
            }
        }

        auto lhs_buffer = get_query_ctx().get_buffer1();
        auto rhs_buffer = get_query_ctx().get_buffer2();

        auto lhs_size = Conversions::print_iri(lhs_oid, lhs_buffer);
        auto rhs_size = Conversions::print_iri(rhs_oid, rhs_buffer);

        return StringManager::compare(
            lhs_buffer + shortest_prefix_size,
            rhs_buffer + shortest_prefix_size,
            lhs_size - shortest_prefix_size,
            rhs_size - shortest_prefix_size
        );
    }
    case ObjectGenType::String: {
        auto lhs_sub_t = lhs_oid.subtype();
        auto rhs_sub_t = rhs_oid.subtype();

        if (lhs_sub_t != rhs_sub_t) {
            return static_cast<int64_t>(lhs_sub_t) - static_cast<int64_t>(rhs_sub_t);
        }

        auto lhs_buffer = get_query_ctx().get_buffer1();
        auto rhs_buffer = get_query_ctx().get_buffer2();

        size_t lhs_size;
        size_t rhs_size;

        switch (lhs_sub_t) {
        case ObjectSubType::String:
        case ObjectSubType::StringXsd: {
            lhs_size = Conversions::print_string(lhs_oid, lhs_buffer);
            rhs_size = Conversions::print_string(rhs_oid, rhs_buffer);
            break;
        }
        case ObjectSubType::StringLang: {
            auto lhs_tag = lhs_oid.id & ObjectId::MASK_LITERAL_TAG;
            auto rhs_tag = rhs_oid.id & ObjectId::MASK_LITERAL_TAG;

            if (lhs_tag != rhs_tag) {
                return static_cast<int64_t>(lhs_tag) - static_cast<int64_t>(rhs_tag);
            }

            lhs_size = Conversions::print_string_lang(lhs_oid, lhs_buffer);
            rhs_size = Conversions::print_string_lang(rhs_oid, rhs_buffer);
            break;
        }
        case ObjectSubType::StringDatatype: {
            auto lhs_tag = lhs_oid.id & ObjectId::MASK_LITERAL_TAG;
            auto rhs_tag = rhs_oid.id & ObjectId::MASK_LITERAL_TAG;

            if (lhs_tag != rhs_tag) {
                return static_cast<int64_t>(lhs_tag) - static_cast<int64_t>(rhs_tag);
            }

            lhs_size = Conversions::print_string_datatype(lhs_oid, lhs_buffer);
            rhs_size = Conversions::print_string_datatype(rhs_oid, rhs_buffer);
            break;
        }
        default:
            throw LogicException("unexpected ObjectSubType at SPARQL::Comparisons::_compare");
        }

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

        if constexpr (mode == CompMode::Strict) {
            return lhs_dt.compare<DTCompare::Strict>(rhs_dt, error);
        } else if constexpr (mode == CompMode::Normal) {
            return lhs_dt.compare<DTCompare::Normal>(rhs_dt, error);
        }
    }
    case ObjectGenType::Bool: {
        return static_cast<int64_t>(lhs_oid.id & 1) - static_cast<int64_t>(rhs_oid.id & 1);
    }
    case ObjectGenType::Tensor: {
        const auto optype = Conversions::calculate_optype(lhs_oid, rhs_oid);
        switch (optype) {
        case Conversions::OpType::TENSOR_FLOAT: {
            const auto lhs = Conversions::to_tensor<float>(lhs_oid);
            const auto rhs = Conversions::to_tensor<float>(rhs_oid);
            return tensor::Tensor<float>::compare(lhs, rhs);
        }
        case Conversions::OpType::TENSOR_DOUBLE: {
            const auto lhs = Conversions::to_tensor<double>(lhs_oid);
            const auto rhs = Conversions::to_tensor<double>(rhs_oid);
            return tensor::Tensor<double>::compare(lhs, rhs);
        }
        default: {
            throw LogicException("This should never happen");
        }
        }
    }
    default: {
        return static_cast<int64_t>(lhs_oid.id & ObjectId::VALUE_MASK)
             - static_cast<int64_t>(rhs_oid.id & ObjectId::VALUE_MASK);
    }
    }
}

template int64_t Comparisons::_compare<CompMode::Normal>(ObjectId lhs, ObjectId rhs, bool* error);
template int64_t Comparisons::_compare<CompMode::Strict>(ObjectId lhs, ObjectId rhs, bool* error);
