#pragma once

#include "graph_models/rdf_model/comparisons.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/aggregation/sparql/uagg_max.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

namespace SPARQL {
class AggMax : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        max = ObjectId::get_null();
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            auto cmp = SPARQL::Comparisons::compare(oid, max);
            if (cmp > 0) {
                max = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return max;
    }

    std::ostream& print(std::ostream& os) const override {
        os << "MAX(";
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
        return std::make_unique<UAggMax>(var_id, expr.get());
    }

private:
    ObjectId max;
};
} // namespace SPARQL
