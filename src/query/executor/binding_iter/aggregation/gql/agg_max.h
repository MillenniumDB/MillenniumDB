#pragma once

#include "graph_models/gql/comparisons.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"

namespace GQL {
class AggMax : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        max = ObjectId::get_null();
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            auto cmp = GQL::Comparisons::compare_null_first(oid, max);
            if (cmp > 0) {
                max = oid;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return max;
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "MAX(";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    ObjectId max;
};
} // namespace GQL
