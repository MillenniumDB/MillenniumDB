#pragma once

#include "base/query/query_element.h"
#include "parser/query/op/op.h"

class OpIsolatedTerm : public Op {
public:
    const QueryElement term;

    OpIsolatedTerm(const QueryElement& term) :
        term (term) { }

    OpIsolatedTerm(QueryElement&& term) :
        term (std::move(term)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpIsolatedTerm& other) const {
        return term < other.term;
    }

    std::set<Var> get_vars() const override { return std::set<Var>(); }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override {
        os << std::string(indent, ' ');
        os << "OpIsolatedTerm(" << term << ")\n";
        return os;
    };
};
