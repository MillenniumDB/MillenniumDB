#pragma once

#include <limits>

#include "execution/binding_iter/aggregation/agg.h"
#include "execution/graph_object/graph_object_types.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class AggMin : public Agg {
public:
    AggMin(VarId var_id) : var_id (var_id) { }

    void begin() override {
        min = std::numeric_limits<double>::infinity();
    }

    void process() override {
        auto graph_obj = (*binding_iter)[var_id];
        if (graph_obj.type == GraphObjectType::INT) {
            int64_t i = GraphObjectInterpreter::get<int64_t>(graph_obj);
            if (min == std::numeric_limits<double>::infinity()) {
                min = i;
            } else if (i < min) {
                min = i;
            }
        } else if (graph_obj.type == GraphObjectType::FLOAT) {
            float f = GraphObjectInterpreter::get<float>(graph_obj);
            if (min == std::numeric_limits<double>::infinity()) {
                min = f;
            } else if (f < min) {
                min = f;
            }
        }
    }

    // indicates the end of a group
    GraphObject get() override {
        if (min == std::numeric_limits<double>::infinity()) {
            return GraphObjectFactory::make_null();
        }
        return GraphObjectFactory::make_float(float(min));
    }

private:
    VarId var_id;

    double min;
};
