#pragma once

#include "graph_models/quad_model/comparisons.h"
#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace MQL {
class AggMin : public Agg {
public:
    using Agg::Agg;

    void begin() override
    {
        min = Conversions::pack_int(Conversions::INTEGER_MAX);
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        if (!oid.is_null()) {
            auto cmp = Comparisons::compare(oid, min);
            if (cmp < 0) {
                min = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override
    {
        if (min == Conversions::pack_int(Conversions::INTEGER_MAX)) {
            return ObjectId::get_null();
        }
        return min;
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "MIN(";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }

private:
    ObjectId min;
};
} // namespace MQL
