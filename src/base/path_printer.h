#pragma once

#include <ostream>

class PathPrinter {
public:
    virtual ~PathPrinter() = default;

    virtual void print(std::ostream& os, uint64_t path_id) const = 0;
};
