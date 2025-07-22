#pragma once

#include "graph_models/rdf_model/comparisons.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/aggregation/sparql/uagg_min.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace SPARQL {
class AggMin : public Agg {
public:
    using Agg::Agg;
    void begin() override
    {
        min = ObjectId::get_null();
    }

    void process() override
    {
        auto oid = expr->eval(*binding);
        if (min.is_null()) {
            min = oid;
        } else if (oid.is_valid()) {
            auto cmp = SPARQL::Comparisons::compare(oid, min);
            if (cmp < 0) {
                min = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override
    {
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

    bool is_pipelineble() const override
    {
        return true;
    }

    std::unique_ptr<UAgg> get_uagg() override
    {
        return std::make_unique<UAggMin>(var_id, expr.get());
    }

private:
    ObjectId min;
};
} // namespace SPARQL
