#pragma once

#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"
#include "query/executor/binding_iter_visitor.h"


// Abstract class
class BindingIter {
protected:
    virtual void _begin(Binding& parent_binding) = 0;
    virtual bool _next() = 0;
    virtual void _reset() = 0;

public:
    uint64_t stat_begin = 0;
    uint64_t stat_next = 0;
    uint64_t stat_reset = 0;

    // Starts with -1 to account for the last next being false
    uint64_t results = 0;
    // uint64_t elapsed_time;

    virtual ~BindingIter() = default;

    // parent_binding is the input and the iter will write its results there.
    // It will look at the parent_binding to know the value of the assigned variables
    inline void begin(Binding& parent_binding) {
        stat_begin++;
        _begin(parent_binding);
    }

    // Iterator starts again.
    // It will look at the parent_binding to know the value of the assigned variables
    inline void reset() {
        stat_reset++;
        _reset();
    }

    // Returns true if there is a next binding or false otherwise.
    // It modifies the parent_binding to include the new results.
    inline bool next() {
        stat_next++;

        bool result = _next();
        results += static_cast<uint64_t>(result);
        return result;
    }

    // Every var that the iter sets in the binding when next() returns true is set to null
    virtual void assign_nulls() = 0;

    virtual void accept_visitor(BindingIterVisitor&) = 0;
};
