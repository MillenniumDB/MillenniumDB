#pragma once

#include <string>

#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/edge.h"
#include "base/query/var.h"

struct QueryElementToString {
    std::string operator()(const Var& v) {
        return v.name;
    }

    std::string operator()(const NamedNode& n) {
        return n.name;
    }

    std::string operator()(const std::string& s) {
        return '"' + s + '"';
    }

    std::string operator()(AnonymousNode n) {
        return "_a" + std::to_string(n.id);
    }

    std::string operator()(Edge e) {
        return "_e" + std::to_string(e.id);
    }

    std::string operator()(int64_t i) {
        return std::to_string(i);
    }

    std::string operator()(bool b) {
        return b ? "true" : "false";
    }

    std::string operator()(float f) {
        return std::to_string(f);
    }
};
