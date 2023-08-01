#pragma once

#include "graph_models/object_id.h"

namespace MQL {

class Comparisons {
public:
    static int64_t compare(ObjectId lhs, ObjectId rhs);
};
} // namespace MQL
