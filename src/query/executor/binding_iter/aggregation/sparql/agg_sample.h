#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"

namespace SPARQL {
class AggSample : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        sample = ObjectId::get_null();
        found_non_null = false;
    }

    void process() override {
        if (found_non_null) {
            return;
        }
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            sample = oid;
            found_non_null = true;
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return sample;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "SAMPLE(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    ObjectId sample;
    bool found_non_null;
};
} // namespace SPARQL
