#pragma once

#include "query/id.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpEdge : public Op {
public:
    const Id from;
    const Id to;
    const Id type;
    const Id edge;

    OpEdge(Id from, Id to, Id type, Id edge) :
        from (from),
        to   (to),
        type (type),
        edge (edge) { }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpEdge>(*this);
    }

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

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        if (from.is_var()) {
            res.insert(from.get_var());
        }
        if (to.is_var()) {
            res.insert(to.get_var());
        }
        if (type.is_var()) {
            res.insert(type.get_var());
        }
        if (edge.is_var()) {
            res.insert(edge.get_var());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpEdge(";
        os << from << ", ";
        os << to << ", ";
        os << type << ", ";
        os << edge;
        return os << ")\n";
    }
};
} // namespace MQL
