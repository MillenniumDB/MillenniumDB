#pragma once

#include "graph_models/object_id.h"

namespace MQL {

class Comparisons {
public:
    static int64_t compare(ObjectId lhs, ObjectId rhs);

    static int64_t strict_compare(ObjectId lhs, ObjectId rhs, bool* error);
};
} // namespace MQL
