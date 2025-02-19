#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"

namespace GQL {
class AggCount : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        count = 0;
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            count++;
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_int(count);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COUNT(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    uint64_t count = 0;
};
} // namespace GQL
