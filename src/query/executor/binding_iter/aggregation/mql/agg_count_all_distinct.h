#pragma once

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace MQL {
class AggCountAllDistinct : public Agg {
public:
    AggCountAllDistinct(VarId var_id, std::unique_ptr<BindingExpr> expr, std::vector<VarId>&& _vars) :
        Agg(var_id, std::move(expr)),
        hash_table(_vars.size()),
        vars(std::move(_vars))
    {
        oid_vec.resize(vars.size());
    }

    void begin() override
    {
        hash_table.reset();
        count = 0;
    }

    void process() override
    {
        for (size_t i = 0; i < vars.size(); i++) {
            oid_vec[i] = (*binding)[vars[i]];
        }

        if (!hash_table.is_in_or_insert(oid_vec)) {
            count++;
        }
    }

    ObjectId get() override
    {
        return Conversions::pack_int(count);
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "COUNT(DISTINCT *)";
        return os;
    }

private:
    uint64_t count = 0;

    DistinctBindingHash hash_table;

    std::vector<VarId> vars;

    // Vector to pass variable values to the hash table
    std::vector<ObjectId> oid_vec;
};
} // namespace MQL
