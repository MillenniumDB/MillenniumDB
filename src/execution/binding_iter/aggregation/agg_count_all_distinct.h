#pragma once

#include <memory>

#include "execution/graph_object/graph_object_factory.h"
#include "execution/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class AggCountAllDistinct : public Agg {
public:
    AggCountAllDistinct(std::vector<VarId>&& var_ids) :
        var_ids (std::move(var_ids)) { }

    void begin() override {
        extendable_table = std::make_unique<DistinctBindingHash<GraphObject>>(var_ids.size());
        count = 0;
        tuple.clear();
        for (uint_fast32_t i = 0; i < var_ids.size(); i++) {
            tuple.push_back(GraphObjectFactory::make_null());
        }
    }

    void process() override;

    // indicates the end of a group
    GraphObject get() override {
        return GraphObjectFactory::make_int(count);
    }

private:
    int64_t count;

    std::vector<VarId> var_ids;

    std::vector<GraphObject> tuple;

    std::unique_ptr<DistinctBindingHash<GraphObject>> extendable_table;
};
