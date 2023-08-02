#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {
class AggCountDistinct : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        hash_table = std::make_unique<DistinctBindingHash<ObjectId>>(1);
        count = 0;
    }

    void process() override {
        auto oid = expr->eval(*binding);
        if (oid.is_valid()) {
            oid_vec[0] = oid;
            if (!hash_table->is_in_or_insert(oid_vec)) {
                count++;
            }
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_int(count);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COUNT( DISTINCT " << *expr << ")";
        return os;
    }

private:
    uint64_t count = 0;

    std::unique_ptr<DistinctBindingHash<ObjectId>> hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec{1};
};
}// namespace SPARQL
