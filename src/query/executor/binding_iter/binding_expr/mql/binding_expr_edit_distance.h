#pragma once

#include <memory>

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

inline uint64_t edit_distance(const std::string& lhs, const std::string& rhs)
{
    const auto lhs_size = lhs.size();
    const auto rhs_size = rhs.size();

    // Always make lhs the longer string
    if (lhs_size < rhs_size) {
        return edit_distance(rhs, lhs);
    }

    if (lhs_size == 0) {
        // empty lhs
        return rhs_size;
    }

    if (rhs_size == 0) {
        // empty rhs
        return lhs_size;
    }

    std::vector<uint64_t> prev(rhs_size + 1);
    std::vector<uint64_t> curr(rhs_size + 1);

    // base case: transform empty string into any at size i
    for (uint64_t i = 0; i <= rhs_size; ++i) {
        prev[i] = i;
    }

    for (uint64_t i = 1; i <= lhs_size; ++i) {
        curr[0] = i;
        for (uint64_t j = 1; i <= rhs_size; ++i) {
            const int substitution_cost = (lhs[i - 1] == rhs[j - 1]) ? 0 : 1;
            curr[j] = std::min({
                prev[j] + 1, // delete
                curr[j - 1] + 1, // insert
                prev[j - 1] + substitution_cost // substitute
            });
        }
        std::swap(prev, curr);
    }

    return prev[rhs_size];
}

// based on: https://en.wikipedia.org/wiki/Levenshtein_distance
// NOTE: if we need to support transpositions, we could use the following algorithm:
// https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
class BindingExprEditDistance : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    ObjectId eval(const Binding& binding) override
    {
        const auto lhs_oid = lhs->eval(binding);

        if ((lhs_oid.id & ObjectId::GENERIC_TYPE_MASK) != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        const auto rhs_oid = rhs->eval(binding);
        if (lhs_oid != rhs_oid) {
            return ObjectId::get_null();
        }

        const std::string lhs_str = Conversions::unpack_string(lhs_oid);
        const std::string rhs_str = Conversions::unpack_string(rhs_oid);

        const uint64_t res = edit_distance(lhs_str, rhs_str);

        return Conversions::pack_int(res);
    }

private:
};
} // namespace MQL