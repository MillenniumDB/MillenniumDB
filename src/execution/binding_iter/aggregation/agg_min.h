#pragma once

#include <limits>

#include "execution/binding_iter/aggregation/agg.h"
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
            if (min == std::numeric_limits<double>::infinity()) {
                min = graph_obj.value.i;
            } else if (graph_obj.value.i < min) {
                min = graph_obj.value.i;
            }
        } else if (graph_obj.type == GraphObjectType::FLOAT) {
            if (min == std::numeric_limits<double>::infinity()) {
                min = graph_obj.value.f;
            } else if (graph_obj.value.f < min) {
                min = graph_obj.value.f;
            }
        }
    }

    // indicates the end of a group
    GraphObject get() override {
        if (min == std::numeric_limits<double>::infinity()) {
            return GraphObject::make_null();
        }
        return GraphObject::make_float(float(min));
    }

private:
    VarId var_id;

    double min;
};
