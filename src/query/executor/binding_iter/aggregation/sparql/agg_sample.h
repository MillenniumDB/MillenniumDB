#pragma once

#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/aggregation/sparql/uagg_sample.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace SPARQL {
class AggSample : public Agg {
public:
    using Agg::Agg;
    void begin() override
    {
        sample = ObjectId::get_null();
    }

    void process() override
    {
        if (!sample.is_null()) {
            return;
        }
        sample = expr->eval(*binding);
    }

    // indicates the end of a group
    ObjectId get() override
    {
        return sample;
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "SAMPLE(";
        BindingExprPrinter printer(os);
        printer.print(*expr);
        os << ")";
        return os;
    }

    bool is_pipelineble() const override
    {
        return true;
    }

    std::unique_ptr<UAgg> get_uagg() override
    {
        return std::make_unique<UAggSample>(var_id, expr.get());
    }

private:
    ObjectId sample;
};
} // namespace SPARQL
