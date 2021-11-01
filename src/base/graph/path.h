#ifndef BASE__PATH_H_
#define BASE__PATH_H_

#include <cstdint>
#include <cstring>
#include <ostream>

#include "base/graph/path_printer.h"


// Path class handles logical operations for property paths


class Path {
public:
    // This static variable needs to be setted at server initialization in order to be able to print paths
    static PathPrinter* path_printer;

    uint64_t path_id;

    Path(uint64_t path_id) :
        path_id (path_id) { }

    ~Path() = default;

    inline bool operator==(const Path& rhs) const noexcept {
        return this->path_id == rhs.path_id;
    }

    inline bool operator!=(const Path& rhs) const noexcept {
        return this->path_id != rhs.path_id;
    }

    inline bool operator<=(const Path& rhs) const noexcept {
        return this->path_id <= rhs.path_id;
    }

    inline bool operator>=(const Path& rhs) const noexcept {
        return this->path_id >= rhs.path_id;
    }

    inline bool operator<(const Path& rhs) const noexcept {
        return this->path_id < rhs.path_id;
    }

    inline bool operator>(const Path& rhs) const noexcept {
        return this->path_id > rhs.path_id;
    }
};

#endif // BASE__PATH_H_
