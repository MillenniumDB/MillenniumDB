#pragma once

#include <cstdint>

#include "graph_models/object_id.h"

// Returns a collision free hash for the ObjectId
inline uint64_t shuffle_hash(ObjectId object_id)
{
    // TODO: Change identity hash?
    return object_id.id;
}