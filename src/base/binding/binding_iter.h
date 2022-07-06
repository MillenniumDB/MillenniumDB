#pragma once

#include <ostream>

#include "base/graph_object/graph_object.h"
#include "base/ids/var_id.h"

// Abstract class
class BindingIter {
public:
    virtual ~BindingIter() = default;

    // begin has to be called before calling next()
    virtual void begin(std::ostream&) = 0;

    // returns true if there are more bindings or false otherwise.
    virtual bool next() = 0;

    // gets the current value of a var for the current binding
    virtual GraphObject operator[](VarId var_id) const = 0;

    // prints execution statistics into an ostream
    virtual void analyze(std::ostream&, int indent = 0) const = 0;
};
