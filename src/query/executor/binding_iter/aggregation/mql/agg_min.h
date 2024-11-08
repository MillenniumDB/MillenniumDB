#pragma once

#include "graph_models/quad_model/comparisons.h"
#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/mql_binding_expr_printer.h"

namespace MQL {
class AggMin : public Agg {
public:
    AggMin(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg (var_id, std::move(expr)) { }

    void begin() override {
        min = Conversions::pack_int(Conversions::INTEGER_MAX);
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (!oid.is_null()) {
            auto cmp = Comparisons::compare(oid, min);
            if (cmp < 0) {
                min = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        if (min == Conversions::pack_int(Conversions::INTEGER_MAX)) {
            return ObjectId::get_null();
        }
        return min;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "MIN(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    ObjectId min;
};
} // namespace MQL
