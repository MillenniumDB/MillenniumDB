#pragma once

#include <functional>

#include "graph_models/object_id.h"
#include "third_party/hashes/hash_function_wrapper.h"

namespace HNSW {
struct HNSWNode {
    ObjectId object_oid; // Object id itself
    ObjectId tensor_oid; // Tensor associated to the object_oid

    HNSWNode() = default;

    HNSWNode(ObjectId object_oid_, ObjectId tensor_oid_) :
        object_oid { object_oid_ },
        tensor_oid { tensor_oid_ }
    { }

    inline bool operator==(const HNSWNode& other) const
    {
        return object_oid == other.object_oid && tensor_oid == other.tensor_oid;
    }

    struct Hasher {
        size_t operator()(const HNSW::HNSWNode& node) const
        {
            return HashFunctionWrapper(reinterpret_cast<const char*>(&node), 2 * sizeof(ObjectId));
        }
    };
};

} // namespace HNSW
