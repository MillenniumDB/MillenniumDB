#pragma once

#include "execution/binding_iter/aggregation/agg.h"

class AggCountVar : public Agg {
public:
    AggCountVar(VarId var_id) : var_id (var_id) { }

    void begin() override {
        count = 0;
    }

    void process() override {
        if ((*binding_iter)[var_id] != GraphObjectFactory::make_null() ) {
            count++;
        }
    }

    // indicates the end of a group
    GraphObject get() override {
        return GraphObjectFactory::make_int(count);
    }

private:
    VarId var_id;

    int64_t count = 0;
};
