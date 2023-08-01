#pragma once

#include <cstdint>
#include <memory>
#include <ostream>

#include "query/query_context.h"
#include "query/executor/binding_iter/paths/path_manager.h"

class QueryExecutor {
public:
    virtual ~QueryExecutor() {
        // We always have a QueryExecutor as the root of our physical query plans.
        // If that changes we might need to call path_manager.clear() somewhere else
        // (it needs to be called always at the destruction of the query and only once)
        path_manager.clear();
    }

    // returns how many results were obtained
    virtual uint64_t execute(std::ostream&) = 0;

    virtual void analyze(std::ostream&, int indent = 0) const = 0;
};
