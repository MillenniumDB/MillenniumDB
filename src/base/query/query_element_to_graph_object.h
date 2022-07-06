#pragma once

#include "base/exceptions.h"
#include "base/graph_object/graph_object.h"
#include "base/query/named_node.h"
#include "base/query/var.h"

struct QueryElementToGraphObject {
    GraphObject operator()(const Var&) {
        throw LogicException("Var cannot be converted into graph_object");
    }

    GraphObject operator()(const NamedNode& s) {
        return GraphObject::make_named_node(s.name);
    }

    GraphObject operator()(const AnonymousNode& n) {
        return GraphObject::make_anonymous(n.id);
    }

    GraphObject operator()(const Edge& e) {
        return GraphObject::make_edge(e.id);
    }

    GraphObject operator()(const std::string& s) {
        return GraphObject::make_string(s);
    }

    GraphObject operator()(const int64_t i) {
        return GraphObject::make_int(i);
    }

    GraphObject operator()(const bool b) {
        return GraphObject::make_bool(b);
    }

    GraphObject operator()(const float f) {
        return GraphObject::make_float(f);
    }
};
