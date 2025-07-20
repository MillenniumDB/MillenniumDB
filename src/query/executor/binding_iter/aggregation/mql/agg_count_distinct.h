#pragma once

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace MQL {
class AggCountDistinct : public Agg {
public:
    AggCountDistinct(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg (var_id, std::move(expr)),
        hash_table (1) { }

    void begin() override {
        hash_table.reset();
        count = 0;
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (!oid.is_null()) {
            oid_vec[0] = oid;
            if (!hash_table.is_in_or_insert(oid_vec)) {
                count++;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_int(count);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COUNT( DISTINCT ";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    int64_t count = 0;

    DistinctBindingHash hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec{1};
};
} // namespace MQL
