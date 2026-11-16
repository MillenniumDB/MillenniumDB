#pragma once

#include <memory>

#include "path_binding.h"
#include "query/executor/binding.h"
#include "query/id.h"

class PathBindingIter {
public:
    virtual ~PathBindingIter() = default;

    virtual void begin(Binding&) = 0;
    virtual void reset() = 0;
    virtual void assign_nulls() = 0;

    // Assigns an empty list to each variable contained in the iterator
    virtual void assign_empty() = 0;

    // Resets the iterator to the paths that can be concatenated to a path ending at left_boundary
    virtual void set_left_boundary(const PathElement& left_boundary) = 0;

    // Assigns the rightmost variable and resets the iterator
    virtual void set_right_boundary(ObjectId left_boundary) = 0;

    virtual std::unique_ptr<PathBinding> next() = 0;
    virtual void print(std::ostream& os, int indent, bool stats) = 0;
};
