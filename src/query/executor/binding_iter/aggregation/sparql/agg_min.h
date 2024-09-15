#pragma once

#include "graph_models/rdf_model/comparisons.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {
class AggMin : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        min = Conversions::pack_int(Conversions::INTEGER_MAX);
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            auto cmp = SPARQL::Comparisons::compare(oid, min);
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
} // namespace SPARQL
