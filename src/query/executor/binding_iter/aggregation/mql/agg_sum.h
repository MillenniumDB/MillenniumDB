#pragma once

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace MQL {
class AggSum : public Agg {
public:
    using Agg::Agg;

    void begin() override {
        sum = 0;
    }

    void process() override {
        auto oid = expr->eval(*binding);
        auto type = Conversions::calculate_optype(oid);

        if (type == Conversions::OpType::INTEGER) {
            sum += Conversions::unpack_int(oid);
        } else if (type == Conversions::OpType::FLOAT) {
            sum += Conversions::to_float(oid);
        } else {
            // Ignore
            return;
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        float sum_f = sum;
        return Conversions::pack_float(sum_f);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "SUM(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    double sum;
};
} // namespace MQL
