#pragma once

#include "base/graph_object/graph_object.h"
#include "base/query/query_element.h"

struct QueryElementToGraphObject {
    GraphObject operator()(const QueryElement&);

    GraphObject operator()(const Var&);
    GraphObject operator()(const NamedNode&);
    GraphObject operator()(const AnonymousNode&);
    GraphObject operator()(const Edge&);
    GraphObject operator()(const std::string&);
    GraphObject operator()(bool);
    GraphObject operator()(int64_t);
    GraphObject operator()(float);
};
