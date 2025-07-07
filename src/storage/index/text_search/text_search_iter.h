#pragma once

#include <cstdint>

namespace TextSearch {

// Common interface for MultiIter and IndexIter, returned by the Searches
// This is an optimization so that we can return a simple IndexIter
// (which does not materialize) for single token queries.
class TextSearchIter {
public:
    // Returns true if there is a result
    virtual bool next() = 0;

    // Returns the table pointer associated with the current result
    virtual uint64_t get_table_pointer() const = 0;

    // Returns the score associated with the current result
    virtual uint64_t get_score() const = 0;

    virtual ~TextSearchIter() = default;
};

} // namespace TextSearch
