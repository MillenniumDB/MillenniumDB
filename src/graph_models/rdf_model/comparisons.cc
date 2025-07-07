#include "comparisons.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/query_context.h"
#include "system/string_manager.h"

using namespace SPARQL;

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

    auto lhs_gen_t = RDF_OID::get_generic_type(lhs_oid);
    auto rhs_gen_t = RDF_OID::get_generic_type(rhs_oid);

    if (lhs_gen_t != rhs_gen_t) {
        if constexpr (mode == Comparisons::Mode::Strict) {
            *error = true;
            return 0;
        } else {
            return static_cast<int64_t>(lhs_gen_t) - static_cast<int64_t>(rhs_gen_t);
        }
    }

    switch (lhs_gen_t) {
    case RDF_OID::GenericType::IRI: {
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
    case RDF_OID::GenericType::STRING: {
        auto lhs_sub_t = RDF_OID::get_generic_sub_type(lhs_oid);
        auto rhs_sub_t = RDF_OID::get_generic_sub_type(rhs_oid);

        if (lhs_sub_t != rhs_sub_t) {
            return static_cast<int64_t>(lhs_sub_t) - static_cast<int64_t>(rhs_sub_t);
        }

        auto lhs_buffer = get_query_ctx().get_buffer1();
        auto rhs_buffer = get_query_ctx().get_buffer2();

        size_t lhs_size;
        size_t rhs_size;

        switch (lhs_sub_t) {
        case RDF_OID::GenericSubType::STRING_SIMPLE:
        case RDF_OID::GenericSubType::STRING_XSD: {
            lhs_size = Conversions::print_string(lhs_oid, lhs_buffer);
            rhs_size = Conversions::print_string(rhs_oid, rhs_buffer);
            break;
        }
        case RDF_OID::GenericSubType::STRING_LANG: {
            auto lhs_tag = lhs_oid.id & ObjectId::MASK_LITERAL_TAG;
            auto rhs_tag = rhs_oid.id & ObjectId::MASK_LITERAL_TAG;

            if (lhs_tag != rhs_tag) {
                return static_cast<int64_t>(lhs_tag) - static_cast<int64_t>(rhs_tag);
            }

            lhs_size = Conversions::print_string_lang(lhs_oid, lhs_buffer);
            rhs_size = Conversions::print_string_lang(rhs_oid, rhs_buffer);
            break;
        }
        case RDF_OID::GenericSubType::STRING_DATATYPE: {
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
            throw LogicException("unexpected RDF_OID::GenericSubType at SPARQL::Comparisons::_compare");
        }

        return StringManager::compare(lhs_buffer, rhs_buffer, lhs_size, rhs_size);
    }
    case RDF_OID::GenericType::NUMERIC: {
        auto lhs_sub_t = lhs_oid.get_sub_type();
        auto rhs_sub_t = rhs_oid.get_sub_type();
        // Integer optimization
        if (lhs_sub_t == ObjectId::MASK_INT && rhs_sub_t == ObjectId::MASK_INT) {
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
    case RDF_OID::GenericType::DATE: {
        DateTime lhs_dt(lhs_oid);
        DateTime rhs_dt(rhs_oid);

        if constexpr (mode == Comparisons::Mode::Strict) {
            return lhs_dt.compare<DateTimeComparisonMode::Strict>(rhs_dt, error);
        } else if constexpr (mode == Comparisons::Mode::Normal){
            return lhs_dt.compare<DateTimeComparisonMode::Normal>(rhs_dt, error);
        }
    }
    case RDF_OID::GenericType::BOOL: {
        return static_cast<int64_t>(lhs_oid.id & 1) - static_cast<int64_t>(rhs_oid.id & 1);
    }
    case RDF_OID::GenericType::TENSOR: {
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


template int64_t Comparisons::_compare<Comparisons::Mode::Normal>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
template int64_t Comparisons::_compare<Comparisons::Mode::Strict>(ObjectId lhs_oid, ObjectId rhs_oid, bool* error);
