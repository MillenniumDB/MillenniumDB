#pragma once

#include "graph_models/object_id.h"

namespace SPARQL {

class Comparisons {
public:

    static int64_t compare(ObjectId lhs, ObjectId rhs) {
        return _compare<Mode::Normal>(lhs, rhs);
    }

    static int64_t strict_compare(ObjectId lhs, ObjectId rhs, bool* error) {
        return _compare<Mode::Strict>(lhs, rhs, error);
    }

private:
    enum class Mode { Normal, Strict };

    template<Mode mode>
    static int64_t _compare(ObjectId lhs, ObjectId rhs, bool* error = nullptr);
};
} // namespace SPARQL
