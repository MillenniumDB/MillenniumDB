#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_expr_printer.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace GQL {
class AggCollectDistinct : public Agg {
public:
    using Agg::Agg;

    AggCollectDistinct(VarId var_id, std::unique_ptr<BindingExpr> expr) :
        Agg (var_id, std::move(expr)),
        hash_table(1) { }

    void begin() override {
        hash_table.reset();
        values.clear();
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            oid_vec[0] = oid;
            if (!hash_table.is_in_or_insert(oid_vec)) {
                values.push_back(oid);
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_list(values);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COLLECT(DISTINCT ";
        BindingExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")";
        return os;
    }

private:
    std::vector<ObjectId> values;

    DistinctBindingHash hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec{1};
};
} // namespace GQL
