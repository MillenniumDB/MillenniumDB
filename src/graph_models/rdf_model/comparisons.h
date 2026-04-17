#pragma once

#include "graph_models/object_id.h"

namespace SPARQL {

enum class CompMode {
    Normal,
    Strict
};

class Comparisons {
public:
    static int64_t compare(ObjectId lhs, ObjectId rhs)
    {
        return _compare<CompMode::Normal>(lhs, rhs);
    }

    static int64_t strict_compare(ObjectId lhs, ObjectId rhs, bool* error)
    {
        return _compare<CompMode::Strict>(lhs, rhs, error);
    }

private:
    template<CompMode mode>
    static int64_t _compare(ObjectId lhs, ObjectId rhs, bool* error = nullptr);
};
} // namespace SPARQL
