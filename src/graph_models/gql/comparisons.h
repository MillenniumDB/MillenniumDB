#pragma once

#include "graph_models/object_id.h"

namespace GQL {

class Comparisons {
public:
    static int64_t compare_null_first(ObjectId lhs, ObjectId rhs)
    {
        return _compare<Mode::Normal, true>(lhs, rhs);
    }

    static int64_t strict_compare_null_first(ObjectId lhs, ObjectId rhs, bool* error)
    {
        return _compare<Mode::Strict, true>(lhs, rhs, error);
    }

    static int64_t compare_null_last(ObjectId lhs, ObjectId rhs)
    {
        return _compare<Mode::Normal, false>(lhs, rhs);
    }

    static int64_t strict_compare_null_last(ObjectId lhs, ObjectId rhs, bool* error)
    {
        return _compare<Mode::Strict, false>(lhs, rhs, error);
    }

private:
    enum class Mode {
        Normal,
        Strict
    };

    template<Mode mode, bool null_first>
    static int64_t _compare(ObjectId lhs, ObjectId rhs, bool* error = nullptr);
};
} // namespace GQL
