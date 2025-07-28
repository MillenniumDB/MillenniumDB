#pragma once

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace MQL {
class AggCount : public Agg {
public:
    using Agg::Agg;

    void begin() override
    {
        count = 0;
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        if (!oid.is_null()) {
            count++;
        }
    }

    // indicates the end of a group
    ObjectId get() override
    {
        return Conversions::pack_int(count);
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "COUNT(";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }

private:
    int64_t count = 0;
};
} // namespace MQL
