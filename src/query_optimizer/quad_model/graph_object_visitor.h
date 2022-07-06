#pragma once

#include "base/graph_object/graph_object.h"
#include "base/ids/object_id.h"

struct GraphObjectVisitor {
    const bool create_if_not_exists;

    GraphObjectVisitor(bool create_if_not_exists);

    ObjectId operator()(const GraphObject&)  const;
};
