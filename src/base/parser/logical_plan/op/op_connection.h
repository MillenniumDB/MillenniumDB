#ifndef BASE__OP_CONNECTION_H_
#define BASE__OP_CONNECTION_H_

#include <string>
#include <vector>

#include "base/ids/node_id.h"
#include "base/parser/logical_plan/op/op.h"

class OpConnection : public Op {
public:
    const NodeId from;
    const NodeId to;
    const NodeId edge;
    const std::vector<NodeId> types;

    OpConnection(NodeId _from, NodeId _to, NodeId _edge, std::vector<NodeId> _types) :
        from  (_from),
        to    (_to),
        edge  (_edge),
        types (std::move(_types)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpConnection& other) const {
        if (from < other.from) {
            return true;
        } else if (from > other.from) {
            return false;
        } else if (to < other.to) {
            return true;
        } else if (to > other.to) {
            return false;
        } else if (edge < other.edge) {
            return true;
        } else if (edge > other.edge) {
            return false;
        } else {
            return types < other.types;
        }
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        if (from.is_var()) {
            res.insert(from.to_var());
        }
        if (to.is_var()) {
            res.insert(to.to_var());
        }
        for (auto& type : types) {
            if (type.is_var()) {
                res.insert(type.to_var());
            }
        }
        if (edge.is_var()) {
            res.insert(edge.to_var());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpConnection( (" << from << ")-[" << edge;

        for (auto& type : types) {
            os << " " << type;
        }

        os << "]->(" << to <<") )\n";
        return os;
    };
};

#endif // BASE__OP_CONNECTION_H_
