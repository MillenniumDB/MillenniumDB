#pragma once

#include <cstdint>

namespace HNSW {

struct Entry {
    float distance;
    uint32_t node_id;

    Entry() = default;

    explicit Entry(float distance_, uint32_t node_id_) :
        distance { distance_ },
        node_id { node_id_ }
    { }
};

struct EntryLess {
    bool operator()(const Entry& a, const Entry& b) const noexcept
    {
        return a.distance < b.distance;
    }
};

} // namespace HNSW
