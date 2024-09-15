#pragma once

#include "graph_models/quad_model/comparisons.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/mql_binding_expr_printer.h"

namespace MQL {
class AggMax : public Agg {
public:
    AggMax(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg (var_id, std::move(expr)) { }

    void begin() override {
        max = ObjectId::get_null();
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (!oid.is_null()) {
            auto cmp = Comparisons::compare(oid, max);
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
} // namespace MQL
