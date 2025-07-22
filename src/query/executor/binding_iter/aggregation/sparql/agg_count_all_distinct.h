#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {
class AggCountAllDistinct : public Agg {
public:
    using Agg::Agg;

    AggCountAllDistinct(
        VarId                        var_id,
        std::unique_ptr<BindingExpr> expr,
        std::vector<VarId>&&         _basic_vars
    ) :
        Agg (var_id, std::move(expr)),
        hash_table (_basic_vars.size()),
        basic_vars (std::move(_basic_vars))
    {
        oid_vec.resize(basic_vars.size());
    }

    void begin() override {
        hash_table.reset();
        count = 0;
    }

    void process() override {
        for (size_t i = 0; i < basic_vars.size(); i++) {
            oid_vec[i] = (*binding)[basic_vars[i]];
        }

        if (!hash_table.is_in_or_insert(oid_vec)) {
            count++;
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        return Conversions::pack_int(count);
    }

    std::ostream& print(std::ostream& os) const override {
        os << "COUNT(DISTINCT *)";
        return os;
    }

private:
    uint64_t count = 0;

    DistinctBindingHash hash_table;

    // Necessary because of the DistinctBindingHash interface
    // The basic graph pattern variables that are used to evaluated DISTINCT.
    // Internal variables, variables declared in SELECT or GROUP BY are not used to evaluate DISTINCT
    std::vector<VarId> basic_vars;

    // Vector to pass variable values to the hash table
    std::vector<ObjectId> oid_vec;
};
} // namespace SPARQL
