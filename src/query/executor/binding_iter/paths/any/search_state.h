#pragma once

#include <memory>

#include "query/id.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"

namespace Paths { namespace Any {

struct DFSSearchState {
    const ObjectId node_id;
    const uint32_t state;
    uint32_t current_transition = 0;
    std::unique_ptr<EdgeIter> iter = std::make_unique<NullIndexIterator>();  // Iterator
    DFSSearchState(ObjectId node_id, uint32_t state) : node_id(node_id), state(state) { }
};
}} // namespace Paths::Any
