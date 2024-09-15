#pragma once

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/mql_binding_expr_printer.h"

namespace MQL {
class AggAvg : public Agg {
public:
    AggAvg(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg (var_id, std::move(expr)) { }

    void begin() override {
        sum = 0;
        count = 0;
    }

    void process() override {
        auto oid = expr->eval(*binding);
        auto type = Conversions::calculate_optype(oid);

        if (type == Conversions::OPTYPE_INTEGER) {
            sum += Conversions::unpack_int(oid);
        } else if (type == Conversions::OPTYPE_FLOAT) {
            sum += Conversions::to_float(oid);
        } else {
            // Ignore
            return;
        }
        count++;
    }

    // indicates the end of a group
    ObjectId get() override {
        if (count == 0) {
            return Conversions::pack_int(0);
        }

        float avg = sum/count;
        return Conversions::pack_float(avg);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "AVG(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    double sum;

    uint64_t count;
};
} // namespace MQL
