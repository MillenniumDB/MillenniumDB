#pragma once

#include <set>

#include "graph_models/object_id.h"
#include "query/var_id.h"

class PathStep {
public:
    std::set<VarId> node_vars;
    ObjectId node_value;

    VarId edge_var = VarId(0);
    ObjectId edge_value = ObjectId(0);
    ObjectId edge_direction = ObjectId(0);

    PathStep(std::set<VarId> node_var, ObjectId node_value) :
        node_vars(node_var),
        node_value(node_value)
    { }

    PathStep(
        std::set<VarId> node_var,
        ObjectId node_value,
        VarId edge_var,
        ObjectId edge_value,
        ObjectId edge_direction
    ) :
        node_vars(node_var),
        node_value(node_value),
        edge_var(edge_var),
        edge_value(edge_value),
        edge_direction(edge_direction)
    { }
};
