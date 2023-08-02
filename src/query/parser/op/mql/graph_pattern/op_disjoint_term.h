#pragma once

#include "graph_models/object_id.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpDisjointTerm : public Op {
public:
    const ObjectId term;

    OpDisjointTerm(ObjectId term) :
        term (term) { }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpDisjointTerm>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpDisjointTerm& other) const {
        return term < other.term;
    }

    std::set<VarId> get_all_vars() const override {
        return { };
    }

    std::set<VarId> get_scope_vars() const override {
        return { };
    }

    std::set<VarId> get_safe_vars() const override {
        return { };
    }

    std::set<VarId> get_fixable_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpDisjointTerm(";
        os << term;
        os << ")\n";
        return os;
    }
};
} // namespace MQL
