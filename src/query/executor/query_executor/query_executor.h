#pragma once

#include <cstdint>
#include <ostream>

class QueryExecutor {
public:
    virtual ~QueryExecutor() = default;

    // returns how many results were obtained
    virtual uint64_t execute(std::ostream&) = 0;

    virtual void analyze(std::ostream&, bool print_stats, int indent = 0) const = 0;
};
