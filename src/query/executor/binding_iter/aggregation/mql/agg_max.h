#pragma once

#include "graph_models/quad_model/comparisons.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace MQL {
class AggMax : public Agg {
public:
    using Agg::Agg;

    void begin() override
    {
        max = ObjectId::get_null();
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        if (!oid.is_null()) {
            auto cmp = Comparisons::compare(oid, max);
            if (cmp > 0) {
                max = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override
    {
        return max;
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "MAX(";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }

private:
    ObjectId max;
};
} // namespace MQL
