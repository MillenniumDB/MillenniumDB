#pragma once

#include "execution/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class AggAvg : public Agg {
public:
    AggAvg(VarId var_id) : var_id (var_id) { }

    void begin() override {
        count = 0;
        sum = 0;
    }

    void process() override {
        auto graph_obj = (*binding_iter)[var_id];
        if (graph_obj.type == GraphObjectType::INT) {
            ++count;
            sum += graph_obj.value.i;
        } else if (graph_obj.type == GraphObjectType::FLOAT) {
            ++count;
            sum += graph_obj.value.f;
        }
    }

    // indicates the end of a group
    GraphObject get() override {
        if (count == 0) {
            return GraphObject::make_null();
        }
        return GraphObject::make_float(float(sum/count));
    }

private:
    VarId var_id;

    double sum;

    uint64_t count;
};
