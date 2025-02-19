#pragma once

#include "query/exceptions.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"
#include <algorithm>

namespace GQL {
//Returns a continuous percentile value interpolated between two values in the ordered dataset.
//The percentile value (IVERE) must be between 0 and 1.
//Formula:
//   INDEX = 1 + (IVERE * (N_ORDERED_VALUES - 1))
//   RESULT = (INDEX_CEILING - INDEX) * RESULT_FLOOR + (INDEX - INDEX_FLOOR) * RESULT_CEILING
//where RESULT_FLOOR is the value at the position INDEX_FLOOR and RESULT_CEILING is the value at the position INDEX_CEILING
//ISO IEC 39075-2024 subchapter 20.9
class AggPercentileCont : public Agg {
public:
using Agg::Agg;

    AggPercentileCont(VarId var_id, std::unique_ptr<BindingExpr> expr, std::unique_ptr<BindingExpr> percentile) :
        Agg (var_id, std::move(expr)),
        percentile (std::move(percentile)) { }

    void begin() override {
        values.clear();
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            values.push_back(Conversions::to_double(oid));
        }
    }

    // indicates the end of a group
    ObjectId get() override {
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

        std::sort(values.begin(), values.end());
        double rank = percentile_value * (values.size() - 1);
        size_t lower = static_cast<size_t>(rank);
        size_t upper = std::min(lower + 1, values.size() - 1);
        double fraction = rank - lower;

        double result = values[lower] + fraction * (values[upper] - values[lower]);
        return Conversions::pack_double(result);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "PERCENTILE_CONT(";
        BindingExprPrinter printer(os);
        percentile->accept_visitor(printer);
        os << ", ";
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

    std::unique_ptr<BindingExpr> percentile;

private:
    std::vector<double> values;
};
} // namespace GQL
