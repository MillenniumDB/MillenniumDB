#ifndef BASE__NODE_ID_H_
#define BASE__NODE_ID_H_

#include <cassert>
#include <ostream>
#include <string>
#include <variant>

#include "base/graph/graph_object.h"
#include "base/parser/logical_plan/node_name.h"
#include "base/parser/logical_plan/var.h"

struct NodeIdToStringVisitor {
    NodeIdToStringVisitor() { }

    std::string operator()(const Var& v) {
        return v.name;
    }

    std::string operator()(const NodeName& n) {
        return n.name;
    }

    std::string operator()(const AnonymousNode& n) {
        return "_a" + std::to_string(n.id);
    }

    std::string operator()(const Edge& e) {
        return "_c" + std::to_string(e.id);
    }

    std::string operator()(const std::string& s) {
        return s;
    }

    std::string operator()(const int64_t i) {
        return std::to_string(i);
    }

    std::string operator()(const bool b) {
        return b ? "true" : "false";
    }

    std::string operator()(const float f) {
        return std::to_string(f);
    }
};

struct NodeIdToGraphObjectVisitor {
    NodeIdToGraphObjectVisitor() { }

    GraphObject operator()(const Var&) {
        throw std::logic_error("Var cannot be converted into graph_object");
    }

    GraphObject operator()(const NodeName& s) {
        return GraphObject::make_identifiable(s.name);
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

class NodeId {
public:
    std::variant<Var, NodeName, AnonymousNode, Edge, std::string, bool, int64_t, float> value;

    NodeId(Var var) :
        value (std::move(var)) { }

    NodeId(NodeName node_name) :
        value (std::move(node_name)) { }

    NodeId(AnonymousNode anon) :
        value (anon) { }

    NodeId(Edge edge) :
        value (edge) { }

    NodeId(std::string _value) :
        value (std::move(_value)) { }

    NodeId(bool _value) :
        value (_value) { }

    NodeId(int64_t _value) :
        value (_value) { }

    NodeId(float _value) :
        value (_value) { }

    ~NodeId() = default;

    inline bool operator==(const NodeId& rhs) const noexcept { return value == rhs.value; }
    inline bool operator!=(const NodeId& rhs) const noexcept { return value != rhs.value; }
    inline bool operator< (const NodeId& rhs) const noexcept { return value <  rhs.value; }
    inline bool operator<=(const NodeId& rhs) const noexcept { return value <= rhs.value; }
    inline bool operator> (const NodeId& rhs) const noexcept { return value >  rhs.value; }
    inline bool operator>=(const NodeId& rhs) const noexcept { return value >= rhs.value; }

    inline bool is_var() const {
        return std::holds_alternative<Var>(value);
    }

    inline std::string to_string() const {
        return std::visit(NodeIdToStringVisitor(), value);
    }

    inline Var to_var() const {
        assert(std::holds_alternative<Var>(value));
        return std::get<Var>(value);
    }

    GraphObject to_graph_object() const {
        return std::visit(NodeIdToGraphObjectVisitor(), value);
    }

    friend std::ostream& operator<<(std::ostream& os, const NodeId& node_id) {
        return os << node_id.to_string();
    }
};

#endif // BASE__NODE_ID_H_
