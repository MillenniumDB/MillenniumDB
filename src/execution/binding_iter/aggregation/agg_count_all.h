#pragma once

#include "execution/binding_iter/aggregation/agg.h"

class AggCountAll : public Agg {
public:
    void begin() override {
        count = 0;
    }

    void process() override {
        count++;
    }

    // indicates the end of a group
    GraphObject get() override {
        return GraphObjectFactory::make_int(count);
    }

private:
    int64_t count = 0;
};
