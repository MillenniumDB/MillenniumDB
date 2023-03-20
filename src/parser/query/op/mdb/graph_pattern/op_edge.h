#pragma once

#include "base/query/query_element.h"
#include "parser/query/op/op.h"

namespace MDB {

class OpEdge : public Op {
public:
    const QueryElement from;
    const QueryElement to;
    const QueryElement type;
    const QueryElement edge;

    OpEdge(const std::string& from, const std::string& to, const std::string& type, const std::string& edge) :
        from(QueryElement::deduce(from)),
        to(QueryElement::deduce(to)),
        type(QueryElement::deduce(type)),
        edge(QueryElement::deduce(edge)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpEdge& other) const {
        if (from < other.from) {
            return true;
        } else if (other.from < from) {
            return false;
        } else if (to < other.to) {
            return true;
        } else if (other.to < to) {
            return false;
        } else if (edge < other.edge) {
            return true;
        } else if (other.edge < edge) {
            return false;
        } else {
            return type < other.type;
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
        if (type.is_var()) {
            res.insert(type.to_var());
        }
        if (edge.is_var()) {
            res.insert(edge.to_var());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        return os << "OpEdge(" << from << ", " << to << ", " << type << ", " << edge << ")\n";
    };
};
} // namespace MDB