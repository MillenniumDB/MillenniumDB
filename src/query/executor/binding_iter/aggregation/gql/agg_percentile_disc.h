#pragma once

#include <algorithm>

#include "graph_models/gql/conversions.h"
#include "query/exceptions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"

namespace GQL {
//Returns the discrete percentile value from the ordered dataset.
//The percentile value (IVERE) must be between 0 and 1.
//Formula:
//   INDEX = 1 + (IVERE * (N_ORDERED_VALUES - 1))
//   If INDEX is an integer, RESULT = ORDERED_VALUES[INDEX]
//   Otherwise, RESULT = ORDERED_VALUES[rounded INDEX] (rounding up or down is up to implementation, here is rounded down)
//ISO IEC 39075-2024 subchapter 20.9
class AggPercentileDisc : public Agg {
public:
    using Agg::Agg;

    AggPercentileDisc(
        VarId var_id,
        std::unique_ptr<BindingExpr> expr,
        std::unique_ptr<BindingExpr> percentile
    ) :
        Agg(var_id, std::move(expr)),
        percentile(std::move(percentile))
    { }

    void begin() override
    {
        values.clear();
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            values.push_back(oid);
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

    std::ostream& print_to_ostream(std::ostream& os) const override
    {
        os << "PERCENTILE_DISC(";
        BindingExprPrinter printer(os);
        percentile->accept_visitor(printer);
        os << ", ";
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

    std::unique_ptr<BindingExpr> percentile;

private:
    std::vector<ObjectId> values;
};
} // namespace GQL
