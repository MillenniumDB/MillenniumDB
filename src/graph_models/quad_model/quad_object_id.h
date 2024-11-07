#pragma once

#include <string>

#include "graph_models/object_id.h"

namespace QuadObjectId {
    // fixed node inside from the grammar
    // fixedNodeInside: identifier | ANON_ID | EDGE_ID | value;
    // example: `N1` | `_a1` | `_e1` | value: (`1` | `-1.1` | `"string"` | `false`) `
    ObjectId get_fixed_node_inside(const std::string&);

    // example: `_e1`
    ObjectId get_edge(const std::string&);

    // example: `N1`
    ObjectId get_named_node(const std::string&);

    // receives a string without surrounding double quotes
    ObjectId get_string(const std::string&);

    // value: numericValue | STRING | boolValue
    // example `-1.1` | `"string"` | `false`
    inline ObjectId get_value(const std::string& v) {
        return get_fixed_node_inside(v);
    }
};
