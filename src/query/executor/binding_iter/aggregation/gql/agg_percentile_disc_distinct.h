#pragma once

#include <algorithm>

#include "graph_models/gql/conversions.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace GQL {
class AggPercentileDiscDistinct : public Agg {
public:
    using Agg::Agg;

    AggPercentileDiscDistinct(
        VarId var_id,
        std::unique_ptr<BindingExpr> expr,
        std::unique_ptr<BindingExpr> percentile
    ) :
        Agg(var_id, std::move(expr)),
        percentile(std::move(percentile)),
        hash_table(1)
    { }

    void begin() override
    {
        hash_table.reset();
        values.clear();
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            oid_vec[0] = oid;
            if (!hash_table.is_in_or_insert(oid_vec)) {
                values.push_back(oid);
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override
    {
        if (values.empty()) {
            return ObjectId::get_null();
        }

        double percentile_value;
        auto percentile_oid = percentile->eval(*binding);
        auto percentile_subtype = GQL_OID::get_generic_type(percentile_oid);
        if (percentile_subtype == GQL_OID::GenericType::NUMERIC) {
            percentile_value = Conversions::to_double(percentile_oid);
            if (percentile_value < 0 || percentile_value > 1) {
                throw QueryExecutionException("data exception — percentile must be a number between 0 and 1");
            }
        } else {
            throw QueryExecutionException("data exception — percentile must be a number between 0 and 1");
        }

        std::sort(values.begin(), values.end(), [](const ObjectId& a, const ObjectId& b) {
            return Conversions::to_double(a) < Conversions::to_double(b);
        });

        size_t index = static_cast<size_t>(std::floor(1 + percentile_value * (values.size() - 1)));
        index = std::min(index - 1, values.size() - 1);

        return values[index];
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "PERCENTILE_DISC(DISTINCT ";
        BindingExprPrinter printer(os);
        printer.print(*percentile);
        os << ", ";
        printer.print(*expr);
        os << ")";
        return os;
    }

    std::unique_ptr<BindingExpr> percentile;

private:
    std::vector<ObjectId> values;

    DistinctBindingHash hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec { 1 };
};
} // namespace GQL
