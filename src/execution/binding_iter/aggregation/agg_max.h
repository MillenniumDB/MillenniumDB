#pragma once

#include <limits>

#include "execution/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

class AggMax : public Agg {
public:
    AggMax(VarId var_id) : var_id (var_id) { }

    void begin() override {
        max = std::numeric_limits<double>::infinity();
    }

    void process() override {
        auto graph_obj = (*binding_iter)[var_id];
        if (graph_obj.type == GraphObjectType::INT) {
            if (max == std::numeric_limits<double>::infinity()) {
                max = graph_obj.value.i;
            } else if (graph_obj.value.i > max) {
                max = graph_obj.value.i;
            }
        } else if (graph_obj.type == GraphObjectType::FLOAT) {
            if (max == std::numeric_limits<double>::infinity()) {
                max = graph_obj.value.f;
            } else if (graph_obj.value.f > max) {
                max = graph_obj.value.f;
            }
        }
    }

    // indicates the end of a group
    GraphObject get() override {
        if (max == std::numeric_limits<double>::infinity()) {
            return GraphObject::make_null();
        }
        return GraphObject::make_float(float(max));
    }

private:
    VarId var_id;

    double max;
};
