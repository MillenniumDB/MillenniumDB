#pragma once

#include "execution/binding_iter/aggregation/agg.h"
#include "execution/graph_object/graph_object_factory.h"
#include "execution/graph_object/graph_object_types.h"
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
            sum += GraphObjectInterpreter::get<int64_t>(graph_obj);
        } else if (graph_obj.type == GraphObjectType::FLOAT) {
            ++count;
            sum += GraphObjectInterpreter::get<float>(graph_obj);

        }
    }

    // indicates the end of a group
    GraphObject get() override {
        if (count == 0) {
            return GraphObjectFactory::make_null();
        }
        return GraphObjectFactory::make_float(float(sum/count));
    }

private:
    VarId var_id;

    double sum;

    uint64_t count;
};
