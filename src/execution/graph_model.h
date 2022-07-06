#pragma once

#include <memory>

#include "base/binding/binding_iter.h"
#include "base/ids/object_id.h"
#include "base/graph_object/graph_object.h"
#include "base/thread/thread_info.h"
#include "parser/query/op/op.h"

// Abstract class
class GraphModel {
public:
    virtual ~GraphModel() = default;

    virtual std::unique_ptr<BindingIter> exec(Op&, ThreadInfo*) const = 0;

    virtual ObjectId get_object_id(const GraphObject&) const = 0;

    virtual GraphObject get_graph_object(ObjectId) const = 0;
};
