#pragma once

#include "base/path_printer.h"

// Path class handles logical operations for property paths
class Path {
public:
    // This static variable needs to be setted at server initialization in order to be able to print paths
    static PathPrinter* path_printer; // TODO: put elsewhere

    uint64_t path_id;

    Path(uint64_t path_id) : path_id(path_id) { }
};
