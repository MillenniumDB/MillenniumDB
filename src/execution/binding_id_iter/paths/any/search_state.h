#pragma once

#include <memory>

#include "base/ids/id.h"
#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"

namespace Paths { namespace Any {

struct DFSSearchState {
    const ObjectId node_id;
    const uint32_t state;
    uint32_t current_transition = 0;
    std::unique_ptr<IndexIterator> iter = std::make_unique<NullIndexIterator>();  // Iterator
    DFSSearchState(ObjectId node_id, uint32_t state) : node_id(node_id), state(state) { }
};
}} // namespace Paths::Any
