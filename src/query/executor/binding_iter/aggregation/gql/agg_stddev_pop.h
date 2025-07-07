#pragma once

#include <cmath>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"

namespace GQL {
// Returns the population standard deviation of all non-null numeric values in the given expression.
// Follows to ensure no loss of precision the rule "If DT is approximate numeric,
// then the declared type of the result is an approximate numeric type with precision not less than DT"
// ISO IEC 39075-2024 subchapter 20.9
// uses Welford's algorithm
class AggStddevPop : public Agg {
public:
    using Agg::Agg;
    void begin() override
    {
        count = 0;
        mean = 0;
        M2 = 0;
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        auto op_type = Conversions::calculate_optype(oid);

        switch (op_type) {
        case Common::Conversions::OpType::INTEGER:
        case Common::Conversions::OpType::DECIMAL:
        case Common::Conversions::OpType::FLOAT:
        case Common::Conversions::OpType::DOUBLE:
            break;
        case Common::Conversions::OpType::TENSOR_FLOAT:
        case Common::Conversions::OpType::TENSOR_DOUBLE:
        case Common::Conversions::OpType::INVALID:
            return;
        }

        double x = Conversions::to_double(oid);
        count++;
        auto delta = x - mean;
        mean += delta / count;
        auto delta2 = x - mean;
        M2 += delta * delta2;
    }

    // indicates the end of a group
    ObjectId get() override
    {
        if (count == 0) {
            return ObjectId::get_null();
        } else if (count == 1) {
            return Conversions::pack_int(0);
        }

        return Conversions::pack_double(sqrt(M2 / count));
    }

    std::ostream& print_to_ostream(std::ostream& os) const override
    {
        os << "STDDEV_POP(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    uint64_t count;
    double mean;
    double M2;
};
} // namespace GQL
