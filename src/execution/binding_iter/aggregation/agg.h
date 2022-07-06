#pragma once

#include "base/binding/binding_iter.h"

class Agg {
public:
    virtual ~Agg() = default;

    virtual void set_binding_iter(BindingIter* iter) {
        binding_iter = iter;
    }

    virtual void begin() = 0;

    virtual void process() = 0;

    virtual GraphObject get() = 0;

protected:
    BindingIter* binding_iter;
};
