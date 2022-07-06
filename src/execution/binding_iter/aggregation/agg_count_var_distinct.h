#pragma once

#include "execution/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class AggCountVarDistinct : public Agg {
public:
    AggCountVarDistinct(VarId var_id) : var_id (var_id) { }

    void begin() override {
        count = 0;
        tuple = std::vector<GraphObject>(1);
        extendable_table = std::make_unique<DistinctBindingHash<GraphObject>>(1);
    }

    void process() override;

    // indicates the end of a group
    GraphObject get() override {
        return GraphObject::make_int(count);
    }

private:
    VarId var_id;

    int64_t count;

    std::vector<GraphObject> tuple;

    // TODO: should use a new class to not may for vector overhead
    std::unique_ptr<DistinctBindingHash<GraphObject>> extendable_table;
};
