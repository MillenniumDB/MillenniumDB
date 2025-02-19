#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"

namespace GQL {
//Returns a list of all values from the specified expression
//ISO IEC 39075-2024 subchapter 20.9
class AggCollect : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        values.clear();
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            values.push_back(oid);
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_list(values);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COLLECT(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    std::vector<ObjectId> values;
};
} // namespace GQL
