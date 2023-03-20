#pragma once

#include "execution/binding_iter/aggregation/agg.h"
#include "execution/graph_object/graph_object_types.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class AggSum : public Agg {
public:
    AggSum(VarId var_id) : var_id (var_id) { }

    void begin() override {
        sum = 0;
    }

    void process() override {
        auto graph_obj = (*binding_iter)[var_id];
        if (graph_obj.type == GraphObjectType::INT) {
            sum += GraphObjectInterpreter::get<int64_t>(graph_obj);
        } else if (graph_obj.type == GraphObjectType::FLOAT) {
            sum += GraphObjectInterpreter::get<float>(graph_obj);
        }
    }

    // indicates the end of a group
    GraphObject get() override {
        return GraphObjectFactory::make_float(float(sum));
    }

private:
    VarId var_id;

    double sum;
};
