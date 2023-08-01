#pragma once

#include <ostream>

#include "query/executor/binding.h"
#include "query/query_context.h"

// Abstract class
class BindingIter {
public:
    virtual ~BindingIter() = default;

    // parent_binding is the input and the iter will write its results there.
    // It will look at the parent_binding to know the value of the assigned variables
    virtual void begin(Binding& parent_binding) = 0;

    // Iterator starts again.
    // It will look at the parent_binding to know the value of the assigned variables
    virtual void reset() = 0;

    // Returns true if there is a next binding or false otherwise.
    // It modifies the parent_binding to include the new results.
    virtual bool next() = 0;

    // Every var that the iter sets in the binding when next() returns true is setted to null
    virtual void assign_nulls() = 0;

    // prints execution statistics
    virtual void analyze(std::ostream&, int indent = 0) const = 0;
};
